
#include"Scene.h"

Scene::Scene(int resolution)
{
	frames = 0;
	source = false;
	Resolution = resolution;
	startframes = -1;
	endframes = -1;
}

Scene::~Scene()
{
}

void Scene::Initial()
{
	
	//double dx = 150.0f*0.018f / (double)resulotion;
	double dx = 0.025;
	end_x = start_x + dx*size_x;
	end_y = start_y + dx*size_y;
	bar = new ParticleCloud();
	//bar->InitialSample();
	grid = new Grid(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), Vector2d(Resolution, Resolution), bar);
	grid->initializeMass();
	grid->calculateVolumes();
}


void Scene::Update()
{
	for (int i = 0; i < SOLVER_STEPS; i++)
	{
		if (source)
		{
			if (frames % insert_time == 0 && frames < endtime)
			{

				
				for (int i = 0; i < grid->polygon.size(); i++)
				{
					if (grid->polygon[i]->type == Source)
					{
						map<SDF*, ParticleType>::iterator it;
						it = SPaType.find(grid->polygon[i]);
						AddObject(it->second, grid->polygon[i], grid->polygon[i]->v_center);
						
		/*				ParticleCloud* bar2;
						bar2 = new ParticleCloud();
						bar2->InitialSample();
						Grid*grid_temp;
						grid_temp = new Grid(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), Vector2d(Resolution, Resolution), bar2);
						grid_temp->initializeMass();
						grid_temp->calculateVolumes();
						bar->merge(*grid_temp->obj);*/
					}
				}

			}
		}
		
		
		grid->initializeMass();
		grid->initializeVelocities();
		grid->explicitVelocities(gravity);
		grid->updateVelocities();
		bar->update();
		//原版
		for (int i = 0; i < grid->polygon.size(); i++)
		{
			if (grid->polygon[i]->type == Object)	grid->polygon[i]->Update(DT);
		}

		//SDF运动脚本，这个要想办法拿出来
		//if (endframes < frames)
		//{
		//	key = true;
		//	double high = -1, max = -1;
		//	for (int i = 0; i < bar->particles.size(); i++)
		//	{
		//		high = (high > bar->particles[i]->position[1]) ? high : bar->particles[i]->position[1];
		//		max = (max > bar->particles[i]->velocity.norm()) ? max : bar->particles[i]->velocity.norm();
		//		if (bar->particles[i]->position[1] > 2 || bar->particles[i]->velocity.norm()>0.4)
		//		{
		//			key = false;
		//		}
		//	}
		//	if (key == true)
		//	{
		//		startframes = frames;
		//		endframes = frames + 1000;
		//	}

		//	//cout << high << " " << max << endl;
		//}

		//
		//for (int i = 0; i < grid->polygon.size(); i++)
		//{
		//	if (grid->polygon[i]->type == Object)	grid->polygon[i]->Update(DT);
		//	if (endframes > frames)
		//	{
		//		if (grid->polygon[i]->type == Boundary) 
		//		{
		//			grid->polygon[i]->omega= M_PI / (1000 * DT);
		//			grid->polygon[i]->Update(DT);
		//		}
		//	}
		//	else if(endframes > (frames-450*SOLVER_STEPS))
		//	{
		//		if (grid->polygon[i]->type == Boundary)
		//		{
		//			grid->polygon[i]->omega = 0;
		//			grid->polygon[i]->Update(DT);
		//		}
		//		
		//	}
		//	else
		//	{
		//		startframes = frames;
		//		endframes = frames + 1000;
		//	}
		//	
		//}
		frames++;
	}
	
}

void Scene::Draw()
{
	bar->draw();
	for (int i = 0; i < grid->polygon.size(); i++)
	{
		grid->polygon[i]->Draw();
	}
}


void Scene::AddObject(ParticleType Type, SDF* polygon, Vector2d velocity)
{
	ParticleCloud* bar2;
	bar2 = new ParticleCloud();
	//bar2->InitialSample();
	bar2->AddParticles(Type, polygon, velocity);
	Grid*grid_temp;
	grid_temp = new Grid(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), Vector2d(Resolution, Resolution), bar2);
	grid_temp->initializeMass();
	grid_temp->calculateVolumes();
	bar->merge(*grid_temp->obj);
}

void Scene::AddPolygon(SDFType Type, SDF* polygon)
{
	polygon->type = Type;
	grid->polygon.push_back(polygon);
	
}
void Scene::AddSource(SDFType Type, SDF* polygon, ParticleType PType, Vector2d velocity)
{
	polygon->type = Type;
	polygon->v_center = velocity;
	grid->polygon.push_back(polygon);
	SPaType.insert(make_pair(polygon, PType));
}