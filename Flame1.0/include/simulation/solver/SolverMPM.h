#pragma once
#include"Solver.h"


class SolverMPM
	:public Solver
{
public:
	SolverMPM();
	~SolverMPM();
	ParticleCloudMPM* bar;
	GridMPM* grid;
	int Resolution;
	int InsertTime;
	void Initial(SolverType SType);
	void Update();
	void PointListCheck();
	void SourceCheck();//这一部分还要重构（多个源的情况）

};