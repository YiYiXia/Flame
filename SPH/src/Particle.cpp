#include "Particle.h"

Particle::Particle()
{
}

Particle::Particle(Vector2d pos, Vector2d velo, double density, double mass)
{
	x = pos;
	v = velo;
	rho = density;
	p = 0;
	m = mass;
}

Particle::~Particle()
{
}