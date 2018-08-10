#pragma once
#include"../../scene/Scene.h"
#include"../../physics/Particle.h"

enum IntegralType
{
	EXPLICIT,
	IMPLICIT
};//Integral type such as explicit and implicit

enum SolverType
{
	MPM,
	MPMMulti,
	MAC
};//Solver tpye for different algorithm

class Solver
{
public:
	Solver();
	~Solver();
	IntegralType Type;//记录求解器显隐式类型
	vector<Particle*> particleList;
	vector<SDFObject*> *objectList;            //list of Object in scene
	vector<SDFBoundary*> *boundaryList;        //list of Boundary in scene
	vector<SDFInfo>* objList;
	virtual void SolverInitial(IntegralType SType, vector<SDFInfo>* SDFlist) = 0;
	virtual void SolverUpdate() = 0;
	virtual void AddParticle(vector<Vector2d> *particleList, ParticleType particleType, Vector2d particleVelocity, Vector3d particleColor) = 0;//Add particle into scene
};