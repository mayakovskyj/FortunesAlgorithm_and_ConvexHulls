#include "Voronoi.h"

Edges * Voronoi::CreateEdges(Vertices * v, int w, int h)
{
	places = v;
	width = w;
	height = h;
	root = 0;

	edges = new Edges();

	//pushing events
	for (Vertices::iterator i = places->begin(); i != places->end(); ++i)
	{
		queue.push(new Event(*i, true));
	}

	Event * e;
	while (!queue.empty())
	{
		e = queue.top();
		queue.pop();
		beach_y = e->point->y;
		if (deleted.find(e) != deleted.end()) { delete(e); deleted.erase(e); continue; }
		if (e->place_event) InsertParabola(e->point);
		else RemoveParabola(e);
		delete(e);
	}

	FinishEdge(root);

	for (Edges::iterator i = edges->begin(); i != edges->end(); ++i)
	{
		if ((*i)->neighbour)
		{
			(*i)->begin = (*i)->neighbour->end;
			delete (*i)->neighbour;
		}
	}

	return edges;
}

// Place event
void Voronoi::InsertParabola(Point * p)
{
	if (!root) { root = new Parabola(p); return; }

	/*
	// tree with only 1 node had only root
	// when adding new parabola ->     root
									/	    \
							parabola      parabola(p)
						 (first_point)
	*/

	if (root->isLeaf && root->site->y - p->y < 1)
	{
		Point * fp = root->site;
		root->isLeaf = false;
		root->SetLeft(new Parabola(fp));
		root->SetRight(new Parabola(p));
		Point * s = new Point((p->x + fp->x) / 2, height); // begin point of the edge
		points.push_back(s);
		//creating edge:
		if (p->x > fp->x) root->edge = new Edge(s, fp, p);
		else root->edge = new Edge(s, p, fp);
		edges->push_back(root->edge);
		return;
	}

	Parabola * par = GetParabolaByX(p->x); //parabola with focus in point p with coordinate x

										   /* if (par has its circle event)
										   remove this event form the queue */
	if (par->circle_event)
	{
		deleted.insert(par->circle_event);
		par->circle_event = 0;
	}

	/* Creating _left and _right edges*/
	Point * begin = new Point(p->x, GetY(par->site, p->x));
	points.push_back(begin);

	Edge * el = new Edge(begin, par->site, p);
	Edge * er = new Edge(begin, p, par->site);

	/* adding new parabola to the tree:

	                   root
	                 /      \
inner_node1(_left edge)      inner_node2(_right edge)
	              /            \
             inner_node3		p2
                        	(_left edge)
                         	/           \
	                       p0            p1

	*/

	el->neighbour = er;
	edges->push_back(el);

	par->edge = er;
	par->isLeaf = false;

	//replace par by the sequence a, xl, b, xr, c
	Parabola * p0 = new Parabola(par->site);
	Parabola * p1 = new Parabola(p);
	Parabola * p2 = new Parabola(par->site);

	par->SetRight(p2);
	par->SetLeft(new Parabola());
	par->Left()->edge = el;

	par->Left()->SetLeft(p0);
	par->Left()->SetRight(p1);

	CheckCircle(p0);
	CheckCircle(p2);
}

/*
//	function of circle event
//
//  1) find an arc lef to p and an arc on the right from p;
//  2) check their Circle events => remove events from the queue, if true
//  3) find the the circumcenter of  l.site, p.site and r.site
//  4) create edge, starts at s, normal to (l.site, r.site)
//  5) finish two neighbour edges xl, xr at point s
//  6) replace xl, p, xr by new edge 
//  7) check circle events of left and right arcks
//
*/
void Voronoi::RemoveParabola(Event * e)
{
	Parabola * p1 = e->arch;

	Parabola * xl = Parabola::GetLeftParent(p1);
	Parabola * xr = Parabola::GetRightParent(p1);

	Parabola * p0 = Parabola::GetLeftChild(xl);
	Parabola * p2 = Parabola::GetRightChild(xr);

	if (p0 == p2) std::cout << "Error -- two parabolas have the same focus!\n";

	if (p0->circle_event) { deleted.insert(p0->circle_event); p0->circle_event = 0; }
	if (p2->circle_event) { deleted.insert(p2->circle_event); p2->circle_event = 0; }

	Point * p = new Point(e->point->x, GetY(p1->site, e->point->x));
	points.push_back(p);

	xl->edge->end = p;
	xr->edge->end = p;

	Parabola * higher;
	Parabola * par = p1;
	while (par != root)
	{
		par = par->parent;
		if (par == xl) higher = xl;
		if (par == xr) higher = xr;
	}
	higher->edge = new Edge(p, p0->site, p2->site);
	edges->push_back(higher->edge);

	Parabola * gparent = p1->parent->parent;
	if (p1->parent->Left() == p1)
	{
		if (gparent->Left() == p1->parent) gparent->SetLeft(p1->parent->Right());
		if (gparent->Right() == p1->parent) gparent->SetRight(p1->parent->Right());
	}
	else
	{
		if (gparent->Left() == p1->parent) gparent->SetLeft(p1->parent->Left());
		if (gparent->Right() == p1->parent) gparent->SetRight(p1->parent->Left());
	}

	delete p1->parent;
	delete p1;

	CheckCircle(p0);
	CheckCircle(p2);
}

void	Voronoi::FinishEdge(Parabola * n)
{
	if (n->isLeaf) { delete n; return; }
	double mx;
	if (n->edge->direction->x > 0.0)	mx = std::max(width, n->edge->begin->x + 10);
	else							mx = std::min(0.0, n->edge->begin->x - 10);

	Point * end = new Point(mx, mx * n->edge->k + n->edge->b);
	n->edge->end = end;
	points.push_back(end);

	FinishEdge(n->Left());
	FinishEdge(n->Right());
	delete n;
}

/*
// Function of searching x coordinate of the edge between two parabolas (_right and _left parabolas from current parabola)
// input: parabola and coordinate of beach line
// body:
// 1) get closest _left and _right parabolas to our parabola, get their focuses
// 2) calculating a1,b1,c1 and a2,b2,c2 -- coefficients of a quadratic parabolas' functions
// 3) find the difference between coefficients (let them be a, b and c)
// 4) find roots of a^2x + bx + c = 0, using discriminant
// 5) chose needed root, depending on the focuses positions
// return root
*/
double	Voronoi::GetXOfEdge(Parabola * par, double y)
{
	Parabola * _left = Parabola::GetLeftChild(par);
	Parabola * _right = Parabola::GetRightChild(par);

	Point * p = _left->site;
	Point * r = _right->site;

	double dp = 2.0 * (p->y - y);
	double a1 = 1.0 / dp;
	double b1 = -2.0 * p->x / dp;
	double c1 = y + dp / 4 + p->x * p->x / dp;

	dp = 2.0 * (r->y - y);
	double a2 = 1.0 / dp;
	double b2 = -2.0 * r->x / dp;
	double c2 = beach_y + dp / 4 + r->x * r->x / dp;

	double a = a1 - a2;
	double b = b1 - b2;
	double c = c1 - c2;

	double disc = b*b - 4 * a * c;
	double x1 = (-b + std::sqrt(disc)) / (2 * a);
	double x2 = (-b - std::sqrt(disc)) / (2 * a);

	double ry;
	if (p->y < r->y) ry = std::max(x1, x2);
	else ry = std::min(x1, x2);

	return ry;
}

Parabola * Voronoi::GetParabolaByX(double xx)
{
	Parabola * par = root;
	double x = 0.0;


	/*
	//	xx -- coordinate of focus
	//  searching the tree, until find _right leaf:
	//	1) get x of edge
	//	2) if edge is to the _right of the center go to _left parabola, else -- to the _right parabola
	//  return needed parabola
	*/

	while (!par->isLeaf)
	{
		x = GetXOfEdge(par, beach_y);
		if (x>xx) par = par->Left();
		else par = par->Right();
	}
	return par;
}

/*
//  Уравнение параболы:
//	y = ((x - xp)^2 +yp^2 - beach_y^2)/(2*(yp - beach_y)) = (x^2 - 2x*xp + xp^2) / (2yp - 2beach_y) + (yp + beach_y)/2
//	y = a*x^2 + bx + c, где:
//	a = 1/2*(yp - beach_y)
//	b = - xp / yp - beach_y
//	c = beach_y + (py - beach_y)/2 + xp^2 = (yp + beach_y)/2 + xp^2
//	где xp, yp — координаты фокуса параболы (центр полигона Вороного);
//	beach_y — положение ЗП (координата Y текущего обрабатываемого события).
*/
double	Voronoi::GetY(Point * p, double x) // focus and x coordinate
{
	double dp = 2 * (p->y - beach_y);
	double a1 = 1 / dp;
	double b1 = -2 * p->x / dp;
	double c1 = beach_y + dp / 4 + p->x * p->x / dp;

	return(a1*x*x + b1*x + c1);
}

/*
//	if parabola has it's circle event -- add it tot the queue
//
// 1) create parabolas on the left to b and to the right;
// 2) create left and right edges
// 3) if no left or right edges or their sites 
// 4) find middle point s
// 5) if (no s) or (edges \  /) => no event
// 6) find distance between s and site of b parabola -- radius
// 7) s.y + r under beach_y => no event
// 8) add new event
//
*/
void Voronoi::CheckCircle(Parabola * b)
{
	Parabola * lp = Parabola::GetLeftParent(b);
	Parabola * rp = Parabola::GetRightParent(b);

	Parabola * a = Parabola::GetLeftChild(lp);
	Parabola * c = Parabola::GetRightChild(rp);

	if (!a || !c || a->site == c->site) return;

	Point * s = 0;
	s = GetEdgeIntersection(lp->edge, rp->edge);
	if (s == 0) return;

	double dx = a->site->x - s->x;
	double dy = a->site->y - s->y;

	double d = std::sqrt((dx * dx) + (dy * dy));

	if (s->y - d >= beach_y) { return; }

	Event * e = new Event(new Point(s->x, s->y - d), false);
	points.push_back(e->point);
	b->circle_event = e;
	e->arch = b;
	queue.push(e);
}

Point * Voronoi::GetEdgeIntersection(Edge * a, Edge * b)
{
	double x = (b->b - a->b) / (a->k - b->k);
	double y = a->k * x + a->b;

	if ((x - a->begin->x) / a->direction->x < 0) return 0;
	if ((y - a->begin->y) / a->direction->y < 0) return 0;

	if ((x - b->begin->x) / b->direction->x < 0) return 0;
	if ((y - b->begin->y) / b->direction->y < 0) return 0;

	Point * p = new Point(x, y);
	points.push_back(p);
	return p;
}


