#pragma once
#include"Solver.h"

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
	void Initial(SolverType SType);
	void Update();
	void PointListCheck();
	void SourceCheck();
	void MomentaExchange();
	void Saturation();
};