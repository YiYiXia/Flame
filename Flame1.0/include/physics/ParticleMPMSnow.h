#pragma once
#include"ParticleMPM.h"

typedef struct SnowParameters
{
	double Lambda;
	double Mu;
	double CRIT_COMPRESS;
	double CRIT_STRETCH;
	double HARDENING;
	double mass;
}SnowParameters;

class ParticleMPMSnow
	:public ParticleMPM
{
public:
	ParticleMPMSnow();
	ParticleMPMSnow(Vector2d pos, Vector2d vel, double mass, int num, double lambda, double mu,double harden,double COMPRESS, double STRETCH);
	ParticleMPMSnow(Vector2d pos, Vector2d vel, int num, SnowParameters p, Vector3d PColor);
	~ParticleMPMSnow();
	
	double Lambda;
	double Mu;
	double CRIT_COMPRESS;
	double CRIT_STRETCH;
	double HARDENING;
	void UpdatePos();
	void UpdateGradient();
	void ApplyPlasticity();
	Matrix2d EnergyDerivative();
	void MPMUpdate();
	void DeltaForce();
};
