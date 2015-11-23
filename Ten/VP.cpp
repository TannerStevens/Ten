#include "VP_H.h"

VP::VP(){}
VP::VP(GLint x, GLint y, GLsizei w, GLsizei h){}

void VP::setX(GLint x){ this->x = x; }
GLint VP::getX(){ return x; }
void VP::setY(GLint y){ this->y = y; }
GLint VP::getY(){ return y; }
void VP::setW(GLsizei w){ this->w = w; }
GLsizei VP::getW(){ return w; }
void VP::setH(GLsizei h){ this->h = h; }
GLsizei VP::getH(){ return h; }
int* VP::getV(){
	int* v = (int *)calloc(4, sizeof(int));
	v[0] = this->x;
	v[1] = this->y;
	v[2] = this->w;
	v[3] = this->h;
	return v;
}

void VP::updateModelView(){
	glGetDoublev(GL_MODELVIEW_MATRIX, this->modelview); //get the modelview info
}
void VP::updateProjection(){
	glGetDoublev(GL_PROJECTION_MATRIX, this->projection); //get the projection matrix info
}

void VP::activate(){ 
	glViewport(this->x, this->y, this->w, this->h); 
	if (display){
		if (modelview || projection){
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadMatrixd(projection);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadMatrixd(modelview);

			display();

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
		else display();
	}
}