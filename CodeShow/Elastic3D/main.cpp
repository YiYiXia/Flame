#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include"Parameters.h"
#include"Grid.h"
#include"ParticleCloud.h"
using namespace std;

int frame = 0;//用于输出文件信息

Vector3f gravity = Vector3f(0, 0,-1.1);//重力信息
Grid* grid;//网格
ParticleCloud* bar;//粒子团

//用于生成被模拟的粒子
vector<Particle> sample(Vector3f start, Vector3f end, Vector3f size,Vector3f v)
{
	Vector3f cellsize;
	cellsize[0] = (end[0] - start[0]) / (size[0] - 1);//格点x向长度
	cellsize[1] = (end[1] - start[1]) / (size[1] - 1);//格点y方向长度
	cellsize[2] = (end[2] - start[2]) / (size[2] - 1);//格点z方向长度
	int length = size[0] * size[1] * size[2];
	vector<Particle> points(length);

	for (int i = 0; i < size[0]; i++)
		for (int j = 0; j < size[1]; j++)
			for (int k = 0; k < size[2]; k++)
				points[(int)(k*size[0] * size[1] + j*size[0] + i)] = Particle(start + Vector3f(cellsize[0] * i, cellsize[1] * j, cellsize[2] * k), v, 0.1);

	return points;
}


//在此处初始化整个模拟场景
void InitMPM()
{
	vector<Particle> points = sample(Vector3f(1.618, 1.618, 1.118), Vector3f(2.245, 2.245, 1.745), Vector3f(20, 20, 20), Vector3f(0, 0, 0));//构建粒子
	bar = ParticleCloud::Insert(points);//将vector中的粒子赋给粒子团
	grid = new Grid(Vector3f(0, 0, 0), Vector3f(VIEW_WIDTH, VIEW_HEIGHT, VIEW_HEIGHT), Vector3f(100, 100, 100), bar);//将粒子团与grid绑定
	grid->initializeMass();	//初始化格点质量，方便下一步计算粒子体积
	grid->calculateVolumes();//计算粒子体积
}



int main(int argc, char** argv)
{
	//整个的模拟流程如下：
	InitMPM();
	for (int i = 0; i < 200; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			grid->initializeMass();//格点质量插值
			grid->initializeVelocities();//格点速度插值
			grid->explicitVelocities(gravity);//格点受力，速度更新（外力为重力）
			grid->updateVelocities();//更新粒子的速度、速度梯度、仿射矩阵
			bar->update();//更新粒子的形变梯度
		}
		bar->fileout();//输出粒子信息
	}
}
