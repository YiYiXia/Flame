#include<iostream>
#include<math.h>
#include<time.h>
#include"../include/common/Parameters.h"
#include"../include/geometry/SDFObject.h"
#include"../include/common/File.h"
#include"../include/scene/Scene.h"
#include"../include/render/Render2D.h"
#include"../include/visual/UI2D.h"
#include"../include/simulation/Simulator.h"
using namespace std;

int main()
{
	//参数读入
	ParametersInput();

	Scene sc;

	//添加边界
	SDFBoundary c(0, Boundary);
	c.sdfName = "bond";
	c.Insert(0.3, 0.3);
	c.Insert(0.3, 4.7);
	c.Insert(4.7, 4.7);
	c.Insert(4.7, 0.3);
	sc.AddObject(&c);

	//添加采样边界下面几种都可以
	SDFSampleBound d(0, WATER, BOUNDSDF_P);
	d.sdfName = "particle water";
	d.Insert(2.3, 0.5);
	d.Insert(2.3, 1.5);
	d.Insert(3.3, 1.5);
	d.Insert(3.3, 0.5);
	sc.AddObject(&d);

	SDFSampleBound e(0, WETSAND, BOUNDSDF_P);
	e.sdfName = "particle sand";
	e.Insert(3.5, 0.5);
	e.Insert(3.5, 1.5);
	e.Insert(4.3, 1.5);
	e.Insert(4.3, 0.5);
	sc.AddObject(&e);

	//模拟器初始化
	Simulator sim;
	sim.SimulatorInitial(&sc, MPMMulti, EXPLICIT);

	int i = 0;
	//渲染器初始化，UI显示
	Render2D render(WINDOW_HEIGHT, WINDOW_WIDTH);
	UI2D ui;
	while (1)
	{
		sim.SimulatorUpdate();
		render.CanvasDraw(&sc);
		ui.UIShow(&render.canvas);
		render.CanvasClear();
		i++;
		cout << i << endl;
	}
}