#include"ParticleElastic.h"
ParticleElastic::ParticleElastic()
{

}
ParticleElastic::~ParticleElastic()
{

}

ParticleElastic::ParticleElastic(Vector2d pos, Vector2d vel, double mass, int num, double lamda, double mu)
{
	position = pos;
	velocity = vel;
	this->mass = mass;
	index = num;
	Lambda = lamda;
	Mu = mu;
	color = Vector3d(0.4, 0.8, 0.2);;
}

Matrix2d ParticleElastic::energyDerivative()
{
	double Je = def_elastic.determinant();
	Matrix2d out;
	Matrix2d first = 2 * mu*(def_elastic - svd_u*svd_v.transpose())*def_elastic.transpose();
	Matrix2d second = lambda*Je*(Je - 1)*Matrix2d::Identity();
	//Add in the primary contour term
	out = first + second;
	//Add hardening and volume

	return volume * out;
}

void ParticleElastic::Update()
{
	updatePos();
	updateGradient();
	applyPlasticity();
}

void ParticleElastic::updatePos()
{
	position += DT*velocity;
}
void ParticleElastic::updateGradient()
{
	velocity_gradient *= DT;
	velocity_gradient = velocity_gradient + Matrix2d::Identity();
	def_elastic = velocity_gradient * def_elastic;
}
void ParticleElastic::applyPlasticity()
{
	JacobiSVD<Eigen::MatrixXd>svd(def_elastic, ComputeThinU | ComputeThinV);
	svd_u = svd.matrixU();
	svd_v = svd.matrixV();
}