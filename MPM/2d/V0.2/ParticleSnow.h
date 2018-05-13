#pragma once
#include"Particle.h"

class ParticleSnow
	:public Particle
{
public:
	ParticleSnow();
	ParticleSnow(Vector2d pos, Vector2d vel, double mass, int num, double lambda, double mu,double harden,double COMPRESS, double STRETCH);
	~ParticleSnow();
	
	double Lambda;
	double Mu;
	double CRIT_COMPRESS;
	double CRIT_STRETCH;
	double HARDENING;

	void updatePos();
	void updateGradient();
	void applyPlasticity();
	Matrix2d energyDerivative();
	void Update();
};
