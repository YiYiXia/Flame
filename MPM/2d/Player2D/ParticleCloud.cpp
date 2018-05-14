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



void ParticleCloud::merge(const ParticleCloud& other)
{
	size += other.size;
	particles.reserve(size);
	particles.insert(particles.end(), other.particles.begin(), other.particles.end());
}

void ParticleCloud::draw()
{
	
	glPointSize(3);
	glBegin(GL_POINTS);

	for (int i = 0; i < size-1; i++)
	{
		glColor3d(particles[i].color[0], particles[i].color[1], particles[i].color[2]);
		//glColor3f(1,0,0);
		glVertex2d(particles[i].position[0], particles[i].position[1]);
	}
	//cout << particles[size - 2].color[0] << "  " << particles[size - 2].color[1] << " " << particles[size - 2].color[2] << endl;
	glEnd();
}

void ParticleCloud::output()
{
	//通过测试，重建的粒子总体积大致等于格点所占体积，且粒子分布越密，粒子所占体积越小
	//float va,m;
	//for (int i = 0; i < size; i++)
	//{
	//	va += particles[i].volume;
	//	m += particles[i].mass;
	//	//cout << particles[i].mass << endl;
	//}
	cout << particles[0].position(0)<<"  "<< particles[0].position(1) << endl;
	//cout << m << endl;
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
	//cout << frame << endl;
	outfile << particles.size() << endl;
	for (int i = 0; i < particles.size(); i++)
	{
		outfile << particles[i].position(0) << " " << particles[i].position(1) << endl;
	}
	//outfile << "PPS Make and model: " << endl;  //将输出写入到文件中，所有cout的函数都可以用  
	outfile.close();
	frame++;
}
//mainCRTStartup
void ParticleCloud::filein(int n)
{
	particles.clear();
	ifstream infile;
	string over = ".txt";
	double val;

	stringstream ss;
	string str;
	int num;
	ss << n;
	ss >> str;
	str += over;
	infile.open(str);

	infile >> num;

	//cout <<n<<"  "<< num << endl;
	particles.reserve(num);

	size = 0;
	while (infile.good())
	{
		infile >> particles[size].position[0];
		infile >> particles[size].position[1];
		infile >> particles[size].color[0];
		infile >> particles[size].color[1];
		infile >> particles[size].color[2];
		//cout << val<<" ";
		//infile >> val;
		//cout << val << endl;

		size++;
	}
	//cout << size << endl;
	infile.close();

}