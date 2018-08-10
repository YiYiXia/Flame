#pragma once
#include<opencv2/opencv.hpp>
#include"../common/Parameters.h"
#include"../geometry/SDF.h"
#include"../math/LinkList.h"
#include"../math/Function.h"

PointList OrderSample(SDF* polygon);
PointList PossionDisc(double r, SDF* polygon, int k);
PointList PossionDisc(double r, cv::Mat picture, SDF* polygon, int k);
bool MatContain(cv::Mat picture, double scalex, double scaley, Vector2d p);

