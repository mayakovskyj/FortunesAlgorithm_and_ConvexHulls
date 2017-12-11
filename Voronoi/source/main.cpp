#include "GL\glut.h"
#include <time.h>
#include "Voronoi.h"

double w = 1000;
Voronoi * v;
Vertices *ver;
Vertices *dir;
Edges *edg;
int choice;

void drawVoronoiOrDelaunay()
{
	for (Vertices::iterator i = ver->begin(); i != ver->end(); ++i)
	{
		glBegin(GL_QUADS);
		cout <<(*i)->x<< " " <<  (*i)->y << endl;
		glVertex2f(-1 + 2 * (*i)->x / w - 0.01, -1 + 2 * (*i)->y / w - 0.01);
		glVertex2f(-1 + 2 * (*i)->x / w + 0.01, -1 + 2 * (*i)->y / w - 0.01);
		glVertex2f(-1 + 2 * (*i)->x / w + 0.01, -1 + 2 * (*i)->y / w + 0.01);
		glVertex2f(-1 + 2 * (*i)->x / w - 0.01, -1 + 2 * (*i)->y / w + 0.01);
		glEnd();
	}

	if (choice == 1)
		for (Edges::iterator i = edg->begin(); i != edg->end(); ++i)
		{
			glBegin(GL_LINES);
			glVertex2f(-1 + 2 * (*i)->begin->x / w, -1 + 2 * (*i)->begin->y / w);
			glVertex2f(-1 + 2 * (*i)->end->x / w, -1 + 2 * (*i)->end->y / w);
			glEnd();
		}
	else 
		for (Edges::iterator i = edg->begin(); i != edg->end(); ++i)
		{
			glBegin(GL_LINES);
			glVertex2f(-1 + 2 * (*i)->_left->x / w, -1 + 2 * (*i)->_left->y / w);
			glVertex2f(-1 + 2 * (*i)->_right->x / w, -1 + 2 * (*i)->_right->y / w);
			glEnd();
		}
	glutSwapBuffers();
}


int main(int argc, char **argv)
{
	try {
		v = new Voronoi();
		ver = new Vertices();
		dir = new Vertices();

		srand(time(NULL));

		for (int i = 0; i < 100; i++)
		{
			ver->push_back(new Point(w * (double)rand() / (double)RAND_MAX, w * (double)rand() / (double)RAND_MAX));
			dir->push_back(new Point((double)rand() / (double)RAND_MAX - 0.5, (double)rand() / (double)RAND_MAX - 0.5));
		}

		/*
		// We've got container of edges of Voronoi's diagramm. To draw Voronoi's diagramm you need:
		// 1) To draw generated points
		// 2) To draw edges, starting from *begin pointer and finishing on *end pointer
		// ------------------------------------------------------------------------------------------
		// To draw Delaunay triangulation you need:
		// 1) To draw generated points
		// 2) To draw triangles' sides. starting from *_left pointer and finishing on *_right pointer
		*/

		edg = v->CreateEdges(ver, w, w);
		cout << "\tVertices and edges are generated! What would you like to draw?\n";
		cout << "\tPress 1 to draw Voronoi's diagramm\n";
		cout << "\tPress 2 to draw Delaunay triangulation\n";

		glutInit(&argc, argv); //glut's Initialization
		glutInitDisplayMode(GLUT_SINGLE); // display mode
		glutInitWindowSize(600, 600);
		glutInitWindowPosition(200, 20);
		
		cin >> choice;
		switch (choice) {
		case 1: glutCreateWindow("Voronoi's diagramm"); break;
		case 2: glutCreateWindow("Delaunay triangulation"); break;
		default: throw (bad_alloc()); break;
		}

		glutDisplayFunc(drawVoronoiOrDelaunay); // Tell GLUT to use the method "display" for rendering
		glutMainLoop(); // Enter GLUT's main loop

		system("pause");
		return 0;
	}
	catch (bad_alloc) {
		cout << "Wrong input! Try again\n"; system("pause"); return 0;
	}
}