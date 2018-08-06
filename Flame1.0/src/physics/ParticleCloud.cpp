#include"../../include/physics/ParticleCloud.h"

ParticleCloud::ParticleCloud()
{
}
ParticleCloud::ParticleCloud(int cloud_size)
{

}
ParticleCloud::~ParticleCloud()
{
}

PointList OrderSample(SDF* polygon)
{
	double x_length = polygon->x_max - polygon->x_min;
	double y_length = polygon->y_max - polygon->y_min;
	PointList cloud;
	double cellsizex = 0.02;
	double cellsizey = 0.02;
	//double cellsizex = VIEW_WIDTH / 500;
	//double cellsizey = VIEW_HEIGHT / 500;
	int size_x = (int)(x_length / cellsizex);
	int size_y = (int)(y_length / cellsizey);
	
	for (int j = 0; j <= size_y; j++)
	{
		for (int i = 0; i <= size_x; i++)
		{
			cloud.push_back(Vector2d(i*cellsizex + polygon->x_min, j*cellsizey + polygon->y_min));
			//cout << i*cellsizex + polygon->x_min<<"  "<<j*cellsizey + polygon->y_min << endl;
		}
	}
	cout << cloud.size() << endl;
	return cloud;
}

PointList PossionDisc(double r, SDF* polygon, int k)
{
	double x_length = polygon->x_max - polygon->x_min;
	double y_length = polygon->y_max - polygon->y_min;
	PointList cloud;
	double grid_size = r / sqrt(2);
	int x_size = ceil(x_length / grid_size);
	int y_size = ceil(y_length / grid_size);
	int* Inside = new int[x_size*y_size];                                         //存放格子内是否有粒子
	memset(Inside, 0, sizeof(int)*x_size*y_size);
	Eigen::Vector2d* C;
	C = new Eigen::Vector2d[x_size*y_size];                                       //
	memset(C, 0, sizeof(Eigen::Vector2d)*x_size*y_size);
	LinkList<int> active;
	double x, y;
	double x0 = x_length*RandomNumber(0, 1);
	double y0 = y_length*RandomNumber(0, 1);
	while (polygon->Contains(polygon->x_min + x0, polygon->y_min + y0) != -1)
	{
		x0 = x_length*RandomNumber(0, 1);
		y0 = y_length*RandomNumber(0, 1);
	}
	int x_index = floor(x0 / grid_size), y_index = floor(y0 / grid_size);          //确定出生点

	Inside[x_size*y_index + x_index] = 1;
	C[x_size*y_index + x_index] = Eigen::Vector2d(x0, y0);
	cloud.push_back(Vector2d(x0 + polygon->x_min, y0 + polygon->y_min));
	active.AddLNode(x_size*y_index + x_index);
	while (active.ListLength != 0)
	{
		int l = active.ListLength;
		int seed = l*RandomNumber(0, 0.9999) + 1;
		int cor = active.Search(seed);
		x0 = C[cor](0);
		y0 = C[cor](1);
		bool key1 = false;
		int j;
		for (j = 0; j < k; j++)
		{
			bool key2 = false;
			double R = r*sqrt(1.0 + 3.0 * RandomNumber(0, 1));//R属于r到2r
			x = x0 + R*cos(RandomNumber(0, 1) * 2.0 * M_PI);
			y = y0 + R*sin(RandomNumber(0, 1) * 2.0 * M_PI);
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
				cloud.push_back(Vector2d(x + polygon->x_min, y + polygon->y_min));
				key1 = true;
			}
		}
		if (key1 == false)	active.Delete(seed);


	}

	cout << "Num of particle" << cloud.size() << endl;
	return cloud;
}

PointList PossionDisc(double r, cv::Mat picture, SDF* polygon, int k)
{
	double x_length = polygon->x_max - polygon->x_min;
	double y_length = polygon->y_max - polygon->y_min;
	Vector2d leftdown = Vector2d(polygon->x_min, polygon->y_min);//矩形左下角

	int width = picture.cols;
	int height = picture.rows;
	double scalex = (double)width / x_length;
	double scaley = (double)height / y_length;
	int samplex = width / 10;
	int sampley = height / 10;
	double cellx = x_length / (double)samplex;
	double celly = y_length / (double)sampley;

	PointList cloud;
	double grid_size = r / sqrt(2);
	int x_size = ceil(x_length / grid_size);
	int y_size = ceil(y_length / grid_size);

	int* Inside = new int[x_size*y_size];                                         //存放格子内是否有粒子
	memset(Inside, 0, sizeof(int)*x_size*y_size);
	Eigen::Vector2d* C;
	C = new Eigen::Vector2d[x_size*y_size];                                       //
	memset(C, 0, sizeof(Eigen::Vector2d)*x_size*y_size);
	LinkList<int> active;

	double x, y;
	double x0, y0;
	int x_index, y_index;
	for (int i = 1; i < samplex; i++)
	{
		for (int j = 1; j < sampley; j++)
		{
			x0 = cellx*i;
			y0 = celly*j;
			if (MatContain(picture, scalex, scaley, Vector2d(x0, y0)))
			{
				x_index = floor(x0 / grid_size);
				y_index = floor(y0 / grid_size);          //确定出生点
				Inside[x_size*y_index + x_index] = 1;
				C[x_size*y_index + x_index] = Eigen::Vector2d(x0, y0);
				cloud.push_back(Vector2d(x0 + polygon->x_min, y0 + polygon->y_min));
				active.AddLNode(x_size*y_index + x_index);
			}
		}
	}
	while (active.ListLength != 0)
	{
		int l = active.ListLength;
		int seed = l*RandomNumber(0, 0.9999) + 1;
		int cor = active.Search(seed);
		x0 = C[cor](0);
		y0 = C[cor](1);
		bool key1 = false;
		int j;
		for (j = 0; j < k; j++)
		{
			bool key2 = false;
			double R = r*sqrt(1.0 + 3.0 * RandomNumber(0, 1));//R属于r到2r
			x = x0 + R*cos(RandomNumber(0, 1) * 2.0 * M_PI);
			y = y0 + R*sin(RandomNumber(0, 1) * 2.0 * M_PI);
			x_index = floor(x / grid_size);
			y_index = floor(y / grid_size);
			if (!MatContain(picture, scalex, scaley, Vector2d(x, y))) continue;
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
				cloud.push_back(Vector2d(x + polygon->x_min, y + polygon->y_min));
				key1 = true;
			}
		}
		if (key1 == false) active.Delete(seed);
	}
	cout << "Num of particle" << cloud.size() << endl;
	return cloud;
}

bool MatContain(cv::Mat picture, double scalex, double scaley, Vector2d p)
{
	int width = picture.cols;
	int height = picture.rows;
	int px = p[0] * scalex;
	int py = height - p[1] * scaley;
	if (picture.at<cv::Vec3b>(py, px)[0] == 0) return true;
	else return false;
}