#include "TetrisSim_H.h"
#include "Genetics_H.h"
#include <GL/freeglut.h>

void main(int argc, char** argv){
	Player play = Player(new TetrisSim(10, 10));
	
	getchar(); getchar();
}