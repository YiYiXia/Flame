#include"Particle.h"
Particle::Particle()
{

}
Particle::Particle(Vector3f pos, Vector3f vel, float mass)
{
	position = pos;
	velocity = vel;
	this->mass = mass;
	def_elastic = Matrix3f::Identity();
	svd_u = Matrix3f::Identity();
	svd_v = Matrix3f::Identity();
	svd_s = Matrix3f::Identity();
	polar_r = Matrix3f::Identity();
	polar_s = Matrix3f::Identity();
	
}
Particle::~Particle()
{

}

void Particle::updatePos()
{
	position += DT*velocity;
	//cout << position[0] << "  " << position[1] <<"  " << position[2] << endl;
}

void Particle::updateGradient()
{
	
	velocity_gradient *= DT;
	velocity_gradient = velocity_gradient+Matrix3f::Identity();
	def_elastic = velocity_gradient * def_elastic;
}

const Matrix3f Particle::energyDerivative()
{
	
	float Je = def_elastic.determinant();
	Matrix3f out;
	Matrix3f first = 2 * mu*(def_elastic - svd_u*svd_v.transpose())*def_elastic.transpose();
	Matrix3f second = lambda*Je*(Je - 1)*Matrix3f::Identity();
	//Add in the primary contour term
	out = first + second;
	//Add hardening and volume
	//cout << Je << endl;
	return volume * out;

}

void Particle::applyPlasticity()
{
	JacobiSVD<Eigen::MatrixXf>svd(def_elastic, ComputeThinU | ComputeThinV);
	svd_u = svd.matrixU();
	svd_v = svd.matrixV();
	//svd_s = svd_u.inverse()*def_elastic*svd_v.transpose().inverse();

}