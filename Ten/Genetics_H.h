#ifndef Genetics_H
#define Genetics_H

#include "TetrisSim_H.h"

class DNA;
class Player;

//Main Driver of the Genetic Algorithm, will be 'slotting' different DNA into player
class Player{
private:
	TetrisSim ts;

	void calculateAggHeight(int c);
	void calculateNHoles(int c);
public:
	int genSize, generations;
	DNA *currentGen, *highscore, *highWscore;

	Player(){};
	Player(TetrisSim t); //"Random" but capped Generation Size
	Player(TetrisSim t, int n); //n Generation Size

	void evaluate();
	void onlyEvaluate();
	void reproduce();
};

class DNA{
private:
	int score, holes, rotationGene, positionGene;
	float aggHeight, wScore;
public:

	DNA();
	DNA(int geneSeed);
	DNA(int rotationGene, int positionGene);

	int getRotationGene();
	int getPositionGene();
	void addToScore(int s);
	int getScore();
	void setScore(int s);
	int getHoles();
	void setHoles(int h);
	float getAggHeight();
	void setAggHeight(float h);
	float getWScore();
	void setWScore(float wS);
};

#endif