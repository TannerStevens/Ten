#ifndef POLY_H
#define POLY_H

#include <gl/freeglut.h>
#include <math.h>

class Poly{
private:
	GLfloat x, y, z, w, l, h, b;
public:
	Poly();
	Poly(GLfloat x, GLfloat y, GLfloat z);

	void setPos(GLfloat x, GLfloat y, GLfloat z);
	GLfloat* getPos();
	GLfloat* getCPos();
	//Right most x Coordinate
	GLfloat getRX();
	//Top most y Coordinate
	GLfloat getTY();
	//Back most z Coordinate
	GLfloat getBZ();
	void setAttribs(GLfloat x, GLfloat y, GLfloat z, int aScore, int hScore, int bScore);
	void aScore2area(int aScore);
	void hScore2height(int hScore);
	void bScore2border(int bScore);

	void drawPoly();
};

#endif