#pragma once
#include<Eigen/Dense>
using namespace Eigen;
class Camera
{
public:
	Camera();
	~Camera();
	Vector2d look_at;
	Vector2i window_center;
	Matrix2d Rotate;
	double view_distance;
	double window_cell_x;
	double window_cell_y;
	double Scale();
	
	Vector2i Project(Vector2d pixel);
private:
	double camera_distance;
};