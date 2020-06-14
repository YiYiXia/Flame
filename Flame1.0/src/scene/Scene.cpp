#include"../../include/scene/Scene.h"

Scene::Scene()
{
	Flame_Frames = 0;
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


void Scene::AddObject(
	SDFObject* polygon)//Object
{
	//polygon->type = Type;
	SDFInfo info;
	info.sdf = polygon;
	info.stype = polygon->type;
	SDFlist.push_back(info);
	SDFObjectList.push_back(polygon);
}

void Scene::AddObject(
	SDFSampleBound* polygon,
	Vector3d ColorFill)//SampleBound
{
	//polygon->type = Type;
	SDFInfo info;
	info.sdf = polygon;
	info.stype = polygon->type;
	info.color = ColorFill;
	SDFlist.push_back(info);
	SDFSampleBoundList.push_back(polygon);
}

void Scene::AddObject(
	SDFJet* polygon,
	Vector3d ColorFill)//Jet
{
	//polygon->type = Type;
	polygon->CalculateBarycenter();
	SDFInfo info;
	info.sdf = polygon;
	info.stype = polygon->type;
	info.color = ColorFill;
	SDFlist.push_back(info);
	SDFJetList.push_back(polygon);
}

void Scene::AddObject(
	SDFBoundary* polygon)//Boundary
{
	//polygon->type = Type;
	SDFInfo info;
	info.sdf = polygon;
	info.stype = polygon->type;
	SDFlist.push_back(info);
	SDFBoundaryList.push_back(polygon);
}

void Scene::SceneUpdate()
{

	for (int i = 0; i < SDFlist.size(); i++)
	{
		SDFlist[i].sdf->Control(Flame_Frames);
		SDFlist[i].sdf->SDFUpdate(DT);
	}
}


SDFObject* Scene::FindObject(string name)
{
	for (int i = 0; i < SDFObjectList.size(); i++)
		if (name == SDFObjectList[i]->sdfName) return SDFObjectList[i];
}

SDFBoundary* Scene::FindBoundary(string name)
{
	for (int i = 0; i < SDFBoundaryList.size(); i++)
		if (name == SDFBoundaryList[i]->sdfName) return SDFBoundaryList[i];
}

SDFJet* Scene::FindJet(string name)
{
	for (int i = 0; i < SDFJetList.size(); i++)
		if (name == SDFJetList[i]->sdfName) return SDFJetList[i];
}