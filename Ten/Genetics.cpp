#include "Genetics_H.h"
#include "Random.h"

Player::Player(TetrisSim *t){
	generations = 5000;
	ts = t;

	genSize = 20;//Random(time(NULL)).Next(20);
	currentGen = (DNA *)calloc(genSize, sizeof(DNA));
	for (int i = 0; i < genSize; i++){
		currentGen[i] = *new DNA();
	}
	while (generations-- > 0){
		evaluate();
	}
}

void Player::evaluate(){
	DNA *highscore = &currentGen[0];
	for (int c = 0; c < genSize; c++){
		ts->resetSim();
		int result=0;
		Random r = *new Random(currentGen[c].getRotationGene());
		Random p = *new Random(currentGen[c].getPositionGene());
		do{
			int i, j;
			i = r.Next(3);
			j = p.Next(ts->getBoardWidth() - 1);
			result = ts->addPiece(i, j);
			currentGen[c].addToScore(result);
			printf("\n %i", currentGen[c].getScore());
		} while (result != 0);
		if (currentGen[c].getScore() > highscore->getScore())highscore = &currentGen[c];
		printf("\n----------------\n%i\n----------------\n", currentGen[c].getScore());
	}
	printf("\nHighest Gen Score: %i\n", highscore->getScore());
	reproduce(highscore);
}

void Player::reproduce(DNA *hs){
	//Highest Scoring DNA should Persist
	Random r = *new Random();
	for (int i = 0; i < genSize; i++){
		currentGen[i].setScore(0);
		if (&currentGen[i] != hs){
			switch (r.Next(0, 5)){
				case 1:
					currentGen[i] = *new DNA(hs->getPositionGene(), currentGen[i].getRotationGene());
				case 2:
					currentGen[i] = *new DNA(currentGen[i].getRotationGene(), hs->getPositionGene());
				case 3:
					currentGen[i] = *new DNA(currentGen[i].getRotationGene() + currentGen[i].getPositionGene());
				case 4:
					currentGen[i] = *new DNA(hs->getRotationGene() + hs->getPositionGene());
				case 5:
					currentGen[i] = *new DNA();
			}
		}
	}
}

DNA::DNA(){ Random r = *new Random(); rotationGene = r.Next(); positionGene = r.Next(); score = 0; }
DNA::DNA(int geneSeed){ Random r = *new Random(geneSeed); rotationGene = r.Next(); positionGene = r.Next(); score = 0; }
DNA::DNA(int rotationGene, int positionGene){ this->rotationGene = rotationGene; this->positionGene = positionGene; score = 0; }
int DNA::getRotationGene(){ return rotationGene; }
int DNA::getPositionGene(){ return positionGene; }
void DNA::addToScore(int s){ score += s; }
int DNA::getScore(){ return score; }
void DNA::setScore(int s){ score = s; }