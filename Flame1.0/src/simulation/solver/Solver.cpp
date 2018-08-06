#include"../../../include/simulation/solver/Solver.h"

Solver::Solver()
{
	frames = 0;
}

Solver::~Solver()
{
}
void Solver::SceneControl()
{
	SDF temp;
	for (int i = 0; i < scene->SDFlist.size(); i++)
	{
		if (scene->SDFlist[i].sdf->move == true)
		{		
			//cout << frames << endl;
			scene->SDFlist[i].sdf->Control(frames);
			scene->SDFlist[i].sdf->Update(DT);
			scene->SDFlist[i].velocity = temp.Rotate(scene->SDFlist[i].velocity, scene->SDFlist[i].sdf->omega*DT);
			if (scene->SDFlist[i].sdf->sample_check == true)
			{
				scene->SDFlist[i].sdf->sample_time = 0.1 / (scene->SDFlist[i].velocity.norm()*DT);
				scene->SDFlist[i].sdf->sample_check = false;
				//cout << "Sample Time:" << scene->SDFlist[i].sdf->sample_time << endl;
			}
			
			
		}
	}
}