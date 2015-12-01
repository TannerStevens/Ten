#ifndef TEN_H
#define TEN_H

#include "TetrisSim_H.h"
#include "Genetics_H.h"
#include <GL/freeglut.h>
#include "Visualizer_H.h"
#include "Graph_H.h"
#include <vector>
#include "Poly_H.h"
#include "Inspector_H.h"

static GLint width, height;
static GLfloat LR = -3.0;



//Redraw the Board
void updatePiece(int c);
void updateBoard();
float rayIntersectsSphere(GLfloat* p, GLfloat* d, GLfloat r, GLfloat* c);
void pauseHandler(int r);

void display(void);
void display_graph(void);
void display_inspection(void);
void keyboard_inspection(unsigned char key, int x, int y);

void mouse(int button, int state, int x, int y);
void mouse_sim(int button, int state, int x, int y);

void keyboard(unsigned char key, int x, int y);
void idle();
void visible(int state);
void reshape(int w, int h);
void other_init();
void init_window(int argc, char** argv);

#endif