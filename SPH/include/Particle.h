#pragma once
#include <Eigen>

using namespace Eigen;

class Particle
{
public:
	Vector2d	x;
	Vector2d	v;
	double		rho;
	double		p;
	double		m;
	
public:
	Particle();
	Particle(Vector2d pos, Vector2d velo, double density, double mass);
	~Particle();
};