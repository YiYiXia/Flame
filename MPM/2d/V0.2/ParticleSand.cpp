#include"ParticleSand.h"

ParticleSand::ParticleSand()
{

}

ParticleSand::~ParticleSand()
{

}

ParticleSand::ParticleSand(Vector2d pos, Vector2d vel, double mass, int num , double lambda, double mu )
{
	position = pos;
	velocity = vel;
	this->mass = mass;
	alpha = 0.267765;
	q = 0.0;
	V_c = 0.0;
	index = num;
	Lambda = lambda;
	Mu = mu;
	float dc = random_number(0, 0.4);
	color = Vector3d(0.4+dc, 0.27+dc , 0.0);
}

void ParticleSand::Update()
{
	updatePos();
	updateGradient();
	applyPlasticity();
}

void ParticleSand::updatePos()
{
	position += DT*velocity;
	//if (position[0]<0||position[1]<0||position[1]>5||position[0]>5)
	//{
	//	cout << velocity[0] << "  " << velocity[1] << endl;
	//}

}

void ParticleSand::updateGradient()
{

	velocity_gradient *= DT;
	velocity_gradient = velocity_gradient + Matrix2d::Identity();
	def_elastic = velocity_gradient * def_elastic;
}

Matrix2d ParticleSand::energyDerivative()
{
	JacobiSVD<Eigen::MatrixXd>svd(def_elastic, ComputeThinU | ComputeThinV);
	svd_u = svd.matrixU();
	svd_v = svd.matrixV();
	svd_s = svd_u.inverse()*def_elastic*svd_v.transpose().inverse();
	Matrix2d T = Derivative(svd_s);
	Matrix2d A;
	A = svd_u*T*svd_v.transpose()*def_elastic.transpose();
	return volume*A;

	//double harden = exp(HARDENING*(1 - def_plastic.determinant()));
	//double Je = def_elastic.determinant();
	//Matrix2d out;
	//Matrix2d first = 2 * mu*(def_elastic - svd_u*svd_v.transpose())*def_elastic.transpose();
	//Matrix2d second = lambda*Je*(Je - 1)*Matrix2d::Identity();
	////Add in the primary contour term
	//out = first + second;
	//return volume *harden* out;

}

Matrix2d ParticleSand::Derivative(Matrix2d s)
{
	Matrix2d s_ln = Matrix2d::Zero();
	Matrix2d s_inverse = Matrix2d::Zero();
	Matrix2d out;
	s_ln(0, 0) = log(s(0, 0));
	s_ln(1, 1) = log(s(1, 1));
	s_inverse(0, 0) = 1.0 / s(0, 0);
	s_inverse(1, 1) = 1.0 / s(1, 1);
	out = 2 * Mu*s_inverse*s_ln + Lambda*s_ln.trace()*s_inverse;
	return out;
}

void ParticleSand::applyPlasticity()
{
	Matrix2d def_all = def_elastic * def_plastic;
	JacobiSVD<Eigen::MatrixXd>svd(def_elastic, ComputeThinU | ComputeThinV);
	svd_u = svd.matrixU();
	svd_v = svd.matrixV();
	svd_s = svd_u.inverse()*def_elastic*svd_v.transpose().inverse();
	double behind = def_elastic.determinant();
	Matrix2d T = project();
	def_elastic = svd_u*T*svd_v.transpose();
	double ahead = def_elastic.determinant();
	V_c = V_c - log(ahead) + log(behind);
	def_plastic = svd_v*T.inverse()*svd_s*svd_v.transpose()*def_plastic;
	q = q + delta_q; 
	double fai = h0 + (h1*q - h3)*exp(-h2*q);
	//cout << fai << endl;
	fai = fai*M_PI / 180.0;
	//double fai = 0;

	alpha = sqrt(2.0 / 3.0)*(2.0 * sin(fai) / (3.0 - sin(fai)));
	//alpha 为0时，整个运动像流体
																
}

Matrix2d ParticleSand::project()
{
	Matrix2d s_ln = Matrix2d::Zero();
	s_ln(0, 0) = log(svd_s(0, 0));
	s_ln(1, 1) = log(svd_s(1, 1));
	s_ln = s_ln + V_c / 2.0*Matrix2d::Identity();
	Matrix2d s_ln_t = s_ln - (s_ln.trace() / 2)*Matrix2d::Identity();
	double f;
	double delta_gama;
	f = sqrt(s_ln_t(0, 0)*s_ln_t(0, 0) + s_ln_t(1, 1)*s_ln_t(1, 1));
	if (f <= 0 || s_ln.trace() > 0)
	{
		//cout << 1 << endl;
		delta_q = sqrt(s_ln(0, 0)*s_ln(0, 0) + s_ln(1, 1)*s_ln(1, 1));
		return Matrix2d::Identity();
	}

	delta_gama = f + (lambda + mu) / mu*s_ln.trace()*alpha;
	if (delta_gama <= 0)
	{
		//cout << 2 << endl;
		delta_q = 0;
		return svd_s;
	}
	Matrix2d H = s_ln - delta_gama / f*s_ln_t;
	H(0, 0) = exp(H(0, 0));
	H(1, 1) = exp(H(1, 1));
	delta_q = delta_gama;
	//cout << 2 << endl;
	//cout << H(0, 0) <<" "<< H(1, 1) << endl;
	return H;


	/*delta_q = 0;
	return svd_s;*/
}