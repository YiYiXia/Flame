#pragma once
#include<time.h>
#include<iostream>
#include<Eigen/Dense>
#include<algorithm>

using namespace Eigen;
double RandomNumber(double lo, double hi);
double BSpline(double x);
double BSplineSlope(double x);
double LinearKernel(Vector2d d,double cell);
double FractionInside(double phi_left, double phi_right);
double Clamp(double a, double s, double e);
double Lerp(Vector2d p, double *a,int width);
double LerpLine(double s,double e,double w);
Vector2d GradBiLerp(double v00, double v10, double v01, double v11, double fx, double fy);
Vector2d AffineLerp(Vector2d p, double *a, int width);
double SmoothKernel(double r, double h);
