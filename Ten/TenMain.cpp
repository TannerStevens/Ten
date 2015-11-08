#include "TetrisSim_H.h"
#include <GL/freeglut.h>

TetrisSim TS;

void main(int argc, char** argv){
	TS = *new TetrisSim(10, 10);

	char key = ' ';
	do{
		printf("type rot x:"); key = getchar(); int r = (int)getchar() - 48; int x = (int)getchar() - 48; getchar();
		if (key == '\n' || r == '\n' || x == '\n')continue;
		if (key == '0') printf("\n%i\n", TS.addPiece(0, r, x));
		if (key == '1') printf("\n%i\n", TS.addPiece(1, r, x));
		if (key == '2') printf("\n%i\n", TS.addPiece(2, r, x));
		if (key == '3') printf("\n%i\n", TS.addPiece(3, r, x));
		if (key == '4') printf("\n%i\n", TS.addPiece(4, r, x));
		if (key == '5') printf("\n%i\n", TS.addPiece(5, r, x));
		if (key == '6') printf("\n%i\n", TS.addPiece(6, r, x));
	} while (key != 'q');
	getchar(); getchar();
}