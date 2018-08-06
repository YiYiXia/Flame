#pragma once
#pragma once
#include"Solver.h"


class SolverMAC
	:public Solver
{
public:
	SolverMAC();
	~SolverMAC();
	ParticleCloudMPM* bar;
	GridMAC* grid;
	int Resolution;
	void Initial(SolverType SType);
	void Update();
	void PointListCheck();
	void SourceCheck();//这一部分还要重构（多个源的情况）

};