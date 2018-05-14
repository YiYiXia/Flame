#include <windows.h>
#include <GL/GLU.H>
#include <GL/freeglut.h>
#include<GL/GL.H>
#include <fstream>
#include <cstdlib> 
#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include"Parameters.h"
#include"ParticleCloud.h"
using namespace std;


Vector2f gravity = Vector2f(0, -3.9);
ParticleCloud* bar;
ParticleCloud* bar2;
int WINDOW_WIDTH = 700;//显示区域
int WINDOW_HEIGHT = 700;
double VIEW_WIDTH = 5.0f;//研究区域，二者要成比例
double VIEW_HEIGHT = 5.0f;
int sleep_time = 15;



void InitGL(void)
{
	glClearColor(0.9f, 0.9f, 0.9f, 1);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(3);
	glMatrixMode(GL_PROJECTION);
}



void Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glOrtho(0, VIEW_WIDTH, 0, VIEW_HEIGHT, 0, 1);

	glBegin(GL_POINTS);
		glColor4f(0.2f, 0.6f, 1.0f, 1);
	glEnd();
	//grid->draw();
	bar->draw();
	glFlush();
}

int fff = 0;
void test()
{
	bar = new ParticleCloud();
	bar->filein(fff);
	fff++;
	Sleep(sleep_time);
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	//ifstream infile;
	//infile.open("Player2D_Config.txt");
	//if (!infile.is_open())
	//{
	//	cout << "Could Not Open File!!!";
	//	exit(EXIT_FAILURE);
	//}
	//double val;
	//string st;
	//int num;
	//infile >> st;
	//while (infile.good())
	//{
	//	if (st.compare("WINDOW_WIDTH") == 0)
	//	{
	//		infile >> num;
	//		WINDOW_WIDTH = num;
	//		//cout << val << endl;
	//	}
	//	if (st.compare("WINDOW_HEIGHT") == 0)
	//	{
	//		infile >> num;
	//		WINDOW_HEIGHT = num;
	//		//cout << val << endl;
	//	}
	//	if (st.compare("VIEW_WIDTH") == 0)
	//	{
	//		infile >> val;
	//		VIEW_WIDTH = val;
	//		//cout << val << endl;
	//	}
	//	if (st.compare("VIEW_WIDTH") == 0)
	//	{
	//		infile >> val;
	//		VIEW_WIDTH = val;
	//		//cout << val << endl;
	//	}
	//	if (st.compare("sleep_time") == 0)
	//	{
	//		infile >> num;
	//		sleep_time = num;
	//		cout << sleep_time << endl;
	//	}
	//	infile >> st;
	//}
	//if (infile.eof())
	//	cout << "End Of File!!";
	//infile.close();

	
	//bar->filein(0);
	bar = new ParticleCloud();
	bar->filein(fff);
	fff++;
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutInit(&argc, argv);  //对GLUT进行初始化，这个函数必须在其它的GLUT使用之前调用一次
	glutCreateWindow("MPMElastic");
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//设置显示方式，其中GLUT_RGB表示使用RGB颜色，与之对应的还有GLUT_INDEX（表示使用索引颜色）。GLUT_SINGLE表示使用单缓冲，与之对应的还有GLUT_DOUBLE（使用双缓冲）
	InitGL();
	glutDisplayFunc(Render);
	glutIdleFunc(test);
	glutMainLoop();
	return 0;
}
