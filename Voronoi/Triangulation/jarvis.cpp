#include "structures&functions.h"

double calculateAngle(point a, point b, point c)
{
	double ABx = b.x - a.x;
	double ABy = b.y - a.y;
	double CBx = b.x - c.x;
	double CBy = b.y - c.y;

	double p = ABx * CBx + ABy * CBy;
	double module = sqrt(ABx * ABx + ABy * ABy) * sqrt(CBx * CBx + CBy * CBy);
	// calculating angle
	double cos = p / module;
	return 2 * M_PI - acos(cos);
}

point findSmallestPointAngle(point start, point end, vector<point> sortedPoints)
{
	point closestPoint;
	double smallestAngle = 2 * M_PI;
	for (point &point : sortedPoints)
	{
		double angle = calculateAngle(start, end, point);
		if (angle < smallestAngle)
		{
			smallestAngle = angle;
			closestPoint = point;
		}
	}
	sortedPoints.erase(remove(sortedPoints.begin(), sortedPoints.end(), closestPoint), sortedPoints.end());
	return closestPoint;
}

bool compareYCoordinateSmallest(point a, point b)
{
	if (a.y == b.y) return a.x < b.x;
	return a.y < b.y;
}


vector<point> JarvisHull(vector<point>& points)
{
	if (points.size() < 3) cout << "points should consist of more than 2 points\n";
	vector<point> sortedPoints;
	for (point &point : points)
	{
		sortedPoints.push_back(point);
	}
	sort(sortedPoints.begin(), sortedPoints.end(), compareYCoordinateSmallest);

	point p = sortedPoints.at(0);
	vector<point> edges;

	edges.push_back(p);

	point q = findSmallestPointAngle(p, point(double(p.x + 0.0001), double(p.y)), sortedPoints);
	edges.push_back(q);
	sortedPoints.erase(remove(sortedPoints.begin(), sortedPoints.end(), q), sortedPoints.end());

	while (q != p)
	{
		q = findSmallestPointAngle(edges.at(edges.size() - 2), edges.at(edges.size() - 1), sortedPoints);
		edges.push_back(q);
	}

	return edges;
}