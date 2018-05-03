#include"Player2D.h"

Player2D::Player2D()
{

}
Player2D::~Player2D()
{

}
void Player2D::Initial(int argc, char** argv)
{
	srand((int)time(0));
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutInit(&argc, argv);  //对GLUT进行初始化，这个函数必须在其它的GLUT使用之前调用一次
	glutCreateWindow("MPMElastic");
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//设置显示方式，其中GLUT_RGB表示使用RGB颜色，与之对应的还有GLUT_INDEX（表示使用索引颜色）。GLUT_SINGLE表示使用单缓冲，与之对应的还有GLUT_DOUBLE（使用双缓冲）
	glClearColor(0.9f, 0.9f, 0.9f, 1);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(3);
	glMatrixMode(GL_PROJECTION);
}

void Player2D::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glOrtho(0, VIEW_WIDTH, 0, VIEW_HEIGHT, 0, 1);


	//grid->draw();
	scene->Draw();
	glFlush();
}

void Player2D::Update()
{
	scene->Update();
	glutPostRedisplay();
}

//void Player2D::Show(int argc, char** argv)
//{
//
//	
//	Initial();
//	while (1)
//	{
//		Render();
//		scence->Update();
//	}
//	//glutDisplayFunc(&Render);
//	//glutIdleFunc(Update);
//	//glutIdleFunc(&(*scence).Update);
//	//glutMainLoop();
//}