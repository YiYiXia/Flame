#include"../../include/physics/ParticleMPMSand.h"
#include"../../include/math/Function.h"

ParticleMPMSand::ParticleMPMSand()
{

}

ParticleMPMSand::~ParticleMPMSand()
{

}

ParticleMPMSand::ParticleMPMSand(Vector2d pos, Vector2d vel, int num, SandParameters p,SandType Type, Vector3d PColor)
{
	position = pos;
	velocity = vel;
	this->mass = mass;
	alpha = 0.267765;
	q = 0.0;
	V_c = 0.0;
	index = num;
	float dc = RandomNumber(0, 0.4);
	if (PColor[0] < 0) color = Vector3d(dc, dc, 0.0) + Vector3d(0.4, 0.27, 0.0);
	else color = Vector3d(dc, dc, 0.0) + PColor;
	Lambda = p.Lambda;
	Mu = p.Mu;
	H0 = p.H0;
	H1 = p.H1;
	H2 = p.H2;
	H3 = p.H3;
	this->mass = p.mass;
	if (Type == DRY)
		C_cp0 = C_cp = 0;

	if (Type == WET)
		C_cp0 = C_cp = -0.008;
	p_type = DRYSAND;
}

ParticleMPMSand::ParticleMPMSand(Vector2d pos, Vector2d vel, double mass, int num , double lambda, double mu )
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
	float dc = RandomNumber(0, 0.4);
	color = Vector3d(0.4+dc, 0.27+dc , 0.0);
	p_type = DRYSAND;
}

void ParticleMPMSand::MPMUpdate()
{
	UpdatePos();
	UpdateGradient();
	C_cp = C_cp0*(1 - coh);
	ApplyPlasticity();
}

void ParticleMPMSand::UpdatePos()
{
	position += DT*velocity;
}

void ParticleMPMSand::DeltaForce()
{

}

void ParticleMPMSand::UpdateGradient()
{

	velocity_gradient *= DT;
	velocity_gradient = velocity_gradient + Matrix2d::Identity();
	def_elastic = velocity_gradient * def_elastic;
}

Matrix2d ParticleMPMSand::EnergyDerivative()
{
	JacobiSVD<Eigen::MatrixXd>svd(def_elastic, ComputeThinU | ComputeThinV);
	svd_u = svd.matrixU();
	svd_v = svd.matrixV();
	svd_s = svd_u.inverse()*def_elastic*svd_v.transpose().inverse();
	Matrix2d T = Derivative(svd_s);
	Matrix2d A;
	A = svd_u*T*svd_v.transpose()*def_elastic.transpose();
	return volume*A;
}

Matrix2d ParticleMPMSand::Derivative(Matrix2d s)
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

void ParticleMPMSand::ApplyPlasticity()
{
	Matrix2d def_all = def_elastic * def_plastic;
	JacobiSVD<Eigen::MatrixXd>svd(def_elastic, ComputeThinU | ComputeThinV);
	svd_u = svd.matrixU();
	svd_v = svd.matrixV();
	svd_s = svd_u.inverse()*def_elastic*svd_v.transpose().inverse();
	double behind = def_elastic.determinant();
	Matrix2d T = Project();
	def_elastic = svd_u*T*svd_v.transpose();
	double ahead = def_elastic.determinant();
	V_c = V_c - log(ahead) + log(behind);
	def_plastic = svd_v*T.inverse()*svd_s*svd_v.transpose()*def_plastic;
	q = q + delta_q; 
	double fai = H0 + (H1*q - H3)*exp(-H2*q);
	fai = fai*M_PI / 180.0;
	alpha = sqrt(2.0 / 3.0)*(2.0 * sin(fai) / (3.0 - sin(fai)));													
}

Matrix2d ParticleMPMSand::Project()
{
	Matrix2d s_ln = Matrix2d::Zero();
	s_ln(0, 0) = log(svd_s(0, 0));
	s_ln(1, 1) = log(svd_s(1, 1));
	s_ln = s_ln + V_c / 2.0*Matrix2d::Identity();
	
	double c = C_cp;
	s_ln = s_ln + c*0.5 / alpha*Matrix2d::Identity();

	Matrix2d s_ln_t = s_ln - (s_ln.trace() / 2)*Matrix2d::Identity();
	double f;
	double delta_gama;
	f = sqrt(s_ln_t(0, 0)*s_ln_t(0, 0) + s_ln_t(1, 1)*s_ln_t(1, 1));
	if (f <= 0 || s_ln.trace() > 0)
	{
		delta_q = sqrt(s_ln(0, 0)*s_ln(0, 0) + s_ln(1, 1)*s_ln(1, 1));
		return Matrix2d::Identity();
	}
	delta_gama = f + (Lambda + Mu) / Mu*s_ln.trace()*alpha;
	if (delta_gama <= 0)
	{
		delta_q = 0;
		return svd_s;
	}
	Matrix2d H = s_ln - (delta_gama) / f*s_ln_t;
	H(0, 0) = exp(H(0, 0));
	H(1, 1) = exp(H(1, 1));
	delta_q = delta_gama;
	return H;
}
