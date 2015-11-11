#include "Genetics_H.h"
#include "Random.h"

Player::Player(TetrisSim *t){
	generations = -1;
	ts = t;

	genSize = 20;//Random(time(NULL)).Next(20);
	currentGen = (DNA *)calloc(genSize, sizeof(DNA));
	for (int i = 0; i < genSize; i++){
		currentGen[i] = *new DNA();
	}
	while (generations==-1 || generations-- > 0){
		evaluate();
	}
}

void Player::evaluate(){
	highscore = &currentGen[0];
	for (int c = 0; c < genSize; c++){
		ts->resetSim();
		int result=0;
		Random *r = new Random(currentGen[c].getRotationGene());
		Random *p = new Random(currentGen[c].getPositionGene());
		do{
			int i, j;
			i = 0; j = 0;
			i = r->Next(3);
			j = p->Next(ts->getBoardWidth() - 1);
			result = ts->addPiece(i, j);
			currentGen[c].addToScore(result);
		} while (result != 0);
		if (currentGen[c].getScore() > highscore->getScore())highscore = &currentGen[c];
		delete r;delete p;
	}
	printf("\nHighest Gen Score: %i\n", highscore->getScore());
	reproduce();
}

void Player::reproduce(){
	//Highest Scoring DNA should Persist
	Random *r = new Random(time(NULL));
	for (int i = 0; i < genSize; i++){
		currentGen[i].setScore(0);
		if (&currentGen[i] != highscore){
			switch (r->Next(1, 5)){
				case 1:
					currentGen[i] = DNA(highscore->getPositionGene(), currentGen[i].getRotationGene());
					break;
				case 2:
					currentGen[i] = DNA(currentGen[i].getRotationGene(), highscore->getPositionGene());
					break;
				case 3:
					currentGen[i] = DNA(currentGen[i].getRotationGene() + currentGen[i].getPositionGene());
					break;
				case 4:
					currentGen[i] = DNA(highscore->getRotationGene() + highscore->getPositionGene());
					break;
				case 5:
					currentGen[i] = DNA();
					break;
			}
		}
	}
	delete r;
}

DNA::DNA(){ Random *r = new Random(); rotationGene = r->Next(); positionGene = r->Next(); score = 0; delete r; }
DNA::DNA(int geneSeed){ Random *r = new Random(geneSeed); rotationGene = r->Next(); positionGene = r->Next(); score = 0; delete r; }
DNA::DNA(int rotationGene, int positionGene){ this->rotationGene = rotationGene; this->positionGene = positionGene; score = 0; }
int DNA::getRotationGene(){ return rotationGene; }
int DNA::getPositionGene(){ return positionGene; }
void DNA::addToScore(int s){ score += s; }
int DNA::getScore(){ return score; }
void DNA::setScore(int s){ score = s; }