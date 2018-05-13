#pragma once
#include"Grid.h"
#include"ParticleCloud.h"
#include"Parameters.h"
#include<map>
//这一层之后可能要改，因为要引入SPH，所以这里有很多东西就不应该出现了
using namespace std;
class Scene
{
public:
	Scene(int resolution);
	~Scene();
	map<SDF*, ParticleType> SPaType;//构建SDF的粒子类型
	Grid* grid;
	ParticleCloud* bar;
	int Resolution;
	bool source;
	int frames;

	bool key;
	int startframes, endframes;

	void Initial();
	void Update();
	void Draw();

	void AddObject(ParticleType Type, SDF* polygon, Vector2d velocity = Vector2d::Zero());//加粒子物体的时候，默认其初始速度为零
	void AddPolygon(SDFType Type, SDF* polygon);
	void AddSource(SDFType Type, SDF* polygon, ParticleType PType, Vector2d velocity);
};