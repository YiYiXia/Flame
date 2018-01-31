#include <windows.h>
#include <GL/GLU.H>
#include <GL/freeglut.h>
#include<GL/GL.H>

#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include"Parameters.h"
#include"Grid.h"
#include"ParticleCloud.h"
using namespace std;

int ji;
Vector2f gravity = Vector2f(0, -3.9);
Grid* grid;
ParticleCloud* bar;
ParticleCloud* bar2;
vector<Particle> sample(Vector2f start, Vector2f end, Vector2f size,Vector2f v)
{
	Vector2f cellsize;
	cellsize[0] = (end[0] - start[0]) / (size[0]-1);//格点横向长度
	cellsize[1] = (end[1] - start[1]) / (size[1]-1);//格点竖直方向长度
	int length = size[0] *size[1];
	vector<Particle> points(length);

	//如果不绘制格点，就不要加这些
	for (int i = 0; i < size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			points[(int)(j*size[0] + i)] = Particle(start+Vector2f(cellsize[0] * i, cellsize[1] * j), v, 1, j*size[0] + i);
			/*points[(int)(j*size[0] + i)].mass = 1;
			points[(int)(j*size[0] + i)].velocity = Vector2f(0, 0);*/
		}
	}
	return points;
}




void InitGL(void)
{
	glClearColor(0.9f, 0.9f, 0.9f, 1);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(3);
	glMatrixMode(GL_PROJECTION);
}
void InitMPM()
{

	vector<Particle> points = sample(Vector2f(0.618, 0.318), Vector2f(1.382, 1.182), Vector2f(24, 24), Vector2f(0, 0));
	//vector<Particle> points = sample(Vector2f(1.618, 0.118), Vector2f(2.382, 0.982), Vector2f(24, 24), Vector2f(0, 0));
	bar = ParticleCloud::Insert(points);

	/*points = sample(Vector2f(3.518, 3.418), Vector2f(4.282, 4.282), Vector2f(24, 24), Vector2f(-6, 0));
	bar2 = ParticleCloud::Insert(points);
	bar->merge(*bar2);*/

	grid = new Grid(Vector2f(0,0), Vector2f(VIEW_WIDTH, VIEW_HEIGHT), Vector2f(100,100),bar);

	grid->initializeMass();
	
	grid->calculateVolumes();
	//cout << grid->cellsize[0] << " " << grid->cellsize[1] << endl;
	//grid->output();
	//bar->output();
}

void Update()
{
	ji++;
	for (int i = 0; i < 10; i++)
	{

		/*for (int j = 0; j < bar->particles.size(); j++)
		{
			cout << bar->particles[j].grid_position[0] << " " << bar->particles[j].grid_position[1] << endl;
		}*/
		/*for (int j = 0; j < bar->particles.size(); j++)
		{
			cout << grid->obj->particles[j].grid_position[0] << " " << grid->obj->particles[j].grid_position[1] << endl;
		}*/
		grid->initializeMass();
		grid->initializeVelocities();
		grid->explicitVelocities(gravity);
		grid->updateVelocities();
		bar->update();
		
		//Sleep(2);
	}
	//cout << ji << endl;
	glutPostRedisplay();
}

void Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glOrtho(0, VIEW_WIDTH, 0, VIEW_HEIGHT, 0, 1);

	glBegin(GL_POINTS);
		glColor4f(0.2f, 0.6f, 1.0f, 1);
		//glVertex2f(19.0f, 15.0f);
	glEnd();
	//grid->draw();
	bar->draw();
	glFlush();
}

int main(int argc, char** argv)
{
	//Sleep(5);
	ji = 0;
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutInit(&argc, argv);  //对GLUT进行初始化，这个函数必须在其它的GLUT使用之前调用一次
	glutCreateWindow("MPMElastic");
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//设置显示方式，其中GLUT_RGB表示使用RGB颜色，与之对应的还有GLUT_INDEX（表示使用索引颜色）。GLUT_SINGLE表示使用单缓冲，与之对应的还有GLUT_DOUBLE（使用双缓冲）
	InitGL();
	InitMPM();
	glutDisplayFunc(Render);
	glutIdleFunc(Update);
	glutMainLoop();
	return 0;
}
