#include"ParticleCloud.h"
#include <fstream>  
#include<sstream>

ParticleCloud::ParticleCloud()
{

}
ParticleCloud::ParticleCloud(int cloud_size)
{
	size = cloud_size;
	particles.reserve(size);
	frame = 0;
}
ParticleCloud::~ParticleCloud()
{

}
//整个运算流程中，与粒子有关的流程
void ParticleCloud::update()
{
	//对于弹性粒子而言，它的整个工作流程是这样的：更新位置，更新形变梯度，对形变梯度计算SVD
	for (int i = 0; i<size; i++) 
	{
		particles[i].updatePos();
		particles[i].updateGradient();
		particles[i].applyPlasticity();
	}
}

void ParticleCloud::merge(const ParticleCloud& other)
{
	size += other.size;
	particles.reserve(size);
	particles.insert(particles.end(), other.particles.begin(), other.particles.end());
}

void ParticleCloud::fileout()
{
	ofstream outfile, fout;
	string over = ".txt";
	stringstream ss;
	string str;
	ss << frame;
	ss >> str;
	str += over;
	outfile.open(str);
	outfile << particles.size() << endl;
	for (int i = 0; i < particles.size(); i++)
	{
		outfile << particles[i].position(0) << " " << particles[i].position(1) << " " << particles[i].position(2) << endl;
	}
	//outfile << "PPS Make and model: " << endl;  //将输出写入到文件中，所有cout的函数都可以用  
	outfile.close();
	frame++;
}