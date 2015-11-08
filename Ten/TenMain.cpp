#include "TetrisSim_H.h"
#include <GL/freeglut.h>

TetrisSim TS;

void main(int argc, char** argv){
	TS = *new TetrisSim(10, 10);

	char key = ' ';
	do{
		printf("type rot x:"); key = getchar(); int r = (int)getchar() - 48; int x = (int)getchar() - 48; getchar();
		if (key == '\n' || r == '\n' || x == '\n')continue;
		if (key == '0') TS.addPiece(0, r, x);
		if (key == '1') TS.addPiece(1, r, x);
		if (key == '2') TS.addPiece(2, r, x);
		if (key == '3') TS.addPiece(3, r, x);
		if (key == '4') TS.addPiece(4, r, x);
		if (key == '5') TS.addPiece(5, r, x);
		if (key == '6') TS.addPiece(6, r, x);
	} while (key != 'q');
}