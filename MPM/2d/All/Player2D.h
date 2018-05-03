#pragma once
#include"Parameters.h"
#include <GL/freeglut.h>
#include <Eigen/Dense>
#include"Scene.h"
using namespace std;
using namespace Eigen;
class Player2D 
{
public:
	Scene *scene;
	Player2D();
	~Player2D();
	void Initial(int argc, char** argv);
	void Render();
	//void Show(int argc, char** argv);
	void Update();
};