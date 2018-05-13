#pragma once
#include"Particle.h"
class ParticleElastic
	:public Particle
{
public:
	ParticleElastic();
	ParticleElastic(Vector2d pos, Vector2d vel, double mass, int num, double lamda, double mu);
	~ParticleElastic();
	double Lambda;
	double Mu;
	Matrix2d energyDerivative();
	void Update();
	void updatePos();
	void updateGradient();
	void applyPlasticity();
};