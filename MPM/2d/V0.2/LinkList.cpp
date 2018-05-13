#include"LinkList.h"

LinkList::LinkList()
{
	L = (List)malloc(sizeof(LNode));
	L->next = NULL;
	L->num = NULL;
	ListLength = 0;
}
LinkList::~LinkList()
{

}

void LinkList::AddLNode(int num)
{
	List s;
	s = (List)malloc(sizeof(LNode));
	s->num = num;
	s->next = NULL;
	if (L->next == NULL) L->next = s;
	else 
	{
		s->next = L->next;
		L->next = s;
	}
	ListLength++;
}

void LinkList::Delete(int num)
{
	List p,q;
	if (L->next == NULL) return;
	p = L;
	q = L->next;
	int n = 1;
	while (n!=num)
	{
		p = p->next;
		q = q->next;
		n++;
	}
	p->next = q->next;
	free(q);
	ListLength--;
}

int LinkList::Search(int num)
{
	List  q;
	if (L->next == NULL) return 0;
	q = L->next;
	int n = 1;
	while (n != num)
	{
		q = q->next;
		n++;
	}
	return q->num;
}
void LinkList::Print()
{
	List p;
	if (L->next == NULL) return;
	p = L->next;
	while (p != NULL)
	{
		std::cout << p->num << std::endl;
		p = p->next;
	}
}


bool LinkList::IsEmpty()
{
	if (L->next == NULL) return false;
	else return true;
}