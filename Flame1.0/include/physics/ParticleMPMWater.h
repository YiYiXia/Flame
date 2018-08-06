#pragma once
#include"ParticleMPM.h"
typedef struct WaterParameters
{
	double Gama;
	double Hardness;
	double mass;
}WaterParameters;

class ParticleMPMWater
	:public ParticleMPM
{
public:
	ParticleMPMWater();
	ParticleMPMWater(Vector2d pos, Vector2d vel, double mass, int num, double gama, double hardness);
	ParticleMPMWater(Vector2d pos, Vector2d vel, int num, WaterParameters p, Vector3d PColor);
	~ParticleMPMWater();
	double J;
	double Gama;
	double Hardness;
	void UpdatePos();
	void UpdateGradient();
	Matrix2d EnergyDerivative();
	void MPMUpdate();
	void DeltaForce();
};