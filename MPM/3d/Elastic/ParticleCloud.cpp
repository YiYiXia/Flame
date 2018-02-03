#include"ParticleCloud.h"

ParticleCloud::ParticleCloud()
{

}
ParticleCloud::ParticleCloud(int cloud_size)
{
	size = cloud_size;
	particles.reserve(size);
}
ParticleCloud::~ParticleCloud()
{

}

void ParticleCloud::update()
{
	for (int i = 0; i<size; i++) {
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

void ParticleCloud::draw()
{
	//glBegin(GL_LINES);
	//	glColor3f(1, 0, 0);
	//	glVertex3f(0.0, 0.0, 0.0);
	//	glVertex3f(0.0, 0.0, 1.0);
	//glEnd;
	//glColor4f(1, 1, 0, 0.5);
	////glRotatef(0, 1, 0, 0);
	//glTranslatef(0.0, 0.0, 0.0);
	//glScaled(3, 3, 3);
	//glutWireCube(1);
	for (int i = 0; i < size; i++)
	{
		glPushMatrix();
		{
			//glTranslatef(1.0f, 1.0f, 3.0f);//增加了变换
			glColor4f(0.0f, 1.0f, 0.0f, 0.5);
			glTranslatef(particles[i].position[0], particles[i].position[1], particles[i].position[2]);
			glutSolidSphere(0.03f, 30.0f, 30.0f);//绘制球体
		}
		glPopMatrix();
	
	}
	
}

void ParticleCloud::output()
{
	//通过测试，重建的粒子总体积大致等于格点所占体积，且粒子分布越密，粒子所占体积越小
	float va,m;
	for (int i = 0; i < size; i++)
	{
		va += particles[i].volume;
		m += particles[i].mass;
		//cout << particles[i].mass << endl;
	}
	cout << va << endl;
	cout << m << endl;
}