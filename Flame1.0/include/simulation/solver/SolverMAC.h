#pragma once
#pragma once
#include"Solver.h"
#include"../grid/GridMPM.h"
#include"../grid/GridMAC.h"


class SolverMAC
	:public Solver
{
public:
	SolverMAC();
	~SolverMAC();
	ParticleCloudMPM* bar;
	GridMAC* grid;
	int Resolution;
	//void Initial(SolverType SType);
	//void Update();
	//void PointListCheck();
	//void SourceCheck();//这一部分还要重构（多个源的情况）
	void SolverInitial(IntegralType SType, vector<SDFInfo>* SDFlist);
	void SolverUpdate();
	void AddParticle(vector<Vector2d> *particleList, ParticleType particleType, Vector2d particleVelocity, Vector3d particleColor);
private:
	void InformationReturn();
};