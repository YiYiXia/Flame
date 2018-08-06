#pragma once
#include"ParticleMPM.h"
typedef struct ElasticParameters
{
	double Lambda;
	double Mu;
	double mass;
} ElasticParameters;

class ParticleMPMElastic
	:public ParticleMPM
{
public:
	ParticleMPMElastic();
	ParticleMPMElastic(Vector2d pos, Vector2d vel, double mass, int num, double lamda, double mu);
	ParticleMPMElastic(Vector2d pos, Vector2d vel, int num, ElasticParameters p, Vector3d PColor);
	~ParticleMPMElastic();
	double Lambda;
	double Mu;
	Matrix2d EnergyDerivative();
	void MPMUpdate();
	void DeltaForce();
	void UpdatePos();
	void UpdateGradient();
	void ApplyPlasticity();
};