#pragma once
#include"Particle.h"

class ParticleWater
	:public Particle 
{
public:
	ParticleWater();
	ParticleWater(Vector2d pos, Vector2d vel, double mass, int num, double gama, double hardness);
	~ParticleWater();
	double J;
	double Gama;
	double Hardness;
	void updatePos();
	void updateGradient();
	Matrix2d energyDerivative();
	void Update();
};