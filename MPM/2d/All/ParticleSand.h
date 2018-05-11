#pragma once
#include"Particle.h"

class ParticleSand
	:public Particle
{
public:
	ParticleSand();
	ParticleSand(Vector2d pos, Vector2d vel, double mass, int num = 0, double lambda = 0, double mu = 0);
	~ParticleSand();
	double alpha;
	double delta_q;
	double q;
	double V_c;
	double Lambda;
	double Mu;
	void updatePos();
	void updateGradient();
	void applyPlasticity();
	Matrix2d energyDerivative();
	Matrix2d Derivative(Matrix2d s);
	Matrix2d project();
	void Update();
};