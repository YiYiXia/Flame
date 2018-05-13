#include"BiTree.h"
BiTree::BiTree()
{
	Nil_ = new TreePoint;
	Nil_->left = NULL;
	Nil_->right = NULL;
	Root_ = Nil_;
}

BiTree::~BiTree()
{

}
void BiTree::Insert(TreePoint &z)
{
	TreePoint* x;
	TreePoint* y;
	y = Nil_;
	x = Root_;
	while (x != Nil_)
	{
		y = x;
		if (z.key<x->key)
		{
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}
	z.p = y;
	if (y == Nil_)
	{
		Root_ = &z;
	}
	else if (z.key<y->key)
	{
		y->left = &z;
	}
	else
	{
		y->right = &z;
	}
	z.left = Nil_;
	z.right = Nil_;
}

void BiTree::PreorderTreeWalk(TreePoint &x)                                                      //œ»–Ú±È¿˙
{
	//cout << "asfsdfsf" << endl;
	if (&x != Nil_)
	{
		
		PreorderTreeWalk(*x.left);
		cout << x.key << endl;
		PreorderTreeWalk(*x.right);
	}
}

TreePoint BiTree::Min()
{
	TreePoint* s;
	s = Root_;
	while (s->left != Nil_)
	{
		s = s->left;
	}
	return *s;
}

TreePoint BiTree::Max()
{
	TreePoint* s;
	s = Root_;
	while (s->right != Nil_)
	{
		s = s->right;
	}
	return *s;
}