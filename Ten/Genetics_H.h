#ifndef Genetics_H
#define Genetics_H

#include "TetrisSim_H.h"

class DNA;
class Player;

//Main Driver of the Genetic Algorithm, will be 'slotting' different DNA into player
class Player{
private:
	TetrisSim *ts;
public:
	int genSize, generations;
	DNA *currentGen, *highscore;
	void (*evalCallback)(int) = NULL, (*reprCallback)() = NULL;

	Player(){};
	Player(TetrisSim *t); //"Random" but capped Generation Size
	Player(TetrisSim *t, int n); //n Generation Size

	void evaluate();
	void setEvalCallback(void(*callbackFunc)(int));
	void reproduce();
	void setReprCallback(void(*callbackFunc)());
};

class DNA{
private:
	int score, rotationGene, positionGene;
public:
	DNA();
	DNA(int geneSeed);
	DNA(int rotationGene, int positionGene);

	int getRotationGene();
	int getPositionGene();
	void addToScore(int s);
	int getScore();
	void setScore(int s);
};

#endif