#include "Mesh3D.h"
#include <fstream>
#include<iostream>
#include <xutility>
#include<stdio.h>
#include<stdlib.h>
#include"Eigen/Dense"                                       //调用Eigen
#include <Eigen/Sparse>
using namespace Eigen;                                            //Eigen命名空间
typedef Eigen::SparseMatrix<double> SparseMatrixType;
using namespace std;
#define SWAP(a,b,T) {T tmp=(a); (a)=(b); (b)=tmp;}
#define min(a,b) a<b?a:b
#define max(a,b) a>b?a:b


Mesh3D::Mesh3D(void)
{
	// intialization
	pvertices_list_ = NULL;
	pfaces_list_ = NULL;
	pedges_list_ = NULL;
	finish_prepare_=false;
	finish_open_=false;
	turn=0;
	xmax_ = ymax_ = zmax_ = 1.f;
	xmin_ = ymin_ = zmin_ = -1.f;
	
	num_components_ = 0;
	average_edge_length_ = 1.f;
}

void Mesh3D::ClearData(void)
{
	ClearVertex();
	ClearEdges();
	ClearFaces();
	edgemap_.clear();

	xmax_ = ymax_ = zmax_ = 1.f;
	xmin_ = ymin_ = zmin_ = -1.f;
}

void Mesh3D::ClearVertex(void)
{

	if (pvertices_list_==NULL)
	{
		return;
	}
	else
	{
		for (VERTEX_ITER viter = pvertices_list_->begin(); viter != pvertices_list_->end(); viter++)
		{
			if (*viter != NULL)
			{
				delete *viter;
				*viter = NULL;
			}
			else
			{
				// ERROR
			}
		}
		delete pvertices_list_;
		pvertices_list_ = NULL;
	}
}


void Mesh3D::ClearEdges(void)
{
	if (pedges_list_ == NULL)
	{
		return;
	}
	else
	{
		for (EDGE_ITER eiter = pedges_list_->begin(); eiter!=pedges_list_->end(); eiter++)
		{
			if (*eiter != NULL)
			{
				delete *eiter;
				*eiter = NULL;
			}
			else
			{
				// ERROR
			}
		}
		delete pedges_list_;
		pedges_list_ = NULL;
	}
}

void Mesh3D::ClearFaces(void)
{
	if (pfaces_list_==NULL)
	{
		return;
	}
	else
	{
		for (FACE_ITER fiter = pfaces_list_->begin(); fiter!=pfaces_list_->end(); fiter++)
		{
			if (*fiter != NULL)
			{
				delete *fiter;
				*fiter = NULL;
			}
			else
			{
				// ERROR
			}
		}
		delete pfaces_list_;
		pfaces_list_ = NULL;
	}
}



//void Mesh3D::divide()                                                                   //边界预处理
//{
//	if(finish_open_==false)                                                             //增强鲁棒性
//	{
//		return;
//	}
//	if(turn>=1)
//	{
//		finish_prepare_=true;
//		return;
//	}
//	turn++;
//
//	//miniface_=new MinifaceItera();
//	miniface_=new MinifaceMatrix();
//	CopyVetry(); 
//	Create2D();
//	miniface_->Openlist("circle.txt");
//	miniface_->Analysetriangle();                                                //分析三角网格，获得点的连接关系
//	CopyVetry();                                                                //记录点的坐标
//	CreatePoint();                                                              //由2D重心坐标生成3D点的坐标
//	CreateEdge();
//	CreateFace();                                                               //由三角剖分的三角关系，生成面
//	
//	
//	CreateBoundary();
//	UpdateMesh();                                                               //绘制网格
//}

//void Mesh3D::CalucateMatrix()                                                   //全局矩阵计算
//{
//	miniface_->CreateMiniface("miniface.txt");
//	CopyVetry("miniface.txt");
//	SetVetry();
//	UpdateMesh();
//}
//
//void Mesh3D::CreateMiniFace()                                                             //生迭代法成最小曲面
//{
//	if(miniface_->out.numberofpoints<=0)                                                   //增强鲁棒性
//	{
//		pass=false;
//		return;
//	}
//	pass=true;
//	miniface_->CreateMiniface("miniface.txt");
//	CopyVetry("miniface.txt");
//	SetVetry();
//	UpdateMesh();
//}

void Mesh3D::Create2D()                                                                       //由3D点生成3D点
{
	FILE *out;
	out = fopen("circle.txt","w");
	vector<float> lenth;
	float z_max_,z_min_,x_max_,x_min_,y_min_,y_max_;
	float d;
	float s=0;
	for(int i=0;i<verty_list_.size()-1;i++)                                                   //记录各边边长，依据边长将点投射到一个圆上
	{
		lenth.push_back(s);
		d=pow(verty_list_[i].x()-verty_list_[i+1].x(),2)
			+pow(verty_list_[i].y()-verty_list_[i+1].y(),2)
			+pow(verty_list_[i].z()-verty_list_[i+1].z(),2);
		d=pow(d,0.5);
		s=s+d;
	}
	lenth.push_back(s);
	d=pow(verty_list_[verty_list_.size()-1].x()-verty_list_[0].x(),2)
			+pow(verty_list_[verty_list_.size()-1].y()-verty_list_[0].y(),2)
			+pow(verty_list_[verty_list_.size()-1].z()-verty_list_[0].z(),2);
	d=pow(d,0.5);
	s=s+d;
	
	float x,y;
	for(int i=0;i<lenth.size();i++)                                                            //由边长生成各点2D坐标
	{
		x=(s/6)*cos(6.2831*lenth[i]/s);
		y=(s/6)*sin(6.2831*lenth[i]/s);
		fprintf(out,"%.1f %.1f",x,y);
		if(i!=(lenth.size()-1))                                                               //打开文件的时候最后一个\n会被当字符输入，导致triangle出错
		{
			fprintf(out,"\n");
		}
	}
	fclose(out);
}

void Mesh3D::SetVetry()                                                                       //由verty_list_设置3D坐标
{
	int num=0;
	VERTEX_ITER viter2 = pvertices_list_->begin();
	viter2 = pvertices_list_->begin();
	for (;viter2!=pvertices_list_->end(); viter2++) 
	{
		(*viter2)->position_.x()=verty_list_[num].x();
		(*viter2)->position_.y()=verty_list_[num].y();
		(*viter2)->position_.z()=verty_list_[num].z();
		num++;
	}
}
void Mesh3D::CopyVetry(const char*fins)                                                                     //从文件对当前点坐标复制
{
	Vec3f nvv;
	verty_list_.clear();
	temp_list_.clear();
	int n;
	FILE *pfile = fopen(fins, "r");                                                         //文件必须是一行3个数字
	if(pfile==NULL)
	{
		cout<<"failed"<<endl;                                                      
	}
	while(!feof(pfile))                                                                    //统计点数
	{
		fscanf(pfile,"%f",&nvv[0]);
		fscanf(pfile,"%f",&nvv[1]);
		fscanf(pfile,"%f",&nvv[2]);
		fscanf(pfile,"%d",&n);
		verty_list_.push_back(nvv);
		temp_list_.push_back(nvv);
	}
	fclose(pfile);
}

void Mesh3D::CopyVetry()                                                                     //直接当前点坐标记录复制
{
	Vec3f nvv;
	verty_list_.clear();
	temp_list_.clear();
	VERTEX_ITER viter2 = pvertices_list_->begin();
	for (;viter2!=pvertices_list_->end(); viter2++) 
	{
		nvv[0]=(*viter2)->position_.x();
		nvv[1]=(*viter2)->position_.y();
		nvv[2]=(*viter2)->position_.z();
		verty_list_.push_back(nvv);
		temp_list_.push_back(nvv);
	}
}


void Mesh3D::CreateBoundary()
{

	EDGE_ITER viter4 =pedges_list_->begin();
	for(;viter4!=pedges_list_->end();viter4++)
	{
		(*viter4)->bound=0;
	}
	FACE_ITER viter = pfaces_list_->begin();
	for(;viter!=pfaces_list_->end();viter++)
	{
		HE_edge *edge=(*viter)->pedge_;
		edge->bound++;
		edge=edge->pnext_;
		while(edge!=(*viter)->pedge_)
		{
			edge->bound++;
			edge=edge->pnext_;
		}
	}
	int j=0;
	int n=0;
	EDGE_ITER viter2 =pedges_list_->begin();
	for(;viter2!=pedges_list_->end();viter2++)
	{
		n++;
		if((*viter2)->bound!=1||(*viter2)->ppair_->bound!=1)
		{
			j++;
			if((*viter2)->bound!=1)
			{
				(*viter2)->boundary_flag_=BOUNDARY;
			}
			if((*viter2)->ppair_->bound!=1)
			{
				(*viter2)->ppair_->boundary_flag_=BOUNDARY;
			}
			(*viter2)->pvert_->bound=1;
			(*viter2)->ppair_->pvert_->bound=1;
			(*viter2)->pvert_->boundary_flag_=BOUNDARY;
			(*viter2)->ppair_->pvert_->boundary_flag_=BOUNDARY;
		}
		else
		{
			(*viter2)->pvert_->bound=0;
			(*viter2)->ppair_->pvert_->bound=0;
			(*viter2)->pvert_->boundary_flag_=INNER;
			(*viter2)->ppair_->pvert_->boundary_flag_=INNER;
		}
	}
}


//第一层，完成对点坐标的赋值，点列表已完成
HE_vert* Mesh3D::InsertVertex(const Vec3f& v)
{
	HE_vert* pvert = new HE_vert(v);                                                   //点坐标赋值
	if (pvertices_list_ == NULL)
	{
		pvertices_list_ = new std::vector<HE_vert*>;
	}
	pvert->id_ = static_cast<int>(pvertices_list_->size());
	pvertices_list_->push_back(pvert);                                                  //存入点的坐标
	return pvert;
}

//第二层，由起始点构造边，完成点的指向，完成半边的指向。起始点的输入是为了定向
HE_edge* Mesh3D::InsertEdge(HE_vert* vstart, HE_vert* vend)                    //以两点为起始点新增半边，返回有向半边
{
	if (vstart==NULL || vend==NULL)                                            //起始点有一个为空，退出。返回空边
	{
		return NULL;
	}

	if (pedges_list_==NULL)
	{
		pedges_list_ = new std::vector<HE_edge*>;
	}

	if (edgemap_[PAIR_VERTEX(vstart, vend)] != NULL)                            //找有没有以vstart和vend为起始点的半边
	{
		return edgemap_[PAIR_VERTEX(vstart, vend)];                             //有，返回该半边
	}

	HE_edge* pedge = new HE_edge;                                               //没有，新建半边
	pedge->pvert_ = vend;                                                       //指向半边终点
	pedge->pvert_->degree_ ++;                                                  //图论中的入度加一
	vstart->pedge_ = pedge;                                                     //起点所指的半边，任意一条
	edgemap_[PAIR_VERTEX(vstart, vend)] = pedge;                                //map中存入该半边
	
	pedge->id_ = static_cast<int>(pedges_list_->size());                        //给新半边编号
	pedges_list_->push_back(pedge);                                             //半边存入数据

	return pedge;                                                               //返回新半边
}
//第三层，由顶点组生成面，面上的半边指向面，指向对，指向后一个，面指向半边。
HE_face* Mesh3D::InsertFace(std::vector<HE_vert* >& vec_hv)                    //新建面，输入顶点组，返回面
{
	int vsize = static_cast<int>(vec_hv.size());                               //标准转换运算符，作用域限于本函数
	if (pfaces_list_ == NULL)                                                  //面表空，新建面表
	{
		pfaces_list_ = new std::vector<HE_face*>;
	}

	HE_face *pface = new HE_face;                                             //开辟面
	pface->valence_ = vsize;                                                  //valence：价  给出面所包含的边数    
	VERTEX_ITER viter = vec_hv.begin();                                       //参数点组第一个元素
	VERTEX_ITER nviter = vec_hv.begin();                                      //参数点组第二个元素
	nviter ++;
	HE_edge *he1=NULL, *he2=NULL;                                             //新建半边为空
	std::vector<HE_edge*> vec_edges;                                          //新建半边组
	int i=0;
	for (i=0; i<vsize-1; i++)                                                 //遍历所有的点
	{
		he1 = InsertEdge( *viter, *nviter);                                   //前->后
		he2 = InsertEdge( *nviter, *viter);                                   //后—>前   半边双向，两个在一条线上

		if (pface->pedge_==NULL)                                              //如果面没有初始化，将面指向半边he1
			pface->pedge_ = he1;

		he1->pface_ = pface;                                                  //he1指向面
		he1->ppair_ = he2;                                                    //he1的pair为he2
		he2->ppair_ = he1;                                                    //he2的pair为he1
		vec_edges.push_back(he1);                                             //he1压入新半边组
		viter++, nviter++;                                                    //迭代器自增
	}

	nviter = vec_hv.begin();                                                  //循环位错  增补一次

	he1 = InsertEdge(*viter, *nviter);
	he2 = InsertEdge(*nviter , *viter);
	he1->pface_ = pface;
	if (pface->pedge_==NULL) 
		pface->pedge_ = he1;

	he1->ppair_ = he2;
	he2->ppair_ = he1;
	vec_edges.push_back(he1);

	for (i=0; i<vsize-1; i++)                                                //对新半边组所有点进行遍历
	{
		vec_edges[i]->pnext_ = vec_edges[i+1];                               //每个半边next指向它后一个半边
		vec_edges[i+1]->pprev_ = vec_edges[i];                               //每个半边pre指向它前一个半边
	}
	vec_edges[i]->pnext_ = vec_edges[0];                                     //循环位错
	vec_edges[0]->pprev_ = vec_edges[i];

	pface->id_ = static_cast<int>(pfaces_list_->size());                     //面序号
	pfaces_list_->push_back(pface);                                          //压入面

	return pface;
}
//只要有v有f的文件被打开，就能自动生成边界
bool Mesh3D::LoadFromOBJFile(const char* fins)   //v顶点 vt纹理坐标 vn法向量 f面 顶点、纹理、法向量索引
{
	finish_prepare_=false;
	finish_open_=false;
	turn=0;
	cout << "Loading......." << endl;
	FILE *pfile = fopen(fins, "r");

	char *tok;
	//char *tok_tok;
	char temp[128];

	try
	{
		ClearData();
		//read vertex
		fseek(pfile, 0, SEEK_SET);
		char pLine[512];

		while(fgets(pLine, 512, pfile))
		{
			if(pLine[0] == 'v' && pLine[1] == ' ')
			{
				Vec3f nvv;                                                                   //插入点的方法
				tok = strtok(pLine," ");
				for (int i=0; i<3; i++)                                                      //此处可以修改点的坐标
				{
					tok = strtok(NULL," ");
					strcpy(temp, tok);
					temp[strcspn(temp," ")] = 0;
					nvv[i] = (float)atof(temp);
				}
				InsertVertex(nvv);
			}
		}
		finish_open_=true;
		fseek(pfile, 0, SEEK_SET);

		while(fgets(pLine, 512, pfile))
		{
			char *pTmp = pLine;
			if(pTmp[0] == 'f')
			{
				std::vector<HE_vert* > s_faceid;

				tok = strtok(pLine," ");
				while ((tok = strtok(NULL," ")) != NULL)
				{
					strcpy(temp, tok);
					temp[strcspn(temp, "/")] = 0;
					int id = (int)strtol(temp, NULL, 10) - 1;
					HE_vert* hv = get_vertex(id);
					bool findit = false;
					for (int i = 0; i <(int) s_faceid.size(); i++)
					{
						if (hv == s_faceid[i])	//remove redundant vertex id if it exists
						{
							//	cout << "remove redundant vertex" << endl;
							findit = true;
							break;
						}
					}
					if (findit == false && hv != NULL)
					{
						s_faceid.push_back(hv);
					}
				}
				
				if ((int)s_faceid.size() >= 3)
				{
					InsertFace(s_faceid);
				}
			}
		}

		//read texture coords
		fseek(pfile, 0, SEEK_SET);
		std::vector<Vec3f> texCoordsTemp;
		while (fscanf(pfile, "%s", pLine) != EOF)
		{
			if (pLine[0] == 'v' && pLine[1] == 't')
			{
				Vec3f texTemp(0.f, 0.f, 0.f);
				fscanf(pfile, "%f %f", &texTemp[0], &texTemp[1]);
				texCoordsTemp.push_back(texTemp);
			}
		}
		//read texture index

		if (texCoordsTemp.size() > 0)
		{
			fseek(pfile, 0, SEEK_SET);

			int faceIndex = 0;
			while (fscanf(pfile, "%s", pLine) != EOF)
			{

				if (pLine[0] == 'f')
				{
					int v, t;
					fscanf(pfile, "%s", pLine);
					if (sscanf(pLine, "%d/%d", &v, &t) == 2)
					{
						std::map<int, int> v2tex;
						v2tex[v - 1] = t - 1;

						fscanf(pfile, "%s", pLine);
						sscanf(pLine, "%d/%d", &v, &t);
						v2tex[v - 1] = t - 1;

						fscanf(pfile, "%s", pLine);
						sscanf(pLine, "%d/%d", &v, &t);
						v2tex[v - 1] = t - 1;

						HE_edge* edgeTemp = pfaces_list_->at(faceIndex)->pedge_;
						edgeTemp->texCoord_ = texCoordsTemp.at(v2tex[edgeTemp->pvert_->id_]);	
						edgeTemp->pvert_->texCoord_ = edgeTemp->texCoord_;
						edgeTemp = edgeTemp->pnext_;
						edgeTemp->texCoord_ = texCoordsTemp.at(v2tex[edgeTemp->pvert_->id_]);
						edgeTemp->pvert_->texCoord_ = edgeTemp->texCoord_;
						edgeTemp = edgeTemp->pnext_;
						edgeTemp->texCoord_ = texCoordsTemp.at(v2tex[edgeTemp->pvert_->id_]);
						edgeTemp->pvert_->texCoord_ = edgeTemp->texCoord_;
						faceIndex++;
					}
				}
			}
		}

		cout << pvertices_list_->size() << " vertex, " << pfaces_list_->size() << " faces " << endl;

		UpdateMesh();
		Unify(2.f);
	}
	catch (...)
	{
		ClearData();
		xmax_ = ymax_ = zmax_ = 1.f;
		xmin_ = ymin_ = zmin_ = -1.f;

		fclose(pfile);
		return false;
	}

	fclose(pfile);

	return isValid();
}



void Mesh3D::WriteToOBJFile(const char* fouts)                                                         //输出OBJ函数
{
	std::ofstream fout(fouts);

	fout<<"g object\n";
	fout.precision(16);
	//output coordinates of each vertex
	VERTEX_ITER viter = pvertices_list_->begin();
	for (;viter!=pvertices_list_->end(); viter++) 
	{
		fout<<"v "<< std::scientific <<(*viter)->position_.x() 
			<<" "<<(*viter)->position_.y() <<" "<< (*viter)->position_.z() <<"\n";                    //(*viter)->position_.x()表示第一个数字，是横坐标
	}

	// 		for (viter = pvertices_list_->begin();viter!=pvertices_list_->end(); viter++) 
	// 		{
	// 			fout<<"vn "<< std::scientific <<(*viter)->normal_.x() 
	// 				<<" "<<(*viter)->normal_.y() <<" "<<(*viter)->normal_.z() <<"\n";
	// 		}
	//output the valence of each face and its vertices_list' id

	FACE_ITER fiter = pfaces_list_->begin();

	for (;fiter!=pfaces_list_->end(); fiter++) 
	{
		fout<<"f";

		HE_edge* edge = (*fiter)->pedge_; 

		do {
			fout<<" "<<edge->ppair_->pvert_->id_+1;
			edge = edge->pnext_;

		} while (edge != (*fiter)->pedge_);
		fout<<"\n";
	}

	fout.close();
}

void Mesh3D::UpdateMesh(void)
{
	if (!isValid())
	{
		std::cout << "Invalid" << "\n";
		return;
	}
	SetBoundaryFlag();
	BoundaryCheck();
	UpdateNormal();
	ComputeBoundingBox();
	ComputeAvarageEdgeLength();
	SetNeighbors();
}

void Mesh3D::SetBoundaryFlag(void)
{
	for (EDGE_ITER eiter = pedges_list_->begin(); eiter!=pedges_list_->end(); eiter++)
	{
		if ((*eiter)->pface_ == NULL)
		{
			(*eiter)->bound=0;
			(*eiter)->set_boundary_flag(BOUNDARY);
			(*eiter)->ppair_->set_boundary_flag(BOUNDARY);
			(*eiter)->pvert_->set_boundary_flag(BOUNDARY);
			(*eiter)->ppair_->pvert_->set_boundary_flag(BOUNDARY);
			(*eiter)->ppair_->pface_->set_boundary_flag(BOUNDARY);
		}
		else
		{
			(*eiter)->bound=1;
		}
	}
}

void Mesh3D::BoundaryCheck()
{
	for (VERTEX_ITER viter=pvertices_list_->begin(); viter!=pvertices_list_->end(); viter++)
	{
		if ((*viter)->isOnBoundary())
		{
			HE_edge* edge = (*viter)->pedge_;
			int deg = 0;
			while (edge->pface_!=NULL && deg<(*viter)->degree())
			{
				edge = edge->pprev_->ppair_;
				deg ++;
			}
			(*viter)->pedge_ = edge;
		}
	}
}

void Mesh3D::UpdateNormal(void)
{
	ComputeFaceslistNormal();
	ComputeVertexlistNormal();
}

void Mesh3D::ComputeFaceslistNormal(void)
{
	for (FACE_ITER fiter = pfaces_list_->begin(); fiter!=pfaces_list_->end(); fiter++)
	{
		ComputePerFaceNormal(*fiter);
	}
}

void Mesh3D::ComputePerFaceNormal(HE_face* hf)
{
	HE_edge *pedge = hf->pedge_;
	HE_edge *nedge = hf->pedge_->pnext_;

	HE_vert *p = pedge->pvert_;
	HE_vert *c = pedge->pnext_->pvert_;
	HE_vert *n = nedge->pnext_->pvert_;

	Vec3f pc, nc;
	pc = p->position_ - c->position_;
	nc = n->position_ - c->position_;

	hf->normal_ = nc ^ pc;	// cross prodoct
	hf->normal_.normalize();
}

void Mesh3D::ComputeVertexlistNormal(void)
{
	for (VERTEX_ITER viter = pvertices_list_->begin(); viter!=pvertices_list_->end(); viter++) 
	{
		ComputePerVertexNormal(*viter);
	}
}

void Mesh3D::ComputePerVertexNormal(HE_vert* hv)                                        //计算法向量
{
	if (hv->degree_ < 2)
	{
		// ERROR: the degree of the vertex is less than 2
		hv->normal_ = Vec3f(1.f,0.f,0.f);
		return;
	}

	HE_edge *edge = hv->pedge_;
	if (edge == NULL)
	{
		// ERROR: the edge attached to the vertex is NULL
		hv->normal_ = Vec3f(1.f,0.f,0.f);
		return;
	}

	hv->normal_ = Vec3f(0.f,0.f,0.f);
	if (hv->boundary_flag_ == INNER)                                      //对顶点各边线进行遍历
	{
		int iterNum = 0;
		do 
		{
			iterNum++;
			if (iterNum > hv->degree())
			{
				/*hv->set_position(hv->position() * 1.1f);*/
				std::cout << "    iterNum > hv->degree : " << hv->id() << "\n";
				break;
			}
			//hv->normal_ = hv->normal_ + edge->pface_->normal_;
			Vec3f  p = edge->pvert_->position(),
				q = edge->pnext_->pvert_->position(),
				r = edge->pprev_->pvert_->position();
			Vec3f  n = (q-p) ^ (r-p);                                                            //叉乘
			hv->normal_ = hv->normal_ + n;                                                       //对所有外积求和
			edge = edge->ppair_->pnext_;
		} while (edge != hv->pedge_ && edge != NULL);
	}
	else
	{
		// NOTE: for the boundary vertices, this part may be something wrong
		//	     Up to now, define the normals all as unity
		hv->normal_ = Vec3f(1.f, 0.f, 0.f);

		//int degree_flag = 0;
		//for (int i=0; i<hv->degree_-1; i++)
		//{
		//	edge = edge->ppair_->pnext_;
		//	if (edge == NULL)
		//	{
		//		// ERROR: the algorithm of computing boundary vertices has errors!
		//		break;
		//	}
		//	if (edge->pface_ != NULL)
		//	{
		//		hv->normal_ = hv->normal_ + edge->pface_->normal_;
		//	}
		//}
	}
	hv->normal_.normalize();
}

void Mesh3D::ComputeBoundingBox(void)
{
	if (pvertices_list_->size() < 3)
	{
		return;
	}

#define MAX_FLOAT_VALUE (static_cast<float>(10e10))
#define MIN_FLOAT_VALUE	(static_cast<float>(-10e10))
	
	xmax_ = ymax_ = zmax_ = MIN_FLOAT_VALUE;
	xmin_ = ymin_ = zmin_ = MAX_FLOAT_VALUE;

	VERTEX_ITER viter = pvertices_list_->begin();
	for (; viter!=pvertices_list_->end(); viter++)
	{
		xmin_ = min(xmin_, (*viter)->position_.x());
		ymin_ = min(ymin_, (*viter)->position_.y());
		zmin_ = min(zmin_, (*viter)->position_.z());
		xmax_ = max(xmax_, (*viter)->position_.x());
		ymax_ = max(ymax_, (*viter)->position_.y());
 		zmax_ = max(zmax_, (*viter)->position_.z());
	}
}

void Mesh3D::Unify(float size)
{
	float scaleX = xmax_ - xmin_;
	float scaleY = ymax_ - ymin_;
	float scaleZ = zmax_ - zmin_;
	float scaleMax;

	if (scaleX < scaleY)
	{
		scaleMax = scaleY;
	}
	else
	{
		scaleMax = scaleX;
	}
	if (scaleMax < scaleZ)
	{
		scaleMax = scaleZ;
	}
	float scaleV = size / scaleMax;
	Vec3f centerPos((xmin_ + xmax_) / 2.f, (ymin_ + ymax_) / 2.f, (zmin_ + zmax_) / 2.f);
	for (size_t i = 0; i != pvertices_list_->size(); i++)
	{
		pvertices_list_->at(i)->position_ = (pvertices_list_->at(i)->position_ - centerPos) * scaleV;
	}
}

void Mesh3D::ComputeAvarageEdgeLength(void)
{
	if(!isValid())
	{
		average_edge_length_ = 0.f;
		return;
	}
	float aveEdgeLength = 0.f;
	for (int i=0; i<num_of_half_edges_list(); i++)
	{
		HE_edge* edge = get_edges_list()->at(i);
		HE_vert* v0 = edge->pvert_;
		HE_vert* v1 = edge->ppair_->pvert_;
		aveEdgeLength += (v0->position() - v1->position()).length();
	}
	average_edge_length_ = aveEdgeLength/num_of_half_edges_list();
	//std::cout << "Average_edge_length = " << average_edge_length_ << "\n";
}

HE_face* Mesh3D::get_face(int vId0, int vId1, int vId2)
{
	HE_vert *v0 = get_vertex(vId0);
	HE_vert *v1 = get_vertex(vId1);
	HE_vert *v2 = get_vertex(vId2);
	if (!v0 || !v1 || !v2)
	{
		return NULL;
	}

	HE_face* face=NULL;

	// 由于对边界点的邻域遍历有bug，所以找到非边界点进行邻域遍历
	if (v0->isOnBoundary())
	{
		if (!v1->isOnBoundary())
		{
			SWAP(v0, v1, HE_vert*);
		}
		else if (!v2->isOnBoundary())
		{
			SWAP(v0, v2, HE_vert*);
		}
		else
		{
			// v0, v1, v2 都是边界点
			// 暂时先不处理
			return NULL;
		}
	}

	if (!v0->isOnBoundary())	// 对边界点的遍历有bug
	{
		HE_edge* edge=v0->pedge_;
		bool inFace = true;
		do 
		{
			bool b1 = isFaceContainVertex(edge->pface_, v1);
			bool b2 = isFaceContainVertex(edge->pface_, v2);
			if (!b1 && !b1)
			{
				edge = edge->ppair_->pnext_;
			}
			else if(b1 && b2)
			{
				face = edge->pface_;
				break;
			}
			else
			{
				inFace = false;
				break;
			}
		} while (edge!=v0->pedge_ && edge!=NULL);
	}

	return face;
}

HE_face* Mesh3D::get_face(const std::vector<unsigned int>& ids)
{
	if (ids.size()<3)
	{
		std::cout << "查询点数过少，无法返回面\n";
		return NULL;
	}
	// 首先找到一个非边界点
	HE_vert* v = NULL;
	for (unsigned int i=0; i<ids.size(); i++)
	{
		if (!get_vertex(ids[i])->isOnBoundary())
		{
			v = get_vertex(ids[i]);
			break;
		}
	}
	if (!v)
	{
		// 所有点都是边界点
		// 暂不处理
		return NULL;
	}

	HE_edge *edge = v->pedge_;
	HE_face *face = NULL;
	do 
	{
		face = edge->pface_;
		edge = edge->ppair_->pnext_;
		bool bInFace = isFaceContainVertex(face, get_vertex(ids[0]));
		if (!bInFace)
		{
			continue;
		}
		for (unsigned int i=1; i<ids.size(); i++)
		{
			bool b = isFaceContainVertex(face, get_vertex(ids[i]));
			if (b!=bInFace)
			{
				bInFace = false;
				break;
			}
		}
		if (bInFace)
		{
			return face;
		}
	} while (edge!=v->pedge_ && edge!=NULL);
	return NULL;
}

bool Mesh3D::isFaceContainVertex(HE_face* face, HE_vert* vert)
{
	HE_edge* edge = face->pedge_;
	do 
	{
		if (edge->pvert_==vert)
		{
			return true;
		}
		edge = edge->pnext_;
	} while (edge!=face->pedge_ && edge!=NULL);
	return false;
}

int Mesh3D::GetFaceId(HE_face* face)
{
	return !face ? -1 : face->id();
}

void Mesh3D::ResetFaceSelectedTags(int tag)
{
	for (int i=0; i<num_of_face_list(); i++)
	{
		get_face(i)->set_selected(tag);
	}
}

void Mesh3D::ResetVertexSelectedTags(int tag)
{
	for (int i=0; i<num_of_vertex_list(); i++)
	{
		get_vertex(i)->set_seleted(tag);
	}
}

bool Mesh3D::isNeighbors(HE_vert* v0, HE_vert* v1)
{
	if (!v0 || !v1)
	{
		return false;
	}

	HE_edge *edge = v0->pedge_;
	do 
	{
		if (edge->pvert_==v1)
		{
			return true;
		}
		edge = edge->ppair_->pnext_;
	} while (edge!=v0->pedge_ && edge);
	return false;
}

int Mesh3D::GetSelectedVrtId()
{
	if (!isValid())
	{
		return -1;
	}
	for (int i=0; i<num_of_vertex_list(); i++)
	{
		if (get_vertex(i)->selected()==SELECTED)
		{
			return i;
		}
	}
	return -1;
}

void Mesh3D::CreateMesh(const std::vector<Vec3f>& verts, const std::vector<int>& triIdx)
{
	ClearData();
	for (unsigned int i=0; i<verts.size(); i++)
	{
		InsertVertex(verts[i]);
	}
	for (unsigned int i=0; i<triIdx.size(); i=i+3)
	{
		std::vector<HE_vert*> tri;
		HE_vert *v0 = get_vertex(triIdx[i]);
		HE_vert *v1 = get_vertex(triIdx[i+1]);
		HE_vert *v2 = get_vertex(triIdx[i+2]);
		if (!v0 || !v1 || !v2) continue;
		tri.push_back(v0);
		tri.push_back(v1);
		tri.push_back(v2);
		InsertFace(tri);
	}
	UpdateMesh();
}

void Mesh3D::CreateMesh(const std::vector<double>& verts, const std::vector<unsigned>& triIdx)
{
	ClearData();
	for (unsigned int i=0; i<verts.size(); i=i+3)
	{
		InsertVertex(Vec3f(verts[i], verts[i+1], verts[i+2]));
	}
	for (unsigned int i=0; i<triIdx.size(); i=i+3)
	{
		std::vector<HE_vert*> tri;
		HE_vert *v0 = get_vertex(triIdx[i]);
		HE_vert *v1 = get_vertex(triIdx[i+1]);
		HE_vert *v2 = get_vertex(triIdx[i+2]);
		if (!v0 || !v1 || !v2) continue;
		tri.push_back(v0);
		tri.push_back(v1);
		tri.push_back(v2);
		InsertFace(tri);
	}
	UpdateMesh();
}

int Mesh3D::GetBoundaryVrtSize()
{
	int count = 0;
	for (int i=0; i<num_of_vertex_list(); i++)
	{
		if (get_vertex(i)->isOnBoundary())
		{
			count ++;
		}
	}
	return count;
}

Mesh3D::~Mesh3D(void)
{
	ClearData();
}
