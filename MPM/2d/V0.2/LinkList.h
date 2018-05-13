#pragma once
#include<iostream>
typedef struct LNode {
	int num;
	struct LNode* next;
}LNode, *List;

class LinkList 
{
public:
	LinkList();
	~LinkList();
	int ListLength;
	List L;
	LNode* Head,Nil;
	void AddLNode(int num);
	void Delete(int num);
	int Search(int num);
	void Print();
	bool IsEmpty();
};