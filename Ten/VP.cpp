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

void VP::activate(){ glViewport(this->x, this->y, this->w, this->h); }