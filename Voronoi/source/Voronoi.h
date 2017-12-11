#ifndef Voronoi_h
#define Voronoi_h

#include <list>
#include <queue>
#include <set>

#include "Event.h"

using std::list;
using std::set;
using std::priority_queue;

typedef list<Point*> Vertices;  //container of vertices
typedef list<Edge*> Edges;  //container of edges

class Voronoi
{
public:
	Voronoi() { edges = 0; }
	Edges*	CreateEdges(Vertices * vert, int w, int h); // A function for generating a diagram

private:
	Vertices*		places;
	Edges*			edges;
	double			width, height;
	Parabola*		root;
	double			beach_y; //current "y" position of the beach line;

	std::priority_queue<Event *, std::vector<Event *>, Event::CompareEvent> queue; //events
	std::list<Point*>	points; //all new points created in algorithm
	std::set<Event*>	deleted; // deleted(false) events (thus we can't delete from queue;

	void		InsertParabola(Point * p);  // place event
	void		RemoveParabola(Event * e);  // circle event
	void		FinishEdge(Parabola * n);   // finishes all infinre edges in the tree
	double		GetXOfEdge(Parabola * par, double y); // intersection(edge) of _left and _right parabolas 
	Parabola*	GetParabolaByX(double xx); // returns parabola that is under "x" point in the beachline
	double		GetY(Point * p, double x); // y coordinate of parabola in point x
	void		CheckCircle(Parabola * b);
	Point*		GetEdgeIntersection(Edge * a, Edge * b);
};

#endif 