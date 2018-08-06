#include<iostream>
#include<math.h>
#include<time.h>
#include<conio.h>
#include"../Flame1.0/include/common/Parameters.h"
#include"../Flame1.0/include/geometry/SDF.h"
#include"../Flame1.0/include/common/File.h"
#include"../Flame1.0/include/scene/Scene.h"
#include"../Flame1.0/include/render/Render2D.h"
#include"../Flame1.0/include/visual/UI2D.h"
#include"../Flame1.0/include/simulation/solver/SolverMPM.h"
using namespace std;

//int main(int argc, char** argv)
//{
//	File file;
//	file.ParametersIn();
//	Scene sc(150);
//	Render2D render(WINDOW_HEIGHT, WINDOW_WIDTH, SHOW);
//	Solver* solve;
//	solve = new MPMMultiSolver();
//	SDF a(0);
//	a.Insert(4.7, 1.85);
//	a.Insert(4.8, 1.85);
//	a.Insert(4.8, 1.95);
//	a.Insert(4.7, 1.95);
//	sc.AddObject(&a, Source, Vector2d(-2.0, 0), WATER);
//	SDF b(0);
//	b.Insert(2.25, 0.70);
//	b.Insert(3.05, 0.70);
//	b.Insert(3.05, 1.70);
//	b.Insert(2.25, 1.70);
//	sc.AddObject(&b, ParticleBoundary, Vector2d(0, 0), WETSAND);
//	SDF c(0);
//	c.Insert(0.2, 0.2);
//	c.Insert(0.2, 2.4);
//	c.Insert(4.8, 2.4);
//	c.Insert(4.8, 0.2);
//	sc.AddObject(&c, Boundary);
//	SDF d(0);
//	d.Insert(4.3, 0.25);
//	d.Insert(4.3, 1.5);
//	d.Insert(4.1, 1.5);
//	d.Insert(4.1, 0.65);
//	d.Insert(2.2, 0.65);
//	d.Insert(1.8, 0.25);
//	sc.AddObject(&d, Object);
//	solve->scene = &sc;
//	solve->Initial(EXPLICIT);
//
//	int i = 0;
//	while (1)
//	{
//		solve->Update();
//		render.CanvasDraw(&sc);
//		//render.CanvasOutput(i);
//		render.CanvasShow();
//		render.CanvasClear();
//		i++;
//	}
//
//}



int main(int argc, char** argv)
{

	
	//file.ParametersIn();
	ParametersInput();
	Scene sc;
	Render2D render(WINDOW_HEIGHT, WINDOW_WIDTH);
	Solver* solve;
	solve = new SolverMPM();
	UI2D ui;

	SDF a(0);
	a.Insert(3.6, 2.85);
	a.Insert(3.7, 2.85);
	a.Insert(3.7, 2.95);
	a.Insert(3.6, 2.95);
	a.Initialize();
	a.AddControl(SDFMoveInfo(10, 600, Vector2d(0, 0), 0.0));
	sc.AddObject(&a, Source, Vector2d(-2.0, 0), WATER);

	//SDF a(0);
	//a.Insert(2.8, 0.49);
	//a.Insert(2.88, 0.49);
	//a.Insert(2.88, 0.57);
	//a.Insert(2.8, 0.57);
	//sc.AddObject(&a, Source, Vector2d(2.0, 2.865), WATER);

	SDF c(0);
	c.Insert(1.3, 0.3);
	c.Insert(1.3, 4.7);
	c.Insert(4.9, 4.7);
	c.Insert(4.9, 0.3);
	sc.AddObject(&c, Boundary);

	SDF d(0);
	d.Insert(1.4, 0.4);
	d.Insert(1.4, 4.6);
	d.Insert(4.8, 4.6);
	d.Insert(4.8, 0.4);
	sc.AddObject(&d, Sink);

	solve->scene = &sc;
	solve->Initial(EXPLICIT);
	//File file;
	//file.CreateFolder();


	//cv::waitKey(0);
	int i = 0;
	Vector2d goal(1.4, 2.58);
	bool key = true;
	double height_last = 0;
	while (1)
	{
		solve->Update();
		render.CanvasDraw(&sc);

		double mark_y = 0;
		double mark_x = 0;
		int num_x = 0;
		int num_y = 0;
		for (int i = 0; i < sc.pointlist.size(); i++)
		{
			//cout << sc.pointlist[i]->velocity[0] << "  " << sc.pointlist[i]->velocity[1] << " " << sc.pointlist[i]->position[0] << "  " << sc.pointlist[i]->position[1] << endl;
			if (sc.pointlist[i]->position[1] < 0.58&&sc.pointlist[i]->position[1] > 0.3)
			{
				mark_x += sc.pointlist[i]->position[0];
				num_x++;
				sc.pointlist[i]->color = Vector3d(1, 0, 0);
			}
			if (sc.pointlist[i]->position[0] < 1.52&&sc.pointlist[i]->position[0] > 1.38)
			{
				mark_y += sc.pointlist[i]->position[1];
				num_y++;
				sc.pointlist[i]->color = Vector3d(1, 0, 0);
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

		if (goal[0] < mark_x)
		{
			a.omega = -0.8*(1 - exp(goal[0] - mark_x));

			//a.omega = -0.8*(-goal[0] + mark_x);
			//cout << "charge_x:" << mark_x<<"    omega:"<< a.omega << endl;
		}


		if (goal[1] < mark_y)
		{
			cout << "charge_y:" << mark_y << endl;
			a.omega = 0.4*(1 - exp(goal[1] - mark_y));
			//a.omega = 0.6*(-goal[1] + mark_y);
			key = true;
			height_last = mark_y;
		}
		else if (mark_y > goal[1] - 0.05)
		{
			cout << "charge_y:" << mark_y << endl;
			a.omega = 0;
			key = true;
			height_last = mark_y;
		}
		else if (mark_y > 0.4)
		{
			cout << "charge_y:" << mark_y << endl;
			//if (height_last <= mark_y - 0.05)
			//{
			//	a.omega = -0.4*(1 - exp(mark_y - goal[1] + 0.05));
			//}
			//else
			//{
			//	a.omega = 0;
			//	Vector2d v = sc.SDFlist[0].velocity.normalized();
			//	sc.SDFlist[0].velocity = sc.SDFlist[0].velocity + v*0.03;
			//}
			if (key == true)
			{
				if (height_last <= mark_y - 0.05)
				{
					a.omega = -0.4*(1 - exp(mark_y - goal[1] + 0.05));
				}
				else
				{
					a.omega = -0.1;
					key = false;
					Vector2d v = sc.SDFlist[0].velocity.normalized();
					sc.SDFlist[0].velocity = sc.SDFlist[0].velocity + v*0.003;
				}
			}
			else
			{
				Vector2d v = sc.SDFlist[0].velocity.normalized();
				sc.SDFlist[0].velocity = sc.SDFlist[0].velocity + v*0.01;
			}
			height_last = mark_y;

			//a.omega = -0.6*(- (mark_y - goal[1] + 0.05));
		}
		/*	if (0.86 < mark_y&&mark_y < 0.9)
		{
		cout << "charge_y:" << mark_y << endl;
		a.omega = 0;
		}
		if (0.86 > mark_y&&mark_y > 0.7)
		{
		cout << "charge_y:" << mark_y << endl;
		a.omega = -0.4*(1 - exp(mark_y - 0.86));
		}*/

		render.CanvasDraw(goal, 6, Vector3d(0, 1.0, 0));

		//render.CanvasOutput(i);
		//render.CanvasShow();
		ui.Show(&render.canvas);
		//file.CanvasOutput(i, &render.canvas);
		render.CanvasClear();
		//sc.camera.look_at += Vector2d(0,-0.08);
		i++;
		cout << i << endl;
	}

}

//int main(int argc, char** argv)
//{
//	File file;
//	file.ParametersIn();
//	Scene sc(150);
//	Render2D render(WINDOW_HEIGHT, WINDOW_WIDTH, SHOW);
//	Solver* solve;
//	solve = new MPMMultiSolver();
//	SDF a(0);
//	a.Insert(4.7, 1.75);
//	a.Insert(4.8, 1.75);
//	a.Insert(4.8, 1.90);
//	a.Insert(4.7, 1.90);
//	sc.AddObject(&a, Source, Vector2d(-2.0, 0), WATER);
//	SDF b(0);
//	b.Insert(2.25, 0.70);
//	b.Insert(3.05, 0.70);
//	b.Insert(3.05, 1.70);
//	b.Insert(2.25, 1.70);
//	sc.AddObject(&b, ParticleBoundary, Vector2d(0, 0), WETSAND);
//	SDF c(0);
//	c.Insert(0.2, 0.2);
//	c.Insert(0.2, 2.4);
//	c.Insert(4.8, 2.4);
//	c.Insert(4.8, 0.2);
//	sc.AddObject(&c, Boundary);
//	SDF d(0);
//	d.Insert(4.3, 0.25);
//	d.Insert(4.3, 1.5);
//	d.Insert(4.1, 1.5);
//	d.Insert(4.1, 0.65);
//	d.Insert(2.2, 0.65);
//	d.Insert(1.8, 0.25);
//	sc.AddObject(&d, Object);
//	solve->scene = &sc;
//	solve->Initial(EXPLICIT);
//
//	int i = 0;
//	while (1)
//	{
//		solve->Update();
//		render.CanvasDraw(&sc);
//		//render.CanvasOutput(i);
//		render.CanvasShow();
//		render.CanvasClear();
//		//sc.camera.look_at += Vector2d(0,-0.08);
//		i++;
//	}
//
//}


//#include<iostream>
//#include<math.h>
//#include<time.h>
//#include"Parameters.h"
//#include"SDF.h"
//#include"Player2D.h"
//#include"ParallelRender.h"
//#include"File.h"
//#include"Scene.h"
//#include"Render2D.h"
//#include"MarchingCube.h"
//#include"LinkList.h"
//#include"MPMSolver.h"
//#include"SPHSolverPCI.h"
//#include"BoxSolver.h"  
//#include"MPMMultiSolver.h"
//using namespace std;
//int main(int argc, char** argv)
//{
//
//
//	File file;
//	file.ParametersIn();
//	Scene sc(150);
//	Render2D render(WINDOW_HEIGHT, WINDOW_WIDTH, SHOW);
//	Solver* solve;
//	solve = new SPHSolverPCI();
//	//solve = new BoxSolver();
//	SDF a(0);
//	a.Insert(6.0, 1.5);
//	a.Insert(1.5, 1.5);
//	a.Insert(1.5, 5.5);
//	a.Insert(6.0, 5.5);
//	sc.AddObject(&a, ParticleBoundary);
//	SDF c(0);
//	c.Insert(0.8, 0.8);
//	c.Insert(0.8, 9.2);
//	c.Insert(9.2, 9.2);
//	c.Insert(9.2, 0.8);
//	sc.AddObject(&c, Boundary);
//	solve->scene = &sc;
//	solve->Initial(EXPLICIT);
//
//	int i = 0;
//	while (1)
//	{
//		solve->Update();
//		render.CanvasDraw(&sc);
//		//render.CanvasOutput(i);
//		render.CanvasShow();
//		render.CanvasClear();
//		i++;
//	}
//
//}









