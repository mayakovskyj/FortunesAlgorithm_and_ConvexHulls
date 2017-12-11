#include "structures&functions.h"

bool isLeft(point a, point b, point c) {
	return ((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x)) > 0;
}

double AreaOfTriangle(point &p0, point &p1, point &p2)
{
	double area;
	area = abs((p0.x*(p1.y - p2.y) + p1.x*(p2.y - p0.y) + p2.x*(p0.y - p1.y)) / 2.0);
	return area;
}

point FurthestPoint(vector<point> &S, point &p1, point &p2)
{
	point ret = S[0];
	int remove = 0;
	double curdist = AreaOfTriangle(ret, p1, p2);
	for (int i = 1; i < S.size(); i++) 
	{
		float tmp = AreaOfTriangle(S[i], p1, p2);
		if (tmp > curdist) 
		{
			ret = S[i];
			curdist = tmp;
			remove = i;
		}
	}
	S.erase(S.begin() + remove);

	return ret;
}

point GetMinX(vector<point> &A)
{
	point res = A[0];
	int remove = 0;
	for (int i = 1; i < A.size(); i++)
	{
		if (res.x > A[i].x) 
		{
			res = A[i];
			remove = i;
		}
	}
	A.erase(A.begin() + remove);
	return res;
}

point GetMaxX(vector<point> &A)
{
	point res = point(A[0]);
	int remove = 0;
	for (int i = 1; i < A.size(); i++) 
	{
		if (res.x < A[i].x) 
		{
			res = A[i];
			remove = i;
		}
	}
	A.erase(A.begin() + remove);

	return res;
}

vector<point> QuickHull(vector<point> &S)
{
	point A = GetMinX(S);
	point B = GetMaxX(S);
	vector<point> H;

	H.push_back(A);
	H.push_back(B);

	/*
	*  Segment AB divides the remaining (n-2) points into 2 groups S1 and S2
	*  where S1 are points in S that are on the right side of the oriented line from A to B,
	*  and S2 are points in S that are on the right side of the oriented line from B to A
	*/
	vector<point> S1;
	vector<point> S2;

	for (point& point : S) {
		if (A.y < B.y) {
			if (isLeft(A, B, point)) {
				S2.push_back(point);
			}
			else {
				S1.push_back(point);
			}
		}
		else {
			if (isLeft(A, B, point)) {
				S1.push_back(point);
			}
			else {
				S2.push_back(point);
			}
		}

	}
	FindHull(S1, H, A, B);
	FindHull(S2, H, B, A);

	return H;
}

void FindHull(vector<point> points, vector<point> &H, point &P, point &Q)
{
	vector<point>::iterator it;
	if (points.size() == 0) 
	{
		return;
	}

	// From the given set of points in points, find farthest point from segment PQ 
	point C = FurthestPoint(points, P, Q);

	//  Add point C to convex hull at the location between P and Q
	for (it = H.begin(); it != H.end(); ++it) 
	{
		if (*it == Q) {
			H.insert(it, C);
			break;
		}
	}
	/*
	* Three points P, Q, and C partition the remaining points of Sk into 3 subsets: S0, S1, and S2
	* where S0 are points inside triangle PCQ, S1 are points on the right side of the oriented
	* line from  P to C, and S2 are points on the right side of the oriented line from C to Q.
	*/

	vector<point> S0; //inside
	vector<point> S1; //right
	vector<point> S2; //left

	for (point& point : points) {
		if (P.x < C.x) {
			if (isLeft(C, P, point)) {
				S2.push_back(point);
			}
			else if (isLeft(C, Q, point)) {
				S0.push_back(point);
			}
			else {
				S1.push_back(point);
			}
		}
		else {
			if (isLeft(P, C, point)) {
				S1.push_back(point);
			}
			else if (isLeft(Q, C, point)) {
				S0.push_back(point);
			}
			else {
				S2.push_back(point);
			}
		}
	}
	FindHull(S1, H, P, C);
	FindHull(S2, H, C, Q);
}
