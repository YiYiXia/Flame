#include"File.h"
double lambda;
double mu;
double STICKY;
double h0;
double h1;
double h2;
double h3;
double mass;
double DT;
int insert_time;
double p_start_x, p_start_y, p_size_x, p_size_y, r;
int iteratime, endtime;
int SOLVER_STEPS;
Vector2d gravity = Vector2d(0, -3.9);
double start_x, start_y, end_x, end_y, g;
int size_x, size_y;


File::File()
{
	
}

File::~File()
{
}

void File::Initial()
{
	//获取当前程序路径，建立与时间同名的文件夹
	string time = getTime();
	char pack[30];
	char op[300] = "md ";
	strcpy(pack, time.c_str());
	_getcwd(buffer, MAXPATH);
	cout << buffer << endl;
	strcat(buffer, "\\");
	strcat(buffer, pack);
	strcat(op, buffer);
	system(op);
	strcat(buffer, "\\");
}

string File::getTime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d-%H.%M.%S", localtime(&timep));
	return tmp;
}

void File::FileOut(int frame, ParticleCloud cloud)
{
	//在目录下新建的文件夹中输出文件
	ofstream outfile, fout;
	string over = ".txt";
	stringstream ss;
	string str;
	ss << frame;
	ss >> str;
	str += over;
	str = buffer + str;
	outfile.open(str);
	//cout << frame << endl;
	outfile << cloud.particles.size() << endl;
	for (int i = 0; i < cloud.particles.size(); i++)
	{
		outfile << cloud.particles[i]->position(0) << " " << cloud.particles[i]->position(1) << endl;
		outfile << cloud.particles[i]->color(0) << " " << cloud.particles[i]->color(1) << " " << cloud.particles[i]->color(2) << endl;
	}
	outfile.close();
}

 
void File::FileIn(int frame, ParticleCloud &cloud)
{
	cloud.particles.clear();
	ifstream infile;
	string over = ".txt";
	double val;
	stringstream ss;
	string str;
	int num;
	ss << frame;
	ss >> str;
	str += over;
	infile.open(str);

	infile >> num;
	cloud.particles.reserve(num);

	cloud.size = num;
	cloud.p_num = 0;
	int size = 0;
	while (infile.good())
	{
		cloud.particles[size] = new ParticleSand();
		infile >> cloud.particles[size]->position(0);
		infile >> cloud.particles[size]->position(1);
		cloud.particles[size]->index = size;
		size++;
	}
	cloud.p_num = size;
	infile.close();
}

void File::FileIn(int frame, vector<Vector2d> &pointlist)
{
	double x, y;
	ifstream infile;
	string over = ".txt";
	double val;
	stringstream ss;
	string str;
	int num;
	ss << frame;
	ss >> str;
	str += over;
	infile.open(str);
	if (!infile) return;
	infile >> num;
	while (infile.good())
	{
		infile >> x;
		infile >> y;
		pointlist.push_back(Vector2d(x, y));
		//cout << x << " "<<y << endl;
	}
	infile.close();
}

void File::ParametersIn()
{
	ifstream infile;
	infile.open("config.txt");
	if (!infile.is_open())
	{
		cout << "Could Not Open File!!!";
		exit(EXIT_FAILURE);
	}
	double val;
	int temp;
	string st;
	int num;
	infile >> st;
	while (infile.good())
	{
		if (st.compare("lambda") == 0)
		{
			infile >> val;
			lambda = val;
			//cout << val << endl;
		}
		if (st.compare("mu") == 0)
		{
			infile >> val;
			mu = val;
			//cout << val << endl;
		}

		if (st.compare("STICKY") == 0)
		{
			infile >> val;
			STICKY = val;
			//cout << val << endl;
		}
		if (st.compare("h0") == 0)
		{
			infile >> val;
			h0 = val;
			//cout << val << endl;
		}
		if (st.compare("h1") == 0)
		{
			infile >> val;
			h1 = val;
			//cout << val << endl;
		}
		if (st.compare("h2") == 0)
		{
			infile >> val;
			h2 = val;
			//cout << val << endl;
		}
		if (st.compare("h3") == 0)
		{
			infile >> val;
			h3 = val;
			//cout << val << endl;
		}
		if (st.compare("start_x") == 0)
		{
			infile >> val;
			start_x = val;
			//cout << val << endl;
		}
		if (st.compare("start_y") == 0)
		{
			infile >> val;
			start_y = val;
			//cout << val << endl;
		}
		if (st.compare("size_x") == 0)
		{
			infile >> num;
			size_x = num;
			//cout << num << endl;
		}
		if (st.compare("size_y") == 0)
		{
			infile >> num;
			size_y = num;
			//cout << num << endl;
		}
		if (st.compare("g") == 0)
		{
			infile >> val;
			gravity(1) = val;
			//cout << val << endl;
		}
		if (st.compare("mass") == 0)
		{
			infile >> val;
			mass = val;
			//cout << val << endl;
		}

		if (st.compare("insert_time") == 0)
		{
			infile >> temp;
			insert_time = temp;
			//cout << 1 << endl;
		}
		if (st.compare("p_start_x") == 0)
		{
			infile >> val;
			p_start_x = val;
			//cout << 2 << endl;
		}
		if (st.compare("p_start_y") == 0)
		{
			infile >> val;
			p_start_y = val;
			//cout << 3 << endl;
		}
		if (st.compare("p_size_x") == 0)
		{
			infile >> val;
			p_size_x = val;
			//cout << 4 << endl;
		}
		if (st.compare("p_size_y") == 0)
		{
			infile >> val;
			p_size_y = val;
			//cout << 5 << endl;
		}
		if (st.compare("r") == 0)
		{
			infile >> val;
			r = val;
			//cout << 6 << endl;
		}
		if (st.compare("iteratime") == 0)
		{
			infile >> temp;
			iteratime = temp;
			//cout << 7 << endl;
		}
		if (st.compare("endtime") == 0)
		{
			infile >> temp;
			endtime = temp;
			//cout << 7 << endl;
		}
		if (st.compare("SOLVER_STEPS") == 0)
		{
			infile >> temp;
			SOLVER_STEPS = temp;
			//cout << 7 << endl;
		}
		infile >> st;
	}
	if (infile.eof())
		cout << "End Of File!!";
	infile.close();
	DT = ((1.0f / fps) / SOLVER_STEPS);
}