#include"../../include/common/File.h"

File::File()
{
	
}

File::~File()
{
}

void File::CreateFolder()
{
	//获取当前程序路径，建立与时间同名的文件夹
	string time = getTime();
	char pack[30];
	char op[300] = "md ";
	strcpy_s(pack, time.c_str());
	_getcwd(buffer, MAXPATH);
	cout << buffer << endl;
	strcat_s(buffer, "\\");
	strcat_s(buffer, pack);
	strcat_s(op, buffer);
	system(op);
	strcat_s(buffer, "\\");
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
	////在目录下新建的文件夹中输出文件
	//ofstream outfile, fout;
	//string over = ".txt";
	//stringstream ss;
	//string str;
	//ss << frame;
	//ss >> str;
	//str += over;
	//str = buffer + str;
	//outfile.open(str);
	////cout << frame << endl;
	//outfile << cloud.particles.size() << endl;
	//for (int i = 0; i < cloud.particles.size(); i++)
	//{
	//	outfile << cloud.particles[i]->position(0) << " " << cloud.particles[i]->position(1) << endl;
	//	outfile << cloud.particles[i]->color(0) << " " << cloud.particles[i]->color(1) << " " << cloud.particles[i]->color(2) << endl;
	//}
	//outfile.close();
}

 
void File::FileIn(int frame, ParticleCloud &cloud)
{
	//cloud.particles.clear();
	//ifstream infile;
	//string over = ".txt";
	//stringstream ss;
	//string str;
	//int num;
	//ss << frame;
	//ss >> str;
	//str += over;
	//infile.open(str);

	//infile >> num;
	//cloud.particles.reserve(num);

	//cloud.size = num;
	//cloud.p_num = 0;
	//int size = 0;
	//while (infile.good())
	//{
	//	cloud.particles[size] = new ParticleSand();
	//	infile >> cloud.particles[size]->position(0);
	//	infile >> cloud.particles[size]->position(1);
	//	cloud.particles[size]->index = size;
	//	size++;
	//}
	//cloud.p_num = size;
	//infile.close();
}

void File::CanvasOutput(int num, cv::Mat* canvas)
{
	string over = ".png";
	stringstream ss;
	string str;
	cout << num << endl;
	ss << num;
	ss >> str;
	str += over;
	str = buffer + str;
	cv::imwrite(str, *canvas);
}