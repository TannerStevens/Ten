#ifndef VP_H
#define VP_H

#include <gl/freeglut.h> 

class VP{
private:
	GLint x, y;
	GLsizei w, h;
	GLdouble modelview[16], projection[16];

	void (*display)(void) = NULL;
public:
	VP();
	VP(GLint x, GLint y, GLsizei w, GLsizei h);

	void setX(GLint x); GLint getX();
	void setY(GLint y); GLint getY();
	void setW(GLsizei w); GLsizei getW();
	void setH(GLsizei h); GLsizei getH();
	int* getV();

	void updateModelView();
	void updateProjection();
	void setDisplayFunc(void(*displayFunc)(void));

	void activate();
};

#endif