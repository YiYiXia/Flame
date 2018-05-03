#pragma once
#include"ParticleCloud.h"
#include"Parameters.h"
#include<opencv2/opencv.hpp>
#include"File.h"
class Render2D
{
public:
	Render2D(int h, int w);
	~Render2D();
	cv::Mat canvas;
	int width, height;
	void CanvasClear();
	void CanvasOutput(int num);
	void CanvasDraw(ParticleCloud *cloud);
	void CanvasShow();
};