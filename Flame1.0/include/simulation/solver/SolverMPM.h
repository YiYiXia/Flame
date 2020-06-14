#pragma once
#include"Solver.h"
#include"../grid/GridMPM.h"

class SolverMPM
	:public Solver
{
public:
	SolverMPM();
	~SolverMPM();
	ParticleCloudMPM* bar;
	GridMPM* grid;
	int Resolution;
	void SolverInitial(IntegralType SType, vector<SDFInfo>* SDFlist);
	void SolverUpdate();
	void AddParticle(vector<Vector2d> *particleList, ParticleType particleType, Vector2d particleVelocity, Vector3d particleColor);
private:
	void InformationReturn();
};
