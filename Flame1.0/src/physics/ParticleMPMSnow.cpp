#include"../../include/physics/ParticleMPMSnow.h"

ParticleMPMSnow::ParticleMPMSnow()
{

}

ParticleMPMSnow::ParticleMPMSnow(Vector2d pos, Vector2d vel, double mass, int num, double lambda, double mu, double harden, double COMPRESS, double STRETCH)
{
	position = pos;
	velocity = vel;
	this->mass = mass;
	index = num;
	Lambda = lambda;
	Mu = mu;
	HARDENING = harden;
	CRIT_COMPRESS = 1 - COMPRESS;
	CRIT_STRETCH = 1 + STRETCH;
	color = Vector3d(0, 0.3, 0.7);
	p_type = SNOW;
}

ParticleMPMSnow::ParticleMPMSnow(Vector2d pos, Vector2d vel, int num, SnowParameters p, Vector3d PColor)
{
	position = pos;
	velocity = vel;
	index = num;
	this->mass = p.mass;
	Lambda = p.Lambda;
	Mu = p.Mu;
	HARDENING = p.HARDENING;
	CRIT_COMPRESS = 1 - p.CRIT_COMPRESS;
	CRIT_STRETCH = 1 + p.CRIT_STRETCH;
	if (PColor[0] < 0) color = Vector3d(0, 0.3, 0.7);
	else color = PColor;
	p_type = SNOW;
}

ParticleMPMSnow::~ParticleMPMSnow()
{
}

void ParticleMPMSnow::UpdatePos()
{
	position += DT*velocity;
}

void ParticleMPMSnow::UpdateGradient()
{
	velocity_gradient *= DT;
	velocity_gradient = velocity_gradient + Matrix2d::Identity();
	def_elastic = velocity_gradient * def_elastic;
}

void ParticleMPMSnow::ApplyPlasticity()
{
	Matrix2d def_all = def_elastic * def_plastic;
	JacobiSVD<Eigen::MatrixXd>svd(def_elastic, ComputeThinU | ComputeThinV);
	svd_u = svd.matrixU();
	svd_v = svd.matrixV();
	svd_s = svd_u.inverse()*def_elastic*svd_v.transpose().inverse();
	for (int i = 0; i < 2; i++)
	{
		if (svd_s(i, i) < CRIT_COMPRESS)
			svd_s(i, i) = CRIT_COMPRESS;
		else if (svd_s(i, i) > CRIT_STRETCH)
			svd_s(i, i) = CRIT_STRETCH;
	}
	Matrix2d temp = svd_s;
	temp(0, 0) = 1 / temp(0, 0);
	temp(1, 1) = 1 / temp(1, 1);
	def_elastic = svd_u*svd_s*svd_v.transpose();
	def_plastic = svd_v*temp*svd_u.transpose()*def_all;
}

Matrix2d ParticleMPMSnow::EnergyDerivative()
{
	double harden = exp(HARDENING*(1 - def_plastic.determinant()));
	double Je = def_elastic.determinant();
	Matrix2d out;
	Matrix2d first = 2 * Mu*(def_elastic - svd_u*svd_v.transpose())*def_elastic.transpose();
	Matrix2d second = Lambda*Je*(Je - 1)*Matrix2d::Identity();
	out = first + second;
	return volume *harden* out;
}

void ParticleMPMSnow::MPMUpdate()
{
	UpdatePos();
	UpdateGradient();
	ApplyPlasticity();
}
void ParticleMPMSnow::DeltaForce()
{
	Matrix2d del_elastic = DT*delta_v*def_elastic;
	double y = (polar_r(0, 0) * del_elastic(0, 1) + polar_r(1, 0) * del_elastic(1, 1)) -
		(polar_r(0, 1) * del_elastic(0, 0) + polar_r(1, 1) * del_elastic(0, 1));
	double x = y / (polar_s(0, 0) + polar_s(1, 1));
	Matrix2d del_rotate;
	del_rotate(0, 0) = -polar_r(0, 1) * x;
	del_rotate(0, 1) = polar_r(0, 0) * x;
	del_rotate(1, 0) = -polar_r(1, 1) * x;
	del_rotate(1, 1) = polar_r(1, 0) * x;


	//JF^-T
	Matrix2d cofactor;
	cofactor(0, 0) = def_elastic(1, 1);
	cofactor(0, 1) = -def_elastic(1, 0);
	cofactor(1, 0) = -def_elastic(0, 1);
	cofactor(1, 1) = def_elastic(0, 0);

	Matrix2d del_cofactor;
	del_cofactor(0, 0) = del_elastic(1, 1);
	del_cofactor(0, 1) = -del_elastic(1, 0);
	del_cofactor(1, 0) = -del_elastic(0, 1);
	del_cofactor(1, 1) = del_elastic(0, 0);
	//cout << u[0] << endl;
	Matrix2d Ap = del_elastic - del_rotate;
	Ap *= 2 * Mu;
	Matrix2d temp = cofactor*del_elastic.transpose();//矩阵对应元素相乘求和

	cofactor *= temp.trace();
	del_cofactor *= (def_elastic.determinant() - 1);
	cofactor += del_cofactor;
	cofactor *= Lambda;
	Ap += cofactor;

	delta_f = volume*(Ap*def_elastic.transpose());
}