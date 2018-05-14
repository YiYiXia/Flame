#pragma once
const static int SOLVER_STEPS = 10;
const static int fps = 40;
static double DT = ((1.0f / fps) / SOLVER_STEPS);//每一步的时长CFL条件

const static double lambda = 1000.0f;
//const static double mu = 8000.0f;
const static double EPS = 0.0000001f;//最小距离，小于此，认为零
const static double EPS2 = EPS*EPS;
const static double FLIP_PERCENT = .95;//FLIP在此处效果似乎效果不是很好
const static double HARDERN = 11000;
const static double GAMA = 7;
const static double STICKY = 0.2;
const static double throld = 0.6;
#define deb true