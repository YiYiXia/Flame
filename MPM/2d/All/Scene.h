#pragma once
#include"Grid.h"
#include"ParticleCloud.h"
#include"Parameters.h"
using namespace std;
class Scene
{
public:
	Scene();
	~Scene();
	
	Grid* grid;
	ParticleCloud* bar;
	int num;
	void Initial();
	void Update();
	void Draw();
};