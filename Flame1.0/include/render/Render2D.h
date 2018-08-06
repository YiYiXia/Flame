#pragma once

#include<opencv2/opencv.hpp>
#include<time.h>
#include"../scene/Scene.h"
#include"../geometry/SDF.h"
#include"../physics/ParticleCloud.h"
#include"../common/Parameters.h"

#define MAX_ITERATION 64
#define MAX_DISTANCE  800.0f
#define EPSILON 0.001f
#define N 156.0f
#define MAX_DEPTH 3
#define BIAS 0.01f
#define BLACK {0.0f,0.0f,0.0f}

typedef struct { double r, g, b; } Color;

typedef struct {
	double d, reflectivity, eta;
	Color emissive, absorption;
} Result;

Color ColorAdd(Color a, Color b);
Color ColorMultiply(Color a, Color b);
Color ColorScale(Color a, double s);
void Reflect(double ix, double iy, double nx, double ny, double *rx, double *ry);//矢量反射
int Refract(double ix, double iy, double nx, double ny, double eta, double *rx, double *ry);
Color BeerLambert(Color a, double d);
Result UnionOp(Result a, Result b);//并
Result IntersectOp(Result a, Result b);//交
Result SubtractOp(Result a, Result b);//差

class Render2D
{
public:
	Render2D(int h, int w);
	~Render2D();
	cv::Mat canvas;
	int width, height;
	SDF *sdf;
	void CanvasClear();//清空画布
	void CanvasDraw(vector<Vector2d> pointlist);
	void CanvasDraw(Scene* sc);
	void CanvasDraw(Vector2d center, double r,Vector3d color);
	Result SceneDistance(double x, double y);
	Color Trace(double x, double y, double ix, double iy, int depth);
	void PixelCalculate(int value);
};

