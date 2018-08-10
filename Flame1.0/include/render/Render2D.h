/*----------- Copyright(c) 2018 YimingXia(491691865@qq.com)-------------------
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
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

//todo:these function below is used to implement the alogrithm of light trace in 2d,they should be reorganized when do rendering
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

/********************************************************
	*Class name:       Render2D
	*Usage:			   Render the object in scene.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/

class Render2D
{
public:
	Render2D(int h, int w);
	~Render2D();
	cv::Mat canvas;
	int width, height;
	SDF *sdf;

/********************************************************
	*Function name:    CanvasClear
	*Usage:            Clean the canvas,make all pixel (255,255,255)
	*Input:            None.
	*Output:           None.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	void CanvasClear();

/********************************************************
	*Function name:    CanvasDraw
	*Usage:            Use the input information,draw them in canvas.
	*Input:            Overload:+3
	                   (1)vector<Vector2d>:draw particles.
					   (2)scene:draw particle cloud,SDF.
					   (3)center,radius,color:draw a solid cirlce.
	*Output:           None.
	*Last Update       2018.08.10 Yiming Xia
***********************************************************/
	void CanvasDraw(vector<Vector2d> pointlist);
	void CanvasDraw(Scene* sc);
	void CanvasDraw(Vector2d center, double r,Vector3d color);


//todo:these function below is used to implement the alogrithm of light trace in 2d,they should be reorganized
	Result SceneDistance(double x, double y);
	Color Trace(double x, double y, double ix, double iy, int depth);
	void PixelCalculate(int value);
};

