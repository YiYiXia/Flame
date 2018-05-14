#include"Particle.h"
Particle::Particle()
{

}
Particle::Particle(Vector2f pos, Vector2f vel, float mass)
{
	position = pos;
	velocity = vel;
	this->mass = mass;
	def_elastic = Matrix2f::Identity();
	svd_u = Matrix2f::Identity();
	svd_v = Matrix2f::Identity();
	svd_s = Matrix2f::Identity();
	polar_r = Matrix2f::Identity();
	polar_s = Matrix2f::Identity();
	J = 1;
	B = Matrix2f::Zero();
}
Particle::~Particle()
{

}

