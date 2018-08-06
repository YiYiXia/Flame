#include"../../../include/simulation/solver/SolverMPM.h"

SolverMPM::SolverMPM()
{
}
SolverMPM::~SolverMPM()
{
}
void SolverMPM::Initial(SolverType SType)
{
	Type = SType;
	bar = new ParticleCloudMPM();
	grid = new GridMPM(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), bar);
	grid->InitializeMass();
	grid->CalculateVolumes();
	//根据场景输入信息分配信息
	for (int i = 0; i < scene->SDFlist.size(); i++)
	{
		if (scene->SDFlist[i].sdf->Live_Info.AppearTime == 0)//出现时间对上
		{
			if (scene->SDFlist[i].stype == Boundary || scene->SDFlist[i].stype == Object || scene->SDFlist[i].stype == Rivet || scene->SDFlist[i].stype == Sink) {
				grid->polygon.push_back(scene->SDFlist[i].sdf);
			}
			else if (scene->SDFlist[i].stype == ParticleBoundary)
			{
				ParticleCloudMPM* temp;
				PointList list;
				do {
					if (scene->SDFlist[i].sdf->Sample_Type == PICTURE_P)
					{
						cv::Mat record = cv::imread(scene->SDFlist[i].sdf->Mat_name);
						list = PossionDisc(r, record, scene->SDFlist[i].sdf, iteratime);
					}
					if (scene->SDFlist[i].sdf->Sample_Type == BOUNDSDF_P)
					{
						list = PossionDisc(r, scene->SDFlist[i].sdf, iteratime);
					}
					if (scene->SDFlist[i].sdf->Sample_Type == BOUNDSDF_O)
					{
						list = OrderSample(scene->SDFlist[i].sdf);
					}
				} while (list.size() < 20);

				temp = new ParticleCloudMPM(list, scene->SDFlist[i].ptype, scene->SDFlist[i].velocity, scene->SDFlist[i].color);
				GridMPM*grid_temp;
				grid_temp = new GridMPM(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), temp);
				grid_temp->InitializeMass();
				grid_temp->CalculateVolumes();
				bar->Merge(*grid_temp->obj);
			}
		}
		
	}
	PointListCheck();
	InsertTime = 0;
}

void SolverMPM::Update()
{
	for (int i = 0; i < SOLVER_STEPS; i++)
	{
		SourceCheck();
		
		grid->InitializeMass();
		grid->InitializeVelocities();
		grid->ExplicitForce();
		grid->GridVelocityUpdate();
		if (Type == IMPLICIT) grid->ImplicitVelocities();
		grid->ApplyExternForce();
		grid->CollisionGrid();
		grid->Friction();
		grid->UpdateVelocities();
		grid->ParticleCheck();
		bar->Update();
		SceneControl();
		PointListCheck();
		frames++;
	}

}

void SolverMPM::PointListCheck()
{
	int psize = scene->pointlist.size();
	int csize = bar->particles.size();
	scene->pointlist.resize(csize);
	for (int i = 0; i < bar->particles.size(); i++)
		scene->pointlist[i] = bar->particles[i];

}

void SolverMPM::SourceCheck()
{
	//if (frames % insert_time == 0 && frames < endtime)
	if (frames < endtime)
	{
		for (int i = 0; i < scene->SDFlist.size(); i++)
		{
			//cout << endtime<<"   sample time " << scene->SDFlist[i].sdf->sample_time << endl;
			if (scene->SDFlist[i].stype == Source&& (frames - InsertTime) == scene->SDFlist[i].sdf->sample_time) {
				
				ParticleCloudMPM* temp;
				PointList list;
				do {
					list = PossionDisc(r, scene->SDFlist[i].sdf, iteratime);
				} while (list.size() < 30);
				/*cout << "Num of particle:" << list.size() << endl;*/
				temp = new ParticleCloudMPM(list, scene->SDFlist[i].ptype, scene->SDFlist[i].velocity , scene->SDFlist[i].color);
				GridMPM*grid_temp;
				grid_temp = new GridMPM(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), temp);
				grid_temp->InitializeMass();
				grid_temp->CalculateVolumes();
				bar->Merge(*grid_temp->obj);
				InsertTime = frames;
				scene->SDFlist[i].sdf->sample_check = true;
				//cout << "yes:"<< bar->particles.size()<< endl;
			}
		}
	}
	for (int i = 0; i < scene->SDFlist.size(); i++)
	{
		if (scene->SDFlist[i].stype == ParticleBoundary&&frames == scene->SDFlist[i].sdf->Live_Info.AppearTime&&frames > 0)//之后插入物体，有三个条件
		{
			ParticleCloudMPM* temp;
			PointList list;
			do {
				if (scene->SDFlist[i].sdf->Sample_Type == PICTURE_P)
				{
					cv::Mat record = cv::imread(scene->SDFlist[i].sdf->Mat_name);
					list = PossionDisc(r, record, scene->SDFlist[i].sdf, iteratime);
				}
				if (scene->SDFlist[i].sdf->Sample_Type == BOUNDSDF_P)
				{
					list = PossionDisc(r, scene->SDFlist[i].sdf, iteratime);
				}
			} while (list.size() < 20);

			temp = new ParticleCloudMPM(list, scene->SDFlist[i].ptype, scene->SDFlist[i].velocity, scene->SDFlist[i].color);
			GridMPM*grid_temp;
			grid_temp = new GridMPM(Vector2d(0, 0), Vector2d(VIEW_WIDTH, VIEW_HEIGHT), temp);
			grid_temp->InitializeMass();
			grid_temp->CalculateVolumes();
			bar->Merge(*grid_temp->obj);
		}
	}
	
}