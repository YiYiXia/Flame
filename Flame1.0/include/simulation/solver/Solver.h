#pragma once
#include"../../scene/Scene.h"
#include"../grid/GridMPM.h"
#include"../grid/GridMAC.h"

//求解器积分类型，隐式，显式（还需要将半隐式的参数脱出）
enum SolverType
{
	EXPLICIT,
	IMPLICIT
};
class Solver
{
public:
	Solver();
	~Solver();
	Scene* scene;
	int frames;
	SolverType Type;//记录求解器显隐式类型
	void SceneControl();//负责根据多边形运动信息，控制其运动，
	virtual void Initial(SolverType SType) = 0;
	virtual void Update() = 0;
};