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
#include<iostream>
#include<fstream>  
#include<sstream>
#include<string.h>
#include"../../include/common/Parameters.h"

using namespace std;

double DT;
double  r;
int iteratime, endtime;
int SOLVER_STEPS;
Eigen::Vector2d gravity = Eigen::Vector2d(0, 0);
double VIEW_HEIGHT;
int WINDOW_WIDTH;
int WINDOW_HEIGHT;
double VIEW_WIDTH;

void ParametersInput()
{
	ifstream infile;
	infile.open("config/config.txt");
	if (!infile.is_open())
	{
		cout << "Could Not Open File!!!";
		exit(EXIT_FAILURE);
	}
	//some variable used to read file
	double val;
	int temp;
	string st;
	int num;
	infile >> st;
	while (infile.good())
	{
		if (st.compare("Gx") == 0){
			infile >> val;
			gravity(0) = val;
		}
		if (st.compare("Gy") == 0){
			infile >> val;
			gravity(1) = val;
		}
		if (st.compare("r") == 0){
			infile >> val;
			r = val;
		}
		if (st.compare("iteratime") == 0){
			infile >> temp;
			iteratime = temp;
		}
		if (st.compare("endtime") == 0){
			infile >> temp;
			endtime = temp;
		}
		if (st.compare("SOLVER_STEPS") == 0){
			infile >> temp;
			SOLVER_STEPS = temp;
		}
		if (st.compare("WINDOW_WIDTH") == 0){
			infile >> temp;
			WINDOW_WIDTH = temp;
		}
		if (st.compare("WINDOW_HEIGHT") == 0){
			infile >> temp;
			WINDOW_HEIGHT = temp;
		}
		if (st.compare("VIEW_WIDTH") == 0){
			infile >> val;
			VIEW_WIDTH = val;
		}
		infile >> st;
	}
	if (infile.eof()) cout << "End Of File!!";
	infile.close();

	DT = ((1.0f / fps) / SOLVER_STEPS);//the defination of DT
	VIEW_HEIGHT = WINDOW_HEIGHT*VIEW_WIDTH / WINDOW_WIDTH;//the defination of VIEW_HEIGHT
}