#include<iostream>
#include<math.h>
#include<time.h>
#include"Parameters.h"
#include"SDF.h"
#include"Player2D.h"
#include"ParallelRender.h"
#include"File.h"
#include"Scene.h"
#include"Render2D.h"
#include"MarchingCube.h"
#include"LinkList.h"
using namespace std;
using namespace tbb;


int main(int argc, char** argv)
{
	//SDF a;
	//a.Insert(0.5, 0.5);
	//a.Insert(1.0, 0.5);
	//a.Insert(1.0, 1.0);
	//a.Insert(0.5, 1.0);
	//cout << a.Gradient(0.0,0.0) << endl;





	File file;
	file.ParametersIn();
	
	//Player2D player;
	Scene sc(150);
	sc.Initial();
	//player.Initial(argc, argv);
	//player.scene = &sc;
	Render2D render(700,700);
	
	SDF a;
	a.Insert(2.050, 3.198);
	a.Insert(2.950, 3.198);
	a.Insert(2.950, 4.148);
	a.Insert(2.050, 4.148);

	sc.AddObject(WATER, &a);

	//Vector2d v1(5, 0);
	
	/*SDF a;
	a.Insert(0.25, 0.25);
	a.Insert(1.95, 0.25);
	a.Insert(1.95, 1.55);
	a.Insert(0.25, 1.55);
	sc.AddObject(SAND, &a);*/

	//Vector2d v2(-5, 0);
	//SDF b;
	//b.Insert(3.3, 2.6);
	//b.Insert(4.3, 2.6);
	//b.Insert(4.3, 3.6);
	//b.Insert(3.3, 3.6);
	//sc.AddObject(ELASTIC, &b, v2);
	
	SDF c;
	c.Insert(0.2, 0.2);
	c.Insert(0.2, 4.8);
	c.Insert(4.8, 4.8);
	c.Insert(4.8, 0.2);
	sc.AddPolygon(Boundary, &c);
	
	/*SDF glass;
	glass.Insert(1.15, 0.75);
	glass.Insert(2.38, 2.38);
	glass.Insert(2.38, 2.62);
	glass.Insert(1.15, 4.25);
	glass.Insert(3.85, 4.25);
	glass.Insert(2.62, 2.62);
	glass.Insert(2.62, 2.38);
	glass.Insert(3.85, 0.75);
	sc.AddPolygon(Boundary, &glass);
	glass.Initialize();
	glass.omega = 0;*/

	/*SDF d;
	d.Insert(0.35, 1.75);
	d.Insert(2.25, 1.75);
	d.Insert(2.25, 2.05);
	d.Insert(0.35, 2.05);
	d.Initialize();
	d.omega = M_PI / (1500 * DT);
	sc.AddPolygon(Object, &d);*/
	SDF e;
	e.Insert(2, 1.5);
	e.Insert(2.5, 2);
	e.Insert(3, 1.5);
	e.Insert(2.5, 1); 
	sc.AddPolygon(Object, &e);
	//SDF e;
	//e.Insert(2.418, 3.918);
	//e.Insert(2.518, 3.918);
	//e.Insert(2.518, 4.118);
	//e.Insert(2.418, 4.118);
	//sc.AddSource(Source, &e,SAND,Vector2d(1,0));
	//sc.source = true;

	//glutDisplayFunc(player.Render);
	//glutIdleFunc(sc.Update);
	//file.Initial();
	int i = 0;
	while (1)
	{
		//player.Render();
		sc.Update();
		render.CanvasDraw(&sc);
		render.CanvasOutput(i);
		render.CanvasClear();
		//file.FileOut(i, *sc.bar);
		//cout << i << endl;
		i++;
		//Sleep(5);
	}
	














	//文件输出
	/*file.Initial();
	int i = 0;
	while (1)
	{
		sc.Update();
		file.FileOut(i, *sc.bar);
		i++;
	}*/

	//clock_t start, end;
	//int x = 400, y = 400; 

	//Render2D render(x, y);
	//
	//File file;
	//file.ParametersIn();
	//vector<Vector2d> p;
	//file.FileIn(237, p);
	//render.CanvasDraw(p);
	//MarchingCube mc;
	//mc.Polygon(render.canvas);
	//Player2D player;
	////窗口展示
	//player.Initial(argc, argv);
	//while (1)
	//{
	//	player.DrawSDF(mc.polygons[0]);
	//	//player.DrawSurface();
	//}


	//SDF sdf(x,y);
	//sdf.Insert(100, 100);
	//sdf.Insert(200, 100);
	//sdf.Insert(200, 200);
	//sdf.Insert(100, 200);*/
	////sdf.Insert(80, 150);
	//sdf.Insert(1, 1);
	//sdf.Insert(2, 1);
	//sdf.Insert(2, 2);
	//sdf.Insert(1, 2);*/
	////sdf.GirdInitial();
	//
	////cout << sdf.GridDistance(99, 99.6) << endl;
	////cout << sdf.GridDistance(0, 0.6) << endl;
	//mc.polygons[0].GirdInitial();
	//render.sdf = &mc.polygons[0];
	//
	//
	////render.sdf = &sdf;
	//

	//start = clock();
	//const int n = x*y;
	//int* a;
	//a = (int *)malloc(n * sizeof(int));
	//for (int i = 0; i < n; i++)
	//{
	//	a[i] = i;
	//	//render.PixelCalculate(i);
	//}
	//
	//
	//parallel_for(blocked_range<size_t>(0, n), ParallelRender(a, &render), simple_partitioner());
	//end = clock();
	//cout << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
	//render.CanvasShow();


	//绘图
		//File file;
		//file.ParametersIn();
		//
		//Player2D player;
		//Scene sc;
		//sc.Initial();
		//
		//
		////窗口展示
		//player.Initial(argc, argv);
		////player.scene = &sc;
		//////
		////while (1)
		////{
		////	player.Render();
		////	sc.Update();
		////}
	
	
		////绘制边界
		//Render2D r(500, 500);
		//vector<Vector2d> p;
		//file.FileIn(237, p);
		////file.FileIn(i, *sc.bar);
		////cout << sc.bar->particles[14].position[1] << endl;
	
	
		//r.CanvasDraw(p);
	
		//player.render = &r;
		//while (1)
		//{
		//	player.DrawSDF();
		//	//player.DrawSurface();
		//}


}




//#include <iostream>
//#include"File.h"
//#include"Scene.h"
//#include"Player2D.h"
//#include"Render2D.h"
//#include"SDF.h"
//#include<tbb/task_scheduler_init.h>
//#include<tbb/blocked_range.h>
//#include<tbb/parallel_for.h>
//
//using namespace std;
//using namespace tbb;
//int main(int argc, char** argv)
//{
//
//	/*SDF sdf;
//	sdf.Insert(100, 100);
//	sdf.Insert(200, 100);
//	sdf.Insert(200, 200);
//	sdf.Insert(100, 200);
//	sdf.Insert(80, 150);
//	double max = 0;
//	Render2D render(400, 400);
//	render.sdf = &sdf;
//	const int n = 160000;
//	int a[n];
//	for (int i = 0; i < n; i++)
//	{
//		a[i] = i;
//	}
//	parallel_for(blocked_range<size_t>(0, n), ParallelRender(a, &render), simple_partitioner());
//	render.CanvasShow();*/
//
//	File file;
//	file.ParametersIn();
//	
//	Player2D player;
//	Scene sc;
//	sc.Initial();
//	
//	
//	//窗口展示
//	player.Initial(argc, argv);
//	//player.scene = &sc;
//	////
//	//while (1)
//	//{
//	//	player.Render();
//	//	sc.Update();
//	//}
//
//
//	//绘制边界
//	Render2D r(500, 500);
//	vector<Vector2d> p;
//	file.FileIn(237, p);
//	//file.FileIn(i, *sc.bar);
//	//cout << sc.bar->particles[14].position[1] << endl;
//
//
//	r.CanvasDraw(p);
//
//	player.render = &r;
//	while (1)
//	{
//		player.DrawSDF();
//		//player.DrawSurface();
//	}
//	//r.CanvasOutput(237);
//
//	//int i = 0;
//	//while (1)
//	//{
//	//	vector<Vector2d> p;
//	//	file.FileIn(i, p);
//	//	//file.FileIn(i, *sc.bar);
//	//	//cout << sc.bar->particles[14].position[1] << endl;
//
//	//	r.CanvasDraw(p);
//	//	r.CanvasOutput(i);
//	//	    //r.CanvasShow();
//	//	r.CanvasClear();
//	//	p.clear();
//	////	player.Render();
//	//	i++;
//	//	//sc.Update();
//	//}
//
//
//	//文件输出
//	//file.Initial();
//	//int i = 0;
//	//while (1)
//	//{
//	//	sc.Update();
//	//	file.FileOut(i, *sc.bar);
//	//	i++;
//	//}
//
//
//
//	//show(argc,  argv);
//	//Sleep(5);
//	//srand((int)time(0));
//	//glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
//	//glutInitWindowPosition(0, 0);
//	//glutInit(&argc, argv);  //对GLUT进行初始化，这个函数必须在其它的GLUT使用之前调用一次
//	//glutCreateWindow("MPMElastic");
//	//glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//设置显示方式，其中GLUT_RGB表示使用RGB颜色，与之对应的还有GLUT_INDEX（表示使用索引颜色）。GLUT_SINGLE表示使用单缓冲，与之对应的还有GLUT_DOUBLE（使用双缓冲）
//	//InitGL();
//	//InitMPM();
//	//glutDisplayFunc(Render);
//	//glutIdleFunc(Update);
//	//glutMainLoop();
//	return 0;
//}
