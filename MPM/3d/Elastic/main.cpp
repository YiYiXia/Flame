#include <windows.h>
#include <GL/GLU.H>
#include <GL/freeglut.h>
#include<GL/GL.H>
#include"HE_mesh/Mesh3D.h"
#include"HE_mesh/Vec.h"
#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include"Parameters.h"
#include"Grid.h"
#include"ParticleCloud.h"
using namespace std;
using trimesh::vec;

GLfloat						eye_distance_;
point						eye_goal_;
point						eye_pose_;

Vector3f gravity = Vector3f(0, -0.9,0);
Grid* grid;
ParticleCloud* bar;
ParticleCloud* bar2;
vector<Particle> sample(Vector3f start, Vector3f end, Vector3f size,Vector3f v)
{
	Vector3f cellsize;
	cellsize[0] = (end[0] - start[0]) / (size[0] - 1);//格点横向长度
	cellsize[1] = (end[1] - start[1]) / (size[1] - 1);//格点竖直方向长度
	cellsize[2] = (end[2] - start[2]) / (size[2] - 1);
	int length = size[0] * size[1] * size[2];
	vector<Particle> points(length);

	//如果不绘制格点，就不要加这些
	for (int i = 0; i < size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			for (int k = 0; k < size[2]; k++)
			{
				points[(int)(k*size[0] * size[1] + j*size[0] + i)] = Particle(start + Vector3f(cellsize[0] * i, cellsize[1] * j, cellsize[2] * k), v, 1.0);
			}
			
			/*points[(int)(j*size[0] + i)].mass = 1;
			points[(int)(j*size[0] + i)].velocity = Vector2f(0, 0);*/
		}
	}
	return points;
}



void InitMPM()
{

	vector<Particle> points = sample(Vector3f(1.618, 1.618, 1.618), Vector3f(1.784, 1.784,1.784), Vector3f(6, 6, 6), Vector3f(0, 0, 0));
	bar = ParticleCloud::Insert(points);

	grid = new Grid(Vector3f(0, 0, 0), Vector3f(VIEW_WIDTH, VIEW_HEIGHT, VIEW_HEIGHT), Vector3f(100, 100, 100), bar);

	grid->initializeMass();
	
	grid->calculateVolumes();
}



void SetLight()
{
	static GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	static GLfloat mat_shininess[] = { 50.0 };
	static GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	static GLfloat white_light[] = { 0.8, 0.8, 0.8, 1.0 };
	static GLfloat lmodel_ambient[] = { 0.3, 0.3, 0.3, 1.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void InitGL(void)
{
	eye_pose_[0] = 4.0;
	eye_pose_[1] = 4.0;
	eye_pose_[2] = 4.0;
	eye_goal_[0] = 0.0;
	eye_goal_[1] = 0.0;
	eye_goal_[2] = 0.0;
	eye_distance_ = 15.0;

	glClearColor(0.3, 0.3, 0.3, 0.0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DOUBLEBUFFER);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1);
	SetLight();
}



void paintGL()
{
	glShadeModel(GL_SMOOTH);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	SetLight();


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye_pose_[0], eye_pose_[1], eye_pose_[2], eye_goal_[0], eye_goal_[1], eye_goal_[2], 0.0, 1.0, 0.0);




	bar->draw();

	glPopMatrix();
	glutSwapBuffers();

}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'x':

		eye_pose_[0] += 0.1;
		eye_pose_[1] += 0.1;
		eye_pose_[2] += 0.2;

		break;
	case 'c':
		eye_pose_[0] -= 0.1;
		eye_pose_[1] -= 0.1;
		eye_pose_[2] -= 0.2;
		break;
	case 27: //ESC 键盘
		exit(0);
		break;
	default:
		break;
	}
	paintGL();
}

void Update()
{
	for (int i = 0; i < 10; i++)
	{

		grid->initializeMass();
		grid->initializeVelocities();
		grid->explicitVelocities(gravity);
		grid->updateVelocities();
		bar->update();
	}

	glutPostRedisplay();
}



void reshape(int w, int h)
{
	//设置视口参数为整个窗口范围内
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	//设置投影参数:投影矩阵初始为单位阵
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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

int main(int argc, char** argv)
{
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutInit(&argc, argv);  //对GLUT进行初始化，这个函数必须在其它的GLUT使用之前调用一次
	glutCreateWindow("MPMElastic");
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//设置显示方式，其中GLUT_RGB表示使用RGB颜色，与之对应的还有GLUT_INDEX（表示使用索引颜色）。GLUT_SINGLE表示使用单缓冲，与之对应的还有GLUT_DOUBLE（使用双缓冲）
	InitGL();
	InitMPM();
	//glutDisplayFunc(Render);
	glutDisplayFunc(paintGL);
	glutReshapeFunc(reshape);
	//glutKeyboardFunc(keyboard);
	//glutDisplayFunc(Render);
	glutIdleFunc(Update);
	glutMainLoop();
	return 0;
}
