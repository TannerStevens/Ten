#ifndef VP_H
#define VP_H

#include <gl/freeglut.h> 

class VP{
private:
	GLint x, y;
	GLsizei w, h;
public:
	VP();
	VP(GLint x, GLint y, GLsizei w, GLsizei h);

	void setX(GLint x); GLint getX();
	void setY(GLint y); GLint getY();
	void setW(GLsizei w); GLsizei getW();
	void setH(GLsizei h); GLsizei getH();

	void activate();
};

#endif