#include "Parabola.h"

Parabola::Parabola()
{
	site = 0;
	isLeaf = false;
	circle_event = 0;
	edge = 0;
	parent = 0;
}

Parabola::Parabola(Point * s)
{
	site = s;
	isLeaf = true;
	circle_event = 0;
	edge = 0;
	parent = 0;
}

Parabola * Parabola::GetLeftLeaf(Parabola * p)
{
	return GetLeftChild(GetLeftParent(p));
}


Parabola * Parabola::GetRightLeaf(Parabola * p)
{
	return GetRightChild(GetRightParent(p));
}

Parabola * Parabola::GetLeftParent(Parabola * p)
{
	Parabola * par = p->parent;
	Parabola * pLast = p;
	while (par->Left() == pLast)
	{
		if (!par->parent) return 0;
		pLast = par;
		par = par->parent;
	}
	return par;
}

Parabola * Parabola::GetRightParent(Parabola * p)
{
	Parabola * par = p->parent;
	Parabola * pLast = p;
	while (par->Right() == pLast)
	{
		if (!par->parent) return 0;
		pLast = par; par = par->parent;
	}
	return par;
}

Parabola * Parabola::GetLeftChild(Parabola * p)
{
	if (!p) return 0;
	Parabola * par = p->Left();
	while (!par->isLeaf) par = par->Right();
	return par;
}

Parabola * Parabola::GetRightChild(Parabola * p)
{
	if (!p) return 0;
	Parabola * par = p->Right();
	while (!par->isLeaf) par = par->Left();
	return par;
}
