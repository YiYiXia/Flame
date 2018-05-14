#include"File.h"
File::File()
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

File::~File()
{
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
		outfile << cloud.particles[i].position(0) << " " << cloud.particles[i].position(1) << endl;
	}
	outfile.close();
}

 
void File::FileIn(int frame, ParticleCloud cloud)
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
	int size = 0;
	while (infile.good())
	{
		infile >> cloud.particles[size].position(0);
		infile >> cloud.particles[size].position(1);
		size++;
	}
	infile.close();
}