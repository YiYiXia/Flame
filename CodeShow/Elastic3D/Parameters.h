/*
��ͷ�ļ����ڲ������
*/
#pragma once
const static int SOLVER_STEPS = 10;
const static int fps = 40;
static double DT = ((1.0f / fps) / SOLVER_STEPS);//ÿһ����ʱ��CFL����
const static int WINDOW_WIDTH = 700;//��ʾ����
const static int WINDOW_HEIGHT = 700;
const static double VIEW_WIDTH =5.0f;//�о����򣬶���Ҫ�ɱ���
const static double VIEW_HEIGHT = WINDOW_HEIGHT*VIEW_WIDTH / WINDOW_WIDTH;
const static double lambda = 2000.0f;
const static double mu = 2000.0f;
const static double EPS = 0.0000001f;//��С���룬С�ڴˣ���Ϊ����
const static double EPS2 = EPS*EPS;

#define deb true