#pragma once
#include"Solver.h"
#include"../grid/GridMPM.h"

class SolverMPMMulti
	:public Solver
{
public:
	SolverMPMMulti();
	~SolverMPMMulti();
	ParticleCloudMPM* sand;
	ParticleCloudMPM* water;
	GridMPM* grid_s;
	GridMPM* grid_w;
	double *cohesion_w; 
	int Resolution;
	void SolverInitial(IntegralType SType, vector<SDFInfo>* SDFlist);
	void SolverUpdate();
	void AddParticle(vector<Vector2d> *particleList, ParticleType particleType, Vector2d particleVelocity, Vector3d particleColor);
	void MomentaExchange();
	void Saturation();
private:
	void InformationReturn();
	
};