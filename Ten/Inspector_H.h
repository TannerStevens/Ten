#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "TetrisSim_H.h"
#include "Genetics_H.h"
#include <GL/freeglut.h>
#include <vector>

class Inspector{
private:
	int step = -1;
	TetrisSim ts;
public:
	int winID;
	DNA ins;

	Inspector();
	Inspector(DNA cur);

	void display(void);
	void keyboard(unsigned char key, int x, int y);
	void nextStep();
};

void Inspect(DNA cur);
void display_inspector(void);
void keyboard_inspector(unsigned char key, int x, int y);

#endif