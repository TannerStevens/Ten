#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "TetrisSim_H.h"
#include "Genetics_H.h"
#include <GL/freeglut.h>
#include <vector>

class Inspector{
private:
public:
	int winID, step = -1;
	TetrisSim ts;
	DNA ins;

	Inspector();
	Inspector(TetrisSim ts);

	void display(void);
	void keyboard(unsigned char key, int x, int y);
	void nextStep();
};

void Inspect(DNA cur);
void initInspector(TetrisSim ts);
void display_inspector(void);
void keyboard_inspector(unsigned char key, int x, int y);

#endif