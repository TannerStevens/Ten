#include "TetrisSim_H.h"


void main(int argc, char** argv){
	TetrisSim TS = *new TetrisSim(10, 10);
	TS.addPiece(0, 0, 0);
	TS.addPiece(0, 0, 0);
	TS.addPiece(0, 0, 1);
	TS.addPiece(0, 0, 2);
	TS.addPiece(4, 0, 5);
	TS.addPiece(4, 0, 5);

	getchar(); getchar();
}