#include"ParticleCloud.h"

ParticleCloud::ParticleCloud()
{
	size = 0;
	p_num = 0;
}
ParticleCloud::ParticleCloud(int cloud_size)
{
	size = cloud_size;
	particles.reserve(size);
	p_num = 0;
}
ParticleCloud::~ParticleCloud()
{
}

void ParticleCloud::update()
{
	for (int i = 0; i<size; i++) {
		/*if (i == 0|| i == 29 )
		{
			cout << particles[i].velocity[0]<<"  "<< particles[i].velocity[1] << endl;
		}*/
		Particle* p;
		p = particles[i];
		p->Update();

		/*particles[i]->updatePos();
		particles[i]->updateGradient();
		particles[i]->applyPlasticity();*/
	}
	//cout << endl;
}

void ParticleCloud::merge(ParticleCloud& other)
{
	size += other.size;
	for (int i = 0; i < other.particles.size(); i++){
		other.particles[i]->index += p_num;
	}
	p_num += other.size;
	particles.reserve(size);
	particles.insert(particles.end(), other.particles.begin(), other.particles.end());
}

void ParticleCloud::copy(ParticleCloud other)
{
	//欠鲁棒
	size = other.size;
	particles.reserve(size);
	for (int i = 0; i < other.particles.size(); i++)
	{

	}
}

ParticleCloud* ParticleCloud::Insert(std::vector<Particle*> points)
{
	ParticleCloud *obj = new ParticleCloud(points.size());

	for (int i = 0; i < points.size(); i++)
	{
		obj->particles.push_back(points[i]);
	}
	return obj;
}

void ParticleCloud::draw()
{
	glPointSize(3);
	glBegin(GL_POINTS);
	for (int i = 0; i < size; i++)
	{
		glColor3f(particles[i]->color[0], particles[i]->color[1], particles[i]->color[2]);
		
		glVertex2f(particles[i]->position[0], particles[i]->position[1]);
	}
	glEnd();
}

void ParticleCloud::output()
{
	//通过测试，重建的粒子总体积大致等于格点所占体积，且粒子分布越密，粒子所占体积越小
	double va,m;
	for (int i = 0; i < size; i++)
	{
		va += particles[i]->volume;
		m += particles[i]->mass;
		//cout << particles[i].mass << endl;
	}
	cout << va << endl;
	cout << m << endl;
}

void ParticleCloud::InitialSample()
{
	/*vector<Particle*> points;
	points = PossionSample(SAND,Vector2d(p_start_x, p_start_y), Vector2d(p_size_x, p_size_y), Vector2d(1.0, 0), r, iteratime);
	size = points.size();
	particles.reserve(size);
	for (int i = 0; i < points.size(); i++)
	{
		particles.push_back(points[i]);
	}*/
}


void ParticleCloud::AddParticles(ParticleType type, SDF* polygon, Vector2d velocity)
{
	vector<Particle*> points;
	points = PossionSample(type, polygon, velocity, r, iteratime);
	size = points.size();
	particles.reserve(size);
	for (int i = 0; i < points.size(); i++)
	{
		particles.push_back(points[i]);
	}
}

double ParticleCloud::random_number(double lo, double hi)
{
	return lo + rand() / (double)(RAND_MAX / (hi - lo));
}

vector<Vector2d> ParticleCloud::PossionDisc(double r, SDF* polygon, int k)
{
	double x_length = polygon->x_max - polygon->x_min;
	double y_length = polygon->y_max - polygon->y_min;

	vector<Vector2d> cloud;
	double grid_size = r / sqrt(2);
	int x_size = ceil(x_length / grid_size);
	int y_size = ceil(y_length / grid_size);

	int* Inside = new int[x_size*y_size];                                         //存放格子内是否有粒子
	memset(Inside, 0, sizeof(int)*x_size*y_size);
	Eigen::Vector2d* C;
	C = new Eigen::Vector2d[x_size*y_size];                                       //
	memset(C, 0, sizeof(Eigen::Vector2d)*x_size*y_size);
	//用链表代替cell来进行操作
	

	LinkList active;
	double x, y;
	double x0 = x_length*random_number(0, 1);
	double y0 = y_length*random_number(0, 1);
	while (polygon->Contains(polygon->x_min + x0, polygon->y_min + y0) != -1)
	{
		x0 = x_length*random_number(0, 1);
		y0 = y_length*random_number(0, 1);
	}
	int x_index = floor(x0 / grid_size), y_index = floor(y0 / grid_size);          //确定出生点

	Inside[x_size*y_index + x_index] = 1;
	C[x_size*y_index + x_index] = Eigen::Vector2d(x0, y0);
	cloud.push_back(Vector2d(x0 + polygon->x_min, y0 + polygon->y_min));
	active.AddLNode(x_size*y_index + x_index);
	while (active.ListLength != 0)
	{
		//int l = active.size();
		int l = active.ListLength;

		//cout << l << endl;
		int seed = l*random_number(0, 0.9999) + 1;
		/*if (seed >= l)
		{
			cout << seed << " " << l << endl;
		}*/
		int cor = active.Search(seed);
		x0 = C[cor](0);
		y0 = C[cor](1);
		
		/*list<int>::iterator iter = active.begin();
		for (int i = 0; iter != active.end(); iter++, i++)
		{
			if (i == seed)
			{
				x0 = C[(*iter)](0);
				y0 = C[(*iter)](1);
				break;
			}
		}*/

		bool key1 = false;
		int j;
		for (j = 0; j < k; j++)
		{
			bool key2 = false;
			double R = r*sqrt(1.0 + 3.0 * random_number(0, 1));//R属于r到2r
			x = x0 + R*cos(random_number(0, 1) * 2.0 * M_PI);
			y = y0 + R*sin(random_number(0, 1) * 2.0 * M_PI);
			x_index = floor(x / grid_size);
			y_index = floor(y / grid_size);
			if (polygon->Contains(polygon->x_min + x, polygon->y_min + y) != -1) continue;
			for (int u = x_index - 2; u <= x_index + 2; u++)
			{
				for (int v = y_index - 2; v <= y_index + 2; v++)
				{
					if (u< 0 || u >= x_size || v < 0 || v >= y_size) continue;
					if (Inside[v*x_size + u] == 1)
					{
						double d = sqrtf(pow(x - C[v*x_size + u](0), 2) + pow(y - C[v*x_size + u](1), 2));
						if (d < r) {
							key2 = true;
							break;
						}
					}
				}
			}
			if (key2 == false)
			{
				C[x_size*y_index + x_index] = Eigen::Vector2d(x, y);
				Inside[x_size*y_index + x_index] = 1;
				active.AddLNode(x_size*y_index + x_index);
				//active.push_back(x_size*y_index + x_index);

				cloud.push_back(Vector2d(x + polygon->x_min, y + polygon->y_min));
				//Cloud.push_back(Particle(Eigen::Vector2d(x + start_x, y + start_y)));
				key1 = true;
			}
		}
		if (key1 == false)
		{
			//cout << active.size() << endl;
			//active.erase(iter);
			//cout << active.size() << endl;
			active.Delete(seed);
		}

	}
	
	cout << "Num of particle" << cloud.size() << endl;
	return cloud;
}



vector<Particle*> ParticleCloud::PossionSample(ParticleType type, SDF* polygon, Vector2d v, double r, int k)
{
	vector<Vector2d> set = PossionDisc(r, polygon, k);
	while (set.size() < 20)
	{
		set = PossionDisc(r, polygon, k);
	}
	vector<Particle*> points;
	for (int i = 0; i < set.size(); i++)
	{
		Particle* p;

		if (type == SAND)	p = new ParticleSand(set[i], v, mass, p_num, lambda, mu);
		if (type == ELASTIC) p = new ParticleElastic(set[i], v, mass, p_num, 1000, 1000);
		if (type == WATER) p = new ParticleWater(set[i], v, mass, p_num, 7, 60000);
		if (type == SNOW) p = new ParticleSnow(set[i], v, mass, p_num, 20000, 20000, 10, 3.9e-2, 7.5e-3);
		//p = new ParticleElastic(set[i], v, mass, p_num, 30000, 30000);
		//p = new ParticleWater(set[i], v, mass, p_num,7,1000);
		//p = new Particle(set[i], v, mass, p_num, lambda, mu);
		points.push_back(p);
		p_num++;
	}

	return points;
}


vector<Particle*> ParticleCloud::OrderSample(Vector2d start, Vector2d end, Vector2d size, Vector2d v)
{
	Vector2d cellsize;
	cellsize[0] = (end[0] - start[0]) / (size[0] - 1);//格点横向长度
	cellsize[1] = (end[1] - start[1]) / (size[1] - 1);//格点竖直方向长度
	int length = size[0] * size[1];
	vector<Particle*> points(length);

	//如果不绘制格点，就不要加这些
	for (int i = 0; i < size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			Particle* p;
			p = new ParticleElastic(start + Vector2d(cellsize[0] * i, cellsize[1] * j), v, mass, p_num, 30000, 30000);
			//p = new ParticleSand(start + Vector2d(cellsize[0] * i, cellsize[1] * j), v, mass, p_num, lambda, mu);
			//p = new Particle(start + Vector2d(cellsize[0] * i, cellsize[1] * j), v, mass, p_num, lambda, mu);
			points[(int)(j*size[0] + i)] = p;
			p_num++;
			/*points[(int)(j*size[0] + i)].mass = 1;
			points[(int)(j*size[0] + i)].velocity = Vector2d(0, 0);*/
		}
	}
	return points;
}
