#include "structures&functions.h"

point p0;

point nextToTop(stack<point> &S)
{
	point p = S.top();
	S.pop();
	point res = S.top();
	S.push(p);
	return res;
}

void swap(point &p1, point &p2)
{
	point temp = p1;
	p1 = p2;
	p2 = temp;
}

int dist(point p1, point p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

int orientation(point p, point q, point r)
{
	int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

	if (val == 0)
		return 0; // colinear
	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// function for sorting
int compare(const void *vp1, const void *vp2)
{
	point *p1 = (point *)vp1;
	point *p2 = (point *)vp2;

	// Find orientation
	int or = orientation(p0, *p1, *p2);
	if (or == 0)
		return (dist(p0, *p2) >= dist(p0, *p1)) ? -1 : 1;

	return (or == 2) ? -1 : 1;
}

vector<point> GrahamHull(vector<point>& points)
{
	int Ymin = points[0].y, min = 0;
	for (int i = 1; i < points.size(); i++)
	{
		double y = points[i].y;

		// Pick the bottom-most or chose the left most point in case of tie
		if ((y < Ymin) || (Ymin == y && points[i].x < points[min].x))
			Ymin = points[i].y, min = i;
	}

	// Place the bottom-most point at first position
	swap(points[0], points[min]);

	// Sort n-1 points with respect to the first point.  A point p1 comes
	// before p2 in sorted ouput if p2 has larger polar angle (in
	// counterclockwise direction) than p1
	p0 = points[0];
	qsort(&points[1], points.size() - 1, sizeof(point), compare);

	// Create an empty stack and push first three points to it.
	stack<point> S;
	S.push(points[0]);
	S.push(points[1]);
	S.push(points[2]);

	// Process remaining n-3 points
	for (int i = 3; i < points.size(); i++)
	{
		// Keep removing top while the angle formed by points next-to-top,
		// top, and points[i] makes a non-left turn
		while (orientation(nextToTop(S), S.top(), points[i]) != 2)
			S.pop();
		S.push(points[i]);
	}

	vector<point> result;
	while (!S.empty())
	{
		point p = S.top();
		result.push_back(p);
		S.pop();
	}
	return result;
}