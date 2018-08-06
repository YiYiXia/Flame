#include"../../include/render/Camera2D.h"

Camera::Camera()
{
	Rotate = Matrix2d::Identity();
	camera_distance = 1.0;
}
Camera::~Camera()
{

}

Vector2i Camera::Project(Vector2d pixel)
{
	Vector2d relate_position = pixel - look_at;
	relate_position = Rotate*relate_position*(camera_distance / view_distance);
	Vector2i output;
	output[0] = relate_position[0] / window_cell_x+ window_center[0];
	output[1] = relate_position[1] / window_cell_y+ window_center[1];
	return output;
}

double Camera::Scale()
{
	return camera_distance / view_distance;
}