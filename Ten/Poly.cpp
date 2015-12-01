#include "Poly_H.h"

Poly::Poly(){
	x = 0; y = 0; z = 0; w = 1; l = 1; h = 1; b = 1;
}
Poly::Poly(GLfloat x, GLfloat y, GLfloat z){
	this->x = x; this->y = y; this->z = z;
	w = 1; l = 1; h = 1; b = 1;
}
void Poly::setPos(GLfloat x, GLfloat y, GLfloat z){
	this->x = x; this->y = y; this->z = z;
}
GLfloat* Poly::getPos(){
	GLfloat* r = (GLfloat*)calloc(3, sizeof(GLfloat));
	r[0] = x;
	r[1] = y;
	r[2] = z;
	return r;
}
GLfloat* Poly::getCPos(){
	GLfloat* r = (GLfloat*)calloc(3, sizeof(GLfloat));
	GLfloat pX = x + b, pY = y + b;
	r[0] = (pX + pX + w) / 2;
	r[1] = (pY + pY + l) / 2;
	r[2] = z;
	return r;
}
GLfloat Poly::getRX(){ return x + w + 2 * b; }
GLfloat Poly::getTY(){ return y + l + 2 * b; }
GLfloat Poly::getBZ(){ return z; }

void Poly::setAttribs(GLfloat x, GLfloat y, GLfloat z, int aScore, int hScore, int bScore){
	this->x = x; this->y = y; this->z = z;
	aScore2area(aScore);
	hScore2height(hScore);
	bScore2border(bScore);
}
void Poly::setMaterials(int rGene, int pGene){
	Random r = Random(rGene);
	Random p = Random(pGene);
	this->kAmb[0] = r.NextDouble(0, 1);
	this->kAmb[1] = r.NextDouble(0, 1);
	this->kAmb[2] = r.NextDouble(0, 1);
	this->kAmb[3] = 1;

	this->kDif[0] = p.NextDouble(0, 1);
	this->kDif[1] = p.NextDouble(0, 1);
	this->kDif[2] = p.NextDouble(0, 1);
	this->kDif[3] = 1;

	this->kSpec[0] = r.NextDouble(0, .5) + p.NextDouble(0, .5);
	this->kSpec[1] = r.NextDouble(0, .5) + p.NextDouble(0, .5);
	this->kSpec[2] = r.NextDouble(0, .5) + p.NextDouble(0, .5);
	this->kSpec[3] = 1;

	this->kE[0] = r.NextDouble(0, 1) * p.NextDouble(0, 1);
	this->kE[1] = r.NextDouble(0, 1) * p.NextDouble(0, 1);
	this->kE[2] = r.NextDouble(0, 1) * p.NextDouble(0, 1);
	this->kE[3] = 1;

	this->kSh = p.Next(128);
}
void Poly::aScore2area(int aScore){
	GLfloat s = 0;
	if (aScore == 0) s = 1;
	else s = sqrt((float)aScore);
	this->w = s;
	this->l = s;
}
void Poly::hScore2height(int hScore){
	this->h = hScore;
}
void Poly::bScore2border(int bScore){
	this->b = bScore;
}
void Poly::drawPoly(){
	//Draw Border
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glBegin(GL_LINE_LOOP);
	glColor4fv(kE);
	glVertex3f(x, y, z);//Lower Left
	glVertex3f(x + w + (2 * b), y, z);//Lower Right
	glVertex3f(x + w + (2 * b), y + l + (2 * b), z);//Upper Right
	glVertex3f(x, y + l + (2 * b), z);//Upper Left
	glEnd();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, kAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, kDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, kSpec);
	glMaterialfv(GL_FRONT, GL_EMISSION, kE);
	glMaterialf(GL_FRONT, GL_SHININESS, kSh);

	//Draw Poly
	GLfloat pX = x + b, pY = y + b;
	GLfloat cX = (pX + pX + w) / 2;
	GLfloat cY = (pY + pY + l) / 2;

	glBegin(GL_TRIANGLES);
	glColor3f(.5, .5, 1);
	glVertex3f(pX, pY, z);
	glVertex3f(pX + w, pY, z);
	glVertex3f(cX, cY, z - h);

	glVertex3f(pX, pY + l, z);
	glVertex3f(pX, pY, z);
	glVertex3f(cX, cY, z - h);

	glVertex3f(pX + w, pY + l, z);
	glVertex3f(pX, pY + l, z);
	glVertex3f(cX, cY, z - h);

	glVertex3f(pX + w, pY, z);
	glVertex3f(pX + w, pY + l, z);
	glVertex3f(cX, cY, z - h);
	glEnd();
}