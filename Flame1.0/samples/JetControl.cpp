#include<iostream>
#include<math.h>
#include<time.h>
#include<conio.h>
#include"../include/common/Parameters.h"
#include"../include/geometry/SDF.h"
#include"../include/geometry/SDFObject.h"
#include"../include/common/File.h"
#include"../include/scene/Scene.h"
#include"../include/render/Render2D.h"
#include"../include/visual/UI2D.h"
#include"../include/simulation/Simulator.h"
using namespace std;

bool key = true;
double height_last = 0;
void JetControl(Scene *sc,Vector2d goal,string name)
{
	double mark_y = 0;
	double mark_x = 0;
	int num_x = 0;
	int num_y = 0;
	for (int i = 0; i < sc->pointlist.size(); i++)
	{
		if (sc->pointlist[i]->position[1] < 0.58&& sc->pointlist[i]->position[1] > 0.3)
		{
			mark_x += sc->pointlist[i]->position[0];
			num_x++;
			sc->pointlist[i]->color = Vector3d(1, 0, 0);
		}
		if (sc->pointlist[i]->position[0] < 1.52&& sc->pointlist[i]->position[0] > 1.38)
		{
			mark_y += sc->pointlist[i]->position[1];
			num_y++;
			sc->pointlist[i]->color = Vector3d(1, 0, 0);
		}
	}
	if (num_x > 0)
	{
		mark_x = mark_x / num_x;
	}

	if (num_y > 0)
	{
		mark_y = mark_y / num_y;
	}
	SDFJet* sdf;
	sdf = sc->FindJet(name);

	if (goal[0] < mark_x)
	{
		sdf->omega = -0.8*(1 - exp(goal[0] - mark_x));
	}
	if (goal[1] < mark_y)
	{
		cout << "charge_y:" << mark_y << endl;
		sdf->omega = 0.4*(1 - exp(goal[1] - mark_y));
		key = true;
		height_last = mark_y;
	}
	else if (mark_y > goal[1] - 0.05)
	{
		cout << "charge_y:" << mark_y << endl;
		sdf->omega = 0;
		key = true;
		height_last = mark_y;
	}
	else if (mark_y > 0.4)
	{
		cout << "charge_y:" << mark_y << endl;
		if (key == true)
		{
			if (height_last <= mark_y - 0.05)
			{
				sdf->omega = -0.4*(1 - exp(mark_y - goal[1] + 0.05));
			}
			else
			{
				sdf->omega = -0.1;
				key = false;
				Vector2d v = sdf->sampleVelocity.normalized();
				sdf->sampleVelocity = sdf->sampleVelocity + v*0.003; 
			}
		}
		else
		{
			Vector2d v = sdf->sampleVelocity.normalized();
			sdf->sampleVelocity = sdf->sampleVelocity + v*0.01;
		}
		height_last = mark_y;
	}
}

int main(int argc, char** argv)
{
	ParametersInput();
	Simulator sim;
	Scene sc;
	Render2D render(WINDOW_HEIGHT, WINDOW_WIDTH);
	UI2D ui;

	SDFJet a(0, WATER, BOUNDSDF_P, Vector2d(-3, 0), 0.1);
	a.sdfName = "jet";
	a.Insert(3.6, 2.85);
	a.Insert(3.7, 2.85);
	a.Insert(3.7, 2.95);
	a.Insert(3.6, 2.95);
	sc.AddObject(&a);

	SDFBoundary c(0, Boundary);
	c.sdfName = "bond";
	c.Insert(1.3, 0.3);
	c.Insert(1.3, 4.7);
	c.Insert(4.9, 4.7);
	c.Insert(4.9, 0.3);
	sc.AddObject(&c);

	SDFBoundary d(0, Sink);
	d.sdfName = "sink";
	d.Insert(1.4, 0.4);
	d.Insert(1.4, 4.6);
	d.Insert(4.8, 4.6);
	d.Insert(4.8, 0.4);
	sc.AddObject(&d);

	sim.SimulatorInitial(&sc, MPM, EXPLICIT);

	int i = 0;
	Vector2d goal(1.4, 2.58);
	while (1)
	{
		sim.SimulatorUpdate();
		render.CanvasDraw(&sc);
		JetControl(&sc, goal, "jet");
		render.CanvasDraw(goal, 6, Vector3d(0, 1.0, 0));
		ui.UIShow(&render.canvas);
		//file.CanvasOutput(i, &render.canvas);
		render.CanvasClear();
		i++;
		//cout << i << endl;
	}

}