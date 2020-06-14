#pragma once
#include"ParticleMPM.h"

typedef struct SandParameters 
{
	double Lambda;
	double Mu;
	double H0;
	double H1;
	double H2;
	double H3;
	double mass;
}SandParameters;

enum SandType 
{
	DRY,
	WET
};

class ParticleMPMSand
	:public ParticleMPM
{
public:
	ParticleMPMSand();
	ParticleMPMSand(Vector2d pos, Vector2d vel, double mass, int num = 0, double lambda = 0, double mu = 0);
	ParticleMPMSand(Vector2d pos, Vector2d vel, int num, SandParameters p, SandType Type, Vector3d PColor);
	~ParticleMPMSand();
	double alpha;
	double delta_q;
	double q;
	double V_c;
	double C_cp0,C_cp;
	double Lambda;
	double Mu;
	double H0;
	double H1;
	double H2;
	double H3;
	
	void UpdatePos();
	void UpdateGradient();
	void ApplyPlasticity();
	Matrix2d EnergyDerivative();
	Matrix2d Derivative(Matrix2d s);
	Matrix2d Project();
	void DeltaForce();
	void MPMUpdate();
};