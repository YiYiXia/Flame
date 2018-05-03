#include <iostream>
#include"File.h"
#include"Scene.h"
#include"Player2D.h"
#include"Render2D.h"
using namespace std;

int main(int argc, char** argv)
{
	File file;
	file.ParametersIn();
	
	Player2D player;
	Scene sc;
	sc.Initial();
	
	Render2D r(600, 600);

	//player.Initial(argc, argv);
	//player.scence = &sc;
	//


	int i = 0;
	while (1)
	{
		file.FileIn(i, *sc.bar);
		//cout << sc.bar->particles[14].position[1] << endl;

		r.CanvasDraw(sc.bar);
		r.CanvasOutput(i);
		//r.CanvasShow();
		r.CanvasClear();
	//	player.Render();
		i++;
		//sc.Update();
	}

	/*file.Initial();
	int i = 0;
	while (1)
	{
		sc.Update();
		file.FileOut(i, *sc.bar);
		i++;
	}*/



	//show(argc,  argv);
	//Sleep(5);
	//srand((int)time(0));
	//glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(0, 0);
	//glutInit(&argc, argv);  //对GLUT进行初始化，这个函数必须在其它的GLUT使用之前调用一次
	//glutCreateWindow("MPMElastic");
	//glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//设置显示方式，其中GLUT_RGB表示使用RGB颜色，与之对应的还有GLUT_INDEX（表示使用索引颜色）。GLUT_SINGLE表示使用单缓冲，与之对应的还有GLUT_DOUBLE（使用双缓冲）
	//InitGL();
	//InitMPM();
	//glutDisplayFunc(Render);
	//glutIdleFunc(Update);
	//glutMainLoop();
	return 0;
}
