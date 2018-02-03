#pragma once
const static int SOLVER_STEPS = 10;
const static int fps = 40;
static double DT = ((1.0f / fps) / SOLVER_STEPS);//每一步的时长CFL条件
const static int WINDOW_WIDTH = 700;//显示区域
const static int WINDOW_HEIGHT = 700;
const static double VIEW_WIDTH =6.6f;//研究区域，二者要成比例
const static double VIEW_HEIGHT = WINDOW_HEIGHT*VIEW_WIDTH / WINDOW_WIDTH;
const static double lambda = 10000.0f;
const static double mu = 10000.0f;
const static double EPS = 0.0000001f;//最小距离，小于此，认为是零
const static double EPS2 = EPS*EPS;

#define deb true