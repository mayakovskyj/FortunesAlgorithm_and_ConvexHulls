#ifndef Point_h
#define Point_h
#include <iostream>

using namespace std;

struct Point
{
public:
	Point(double nx, double ny) : x(nx), y(ny) {}
	void write() {
		cout << "x: " << x << " y: " << y << endl;
	}
public:
	double x, y;
};

#endif