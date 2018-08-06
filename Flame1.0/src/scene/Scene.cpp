#include"../../include/scene/Scene.h"

Scene::Scene()
{
	frames = 0;
	camera.look_at = Vector2d(VIEW_WIDTH / 2.0, VIEW_HEIGHT / 2.0 - 0.1);
	camera.window_cell_x = VIEW_WIDTH / (double)WINDOW_WIDTH;
	camera.window_cell_y = VIEW_HEIGHT / (double)WINDOW_HEIGHT;
	camera.window_center[0] = (int)VIEW_WIDTH / (2.0*camera.window_cell_x);
	camera.window_center[1] = (int)VIEW_HEIGHT/ (2.0*camera.window_cell_y);
	camera.view_distance = 1.0;
}

Scene::~Scene()
{
}

void Scene::AddObject(SDF* polygon, SDFType Type, Vector2d velocity, ParticleType PType, Vector3d ColorFill)
{
	polygon->type = Type;
	SDFInfo info;

	info.sdf = polygon;
	info.stype = Type;
	info.ptype =PType;
	info.velocity = velocity;
	info.color = ColorFill;
	SDFlist.push_back(info);
}