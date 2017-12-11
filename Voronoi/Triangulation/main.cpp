#include "structures&functions.h"
#include "GL\glut.h"
#include "Point.h"
#include <list>
#include <time.h>

typedef vector<point*> Vertices;
Vertices *ver;
vector<point> points;
vector<point> convex_points;
double w = 10;

void draw()
{
	for (Vertices::iterator i = ver->begin(); i != ver->end(); ++i)
	{
		glBegin(GL_QUADS);
		glVertex2f(-1 + 2 * (*i)->x / w - 0.01, -1 + 2 * (*i)->y / w - 0.01);
		glVertex2f(-1 + 2 * (*i)->x / w + 0.01, -1 + 2 * (*i)->y / w - 0.01);
		glVertex2f(-1 + 2 * (*i)->x / w + 0.01, -1 + 2 * (*i)->y / w + 0.01);
		glVertex2f(-1 + 2 * (*i)->x / w - 0.01, -1 + 2 * (*i)->y / w + 0.01);
		glEnd();
	}

	for (auto i = convex_points.begin(); i != convex_points.end() - 1; i++)
	{
		glBegin(GL_LINES);
		glVertex2f(-1 + 2 * (*i).x / w, -1 + 2 * (*i).y / w);
		glVertex2f(-1 + 2 * (*(i + 1)).x / w, -1 + 2 * (*(i + 1)).y / w);
	}
	auto i = convex_points.begin();
	auto j = convex_points.end() - 1;
	glVertex2f(-1 + 2 * (*i).x / w, -1 + 2 * (*i).y / w);
	glVertex2f(-1 + 2 * (*j).x / w, -1 + 2 * (*j).y / w);
	glEnd();

	glutSwapBuffers();
}


int main(int argc, char **argv)
{
	ver = new Vertices();
	srand(time(NULL));

	for (int i = 0; i < 10; i++)
		ver->push_back(new point(w * (double)rand() / (double)RAND_MAX, w * (double)rand() / (double)RAND_MAX));
	
	cout << "\tVertices has been generated. Pls choose method to build linear shell: \n";
	cout << "\tPress 1 to use Andrew's and Jarvis' method\n";
	cout << "\tPress 2 to use Graham method\n";
	cout << "\tPress 3 to use recursive method\n";

	glutInit(&argc, argv); //glut's Initialization
	glutInitDisplayMode(GLUT_SINGLE); // display mode
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 20);

	int j = 0;
	for (Vertices::iterator i = (*ver).begin(); i != (*ver).end(); i++) {
		j++;
		points.push_back(*(*i));
	}

	int choice;
	cin >> choice;
	switch (choice) {
	case 1: glutCreateWindow("Andrew's and Jarvis' method"); 
		cout << "\tAndrew's and Jarvis' algorithm of the linear shell\n";
		    convex_points = JarvisHull(points);
			break;
	case 2: glutCreateWindow("Graham method"); 
		cout << "\tGraham's algorithm of the linear shell\n";
		convex_points = GrahamHull(points);
		break;
	case 3: glutCreateWindow("Recursive method"); 
		cout << "\tRecursive' algorithm of the linear shell\n";
		convex_points = QuickHull(points); 
		break;
	default: throw (bad_alloc()); break;
	}

	cout << "\tCoordinates of edges:\n";
	for (auto it = convex_points.begin(); it != convex_points.end(); it++)
	{
		cout << "(" << (*it).x << ", " << (*it).y << ")" << endl;
	}

	glutDisplayFunc(draw); // Tell GLUT to use the method "display" for rendering
	glutMainLoop(); // Enter GLUT's main loop

	system("pause");
	return 0;
}