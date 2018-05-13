#pragma once
#include"Parameters.h"
#include <GL/freeglut.h>
#include <Eigen/Dense>
#include"Scene.h"
#include"Render2D.h"
#include"MarchingCube.h"
using namespace std;
using namespace Eigen;
class Player2D 
{
public:
	Scene *scene;
	Render2D *render;
	Player2D();
	~Player2D();
	void Initial(int argc, char** argv);
	void Render();
	//void Show(int argc, char** argv);
	void Update();
	void DrawSurface();
	void DrawSDF(SDF sdf);
};