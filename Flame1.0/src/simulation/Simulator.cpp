#include"../../include/simulation/Simulator.h"

Simulator::Simulator()
{
}

Simulator::~Simulator()
{

}

void Simulator::SimulatorInitial(Scene* scene, SolverType solverType, IntegralType integralType)
{
	sc = scene;
	//the type of child class is judged by the input information
	if (solverType == MPM) sol = new SolverMPM();
	if (solverType == MPMMulti) sol = new SolverMPMMulti();
	if (solverType == MAC) sol = new SolverMAC();
	//完成求解器的初始化，同时将积分类型赋值，将物体信息送给求解器
	sol->objectList = &sc->SDFObjectList;
	sol->boundaryList = &sc->SDFBoundaryList;
	sol->SolverInitial(integralType, &scene->SDFlist);
	for (int i = 0; i < sc->SDFSampleBoundList.size(); i++)
	{
		if (sc->SDFSampleBoundList[i]->appearTime == 0)
		{
			PointList list;
			list = sc->SDFSampleBoundList[i]->SampleParticle();
			sol->AddParticle(&list, sc->SDFSampleBoundList[i]->particleType, sc->SDFSampleBoundList[i]->sampleVelocity, sc->SDFlist[i].color);
		}
	}
}

void Simulator::SimulatorUpdate()
{
	for (int i = 0; i < SOLVER_STEPS; i++) 
	{
		InformationCheck();
		sol->SolverUpdate();
		sc->SceneUpdate();
		InformationUpdate();
		Flame_Frames++;
	}
}

void Simulator::InformationUpdate()
{
	int csize = sol->particleList.size();
	sc->pointlist.resize(csize);
	for (int i = 0; i < sol->particleList.size(); i++)
		sc->pointlist[i] = sol->particleList[i];
}

void Simulator::InformationCheck()
{
	if (Flame_Frames < endtime)
	{
		for (int i = 0; i < sc->SDFJetList.size(); i++)
		{
			if ((Flame_Frames - sc->SDFJetList[i]->insertTime) == sc->SDFJetList[i]->sampleTime)
			{
				PointList list;
				list = sc->SDFJetList[i]->SampleParticle();
				sol->AddParticle(&list, sc->SDFJetList[i]->particleType, sc->SDFJetList[i]->sampleVelocity, sc->SDFlist[i].color);
				sc->SDFJetList[i]->insertTime = Flame_Frames;
				sc->SDFJetList[i]->sampleCheck = true;
			}
		}
	}
	for (int i = 0; i < sc->SDFSampleBoundList.size(); i++)
	{
		if (Flame_Frames == sc->SDFSampleBoundList[i]->appearTime&&Flame_Frames > 0)
		{
			PointList list;
			list = sc->SDFSampleBoundList[i]->SampleParticle();
			sol->AddParticle(&list, sc->SDFSampleBoundList[i]->particleType, sc->SDFSampleBoundList[i]->sampleVelocity, sc->SDFlist[i].color);
		}
	}
}