#ifndef Genetics_H
#define Genetics_H

#include "TetrisSim_H.h"

//Main Driver of the Genetic Algorithm, will be 'slotting' different DNA into player
class Player{
private:
	TetrisSim *ts;
	int *po;
public:
	Player(TetrisSim *t);

	void getPieceOrder();
};

class DNA{
private:
	int gene;
public:
	DNA(int gSeed);
};

#endif