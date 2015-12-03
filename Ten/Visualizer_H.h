#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "Genetics_H.h"
#include "Poly_H.h"
#include "Graph_H.h"
#include "Inspector_H.h"
#include <list>
#include <vector>

class Visualizer{
private:
public:
	Poly *polys;
	int x, y, w, h, pauseUpdates, pauseRendering, highscore;
	/*	0-2 xyz shifts
		3-5 xyz rots
		6-8 xyz scaling
	*/
	GLfloat transformations[9];
	GLfloat mx, my, mz;
	Player play;

	Visualizer(int x, int y, int w, int h, Player play);

	void update();
	void updateVP(int x, int y, int w, int h);
	void display(void);
	void keyboard(unsigned char key, int x, int y);
	void mouse(int button, int state, int x, int y);

	void pauseHandler(int r);
};

float rayIntersectsSphere(GLfloat* p, GLfloat* d, GLfloat r, GLfloat* c);

#endif