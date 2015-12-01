#ifndef POLY_H
#define POLY_H

#include <gl/freeglut.h>
#include <math.h>
#include "Random.h"

class Poly{
private:
	GLfloat x, y, z, w, l, h, b;
	GLfloat kAmb[4], kDif[4], kSpec[4], kE[4], kSh;
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
	void setMaterials(int rGene, int pGene);
	void aScore2area(int aScore);
	void hScore2height(int hScore);
	void bScore2border(int bScore);

	void drawPoly();
};

#endif