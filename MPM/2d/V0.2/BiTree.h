#pragma once
#include<iostream>
#include<iomanip>
using namespace std;

struct TreePoint
{
	float key;                                                 //键值
	TreePoint* right;                                          //右节点
	TreePoint* left;                                           //左节点
	TreePoint* p;                                              //前驱
	int num;
};

class BiTree 
{
public:
	BiTree();
	~BiTree();
	void Insert(TreePoint &z);
	TreePoint Min();
	TreePoint Max();
	void PreorderTreeWalk(TreePoint &x);
	TreePoint* Root_;
private:
	TreePoint* Nil_;
};