#ifndef Edhe_h
#define Edhe_h

#include "Point.h"

class Edge
{
public:
	Point *	begin;
	Point *	end;
	Point *	direction; // directional vector
	Point *	_left;  // pointer to Voronoi place on the _left side of edge 
	Point *	_right; // pointer to Voronoi place on the _right side of edge
	Edge * neighbour; //edges can consist of two parts, neighbour is the pointer to another part (connecting in the end of algorithm)

					  //edge lies on the line: y = k*x + b;
	double	k;
	double	b;

	Edge(Point * start1, Point * left1, Point * right1)
	{
		begin = start1;
		_left = left1;
		_right = right1;
		neighbour = 0;
		end = 0;

		k = (right1->x - left1->x) / (left1->y - right1->y);
		b = start1->y - k * start1->x;
		direction = new Point(right1->y - left1->y, -(right1->x - left1->x));
	}
};

#endif