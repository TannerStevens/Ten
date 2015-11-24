#ifndef VP_H
#define VP_H

#include <gl/freeglut.h> 

class VP{
private:
	GLint x, y;
	GLsizei w, h;
	GLdouble modelview[16], projection[16];
public:
	void(*display)(void) = NULL;
	void(*mouse)(int, int, int, int) = NULL;
	void(*keyboard)(unsigned char key, int x, int y) = NULL;

	VP();
	VP(GLint x, GLint y, GLsizei w, GLsizei h);

	void setX(GLint x); GLint getX();
	void setY(GLint y); GLint getY();
	void setW(GLsizei w); GLsizei getW();
	void setH(GLsizei h); GLsizei getH();
	int* getV();

	void updateModelView();
	GLdouble* getModelView();
	void updateProjection();
	GLdouble* getProjection();

	void activate();
};

#endif