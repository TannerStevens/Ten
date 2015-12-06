#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "TetrisSim_H.h"
#include "Genetics_H.h"
#include "OBJLoader_H.h"
#include <GL/freeglut.h>
#include <SOIL\SOIL.h>

class Inspector{
private:
public:
	int winID, step = -1, *lBoard, *pieces, pieceCount;
	GLuint bgTexture;
	TetrisSim ts;
	OBJLoader tpBase;
	DNA ins;

	/*	0-2 xyz shifts
	3-5 xyz rots
	6-8 xyz scaling
	*/
	GLfloat transformations[9];

	Inspector();
	Inspector(TetrisSim ts);

	void display(void);
	void keyboard(unsigned char key, int x, int y);
	void mouse_movement(int x, int y);
	void nextStep();
	void reset();
	void drawTetrisPiece(int t, int x, int y, int j);
};

void Inspect(DNA cur);
void initInspector(TetrisSim ts);
void display_inspector(void);
void keyboard_inspector(unsigned char key, int x, int y);
void pMouseMove_inspector(int x, int y);

#endif