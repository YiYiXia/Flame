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
#include <Eigen/Dense>
#define M_PI 3.14159265358979

extern int SOLVER_STEPS;                //Simulation parameter:the simulate time per frame
const static int fps = 40;              //Simulation parameter:frames per second
extern Eigen::Vector2d gravity;         //Simulation parameter:Gravity
extern double DT;                       //Simulation parameter:dt for per frames
extern double VIEW_WIDTH;               //Simulation parameter:Sizex of simulation region 
extern double VIEW_HEIGHT;              //Simulation parameter:Sizey of simulation region 
extern int WINDOW_WIDTH;                //UI parameter:Sizex of show image
extern int WINDOW_HEIGHT;               //UI parameter:Sizey of show image
extern double  r;                       //Possion disk parameter:sample radius
extern int iteratime;                   //Possion disk parameter:sample iteration times
extern int endtime;                     //Source parameter:the time the source stop

void ParametersInput();                 //Input parameters from config filess