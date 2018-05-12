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
	bool source;
	int num;
	void Initial();
	void Update();
	void Draw();

	void AddObject(ParticleType Type, SDF* polygon);
	void AddPolygon(SDFType Type, SDF* polygon);
};