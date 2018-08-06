#pragma once
#include<map>

#include"../physics/ParticleCloud.h"
#include"../common/Parameters.h"
#include"../physics/Line.h"
#include"../render/Camera2D.h"

using namespace std;

typedef struct SDFInfo 
{
	SDF* sdf;
	SDFType stype;
	ParticleType ptype;
	Vector2d velocity;
	Vector3d color;
};                                               //the information of SDF used in the scene

class Scene
{
public:
	Scene();
	~Scene();
	vector<SDFInfo> SDFlist;                     //list of objects in scene
	vector<Particle*> pointlist;                 //list of particles in scene
	vector<Line*> linelist;                      //list of line object in scene
	int frames;                                  //frames in simulation
	Camera camera;                               //camera in scene

	void AddObject(
		SDF* polygon, 
		SDFType Type, 
		Vector2d velocity = Vector2d::Zero(), 
		ParticleType PType=NONE,
		Vector3d ColorFill=Vector3d(-1,-1,-1));

};