#pragma once
#define debs false
const static int SOLVER_STEPS = 30;
const static int fps = 40;
static double DT = ((1.0f / fps) / SOLVER_STEPS);//每一步的时长CFL条件
const static int WINDOW_WIDTH = 700;//显示区域
const static int WINDOW_HEIGHT = 700;
const static double VIEW_WIDTH = 5.0f;//研究区域，二者要成比例
const static double VIEW_HEIGHT = WINDOW_HEIGHT*VIEW_WIDTH / WINDOW_WIDTH;
const static double lambda = 5000.0f; 
const static double mu = 5000.0f;
const static double EPS = 0.0000001f;//最小距离，小于此，认为是零
const static double EPS2 = EPS*EPS;
const static double FLIP_PERCENT = .95;//FLIP在此处效果似乎效果不是很好
const static double CRIT_COMPRESS = 1 - 3.9e-2;
const static double CRIT_STRETCH = 1 + 7.5e-3;
const static double HARDENING = 4.0;
const static double STICKY = 0.6;
extern int ji;
