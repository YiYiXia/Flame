#include"ParticleSnow.h"
ParticleSnow::ParticleSnow()
{

}

ParticleSnow::ParticleSnow(Vector2d pos, Vector2d vel, double mass, int num, double lambda, double mu, double harden, double COMPRESS, double STRETCH)
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
}

ParticleSnow::~ParticleSnow()
{

}

void ParticleSnow::updatePos()
{
	position += DT*velocity;
}

void ParticleSnow::updateGradient()
{
	velocity_gradient *= DT;
	velocity_gradient = velocity_gradient + Matrix2d::Identity();
	def_elastic = velocity_gradient * def_elastic;
}

void ParticleSnow::applyPlasticity()
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

Matrix2d ParticleSnow::energyDerivative()
{
	double harden = exp(HARDENING*(1 - def_plastic.determinant()));

	double Je = def_elastic.determinant();
	Matrix2d out;
	Matrix2d first = 2 * Mu*(def_elastic - svd_u*svd_v.transpose())*def_elastic.transpose();
	Matrix2d second = Lambda*Je*(Je - 1)*Matrix2d::Identity();
	//Add in the primary contour term
	out = first + second;
	//Add hardening and volume

	return volume *harden* out;
}

void ParticleSnow::Update()
{
	updatePos();
	updateGradient();
	applyPlasticity();
}