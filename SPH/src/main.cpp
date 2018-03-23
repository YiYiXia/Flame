#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include "Simulator.h"
#include "Parameters.h"

using namespace std;

Simulator sim;

void InitGL()
{
	glClearColor(1.0, 1.0, 1.0, 0.9);
	glColor3f(0.0f, 0.0f, 1.0f);
	glPointSize(5.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 400.0, 0.0, 300.0);
}

void InitSPH()
{
	sim.init(DELTA_T, RESOLUTION, DENSITY, STIFFNESS, GAMMA, FRAME_RATE, Vector2d(SCALE/2.0 + 2.0/RESOLUTION, SCALE/2.0 + 2.0/RESOLUTION), SCALE);
}

void Update()
{
	sim.oneTimeStep();
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(500, 300);
	glutCreateWindow("SPH simulation");
	InitGL();
	InitSPH();
	glutDisplayFunc(Update);
	glutMainLoop();
	return 0;
}