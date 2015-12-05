#include "Genetics_H.h"
#include "Random.h"

Player::Player(TetrisSim t){
	generations = -1;
	nGenNHS = 0;
	ts = t;

	genSize = 100;//Random(time(NULL)).Next(20);
	currentGen = (DNA *)calloc(genSize, sizeof(DNA));
	for (int i = 0; i < genSize; i++){
		currentGen[i] = *new DNA();
	}
	highWscore = &currentGen[0];
	highscore = &currentGen[0];
	/*while (generations==-1 || generations-- > 0){
		evaluate();
	}*/
}

void Player::evaluate(){
	highscore = &currentGen[0];
	for (int c = 0; c < genSize; c++){
		ts.resetSim();
		int result=0;
		Random *r = new Random(currentGen[c].getRotationGene());
		Random *p = new Random(currentGen[c].getPositionGene());
		do{
			int i, j;
			i = 0; j = 0;
			i = r->Next(3);
			j = p->Next(ts.getBoardWidth() - 1);
			result = ts.addPiece(i, j);
			currentGen[c].addToScore(result);
		} while (result != 0);
		if (currentGen[c].getScore() > highscore->getScore())highscore = &currentGen[c];
		delete r;delete p;
		calculateAggHeight(c);
	}
	reproduce();
}

void Player::onlyEvaluate(){
	highWscore = &currentGen[0];
	highscore = &currentGen[0];
	for (int c = 0; c < genSize; c++){
		ts.resetSim();
		int result = 0;
		Random *r = new Random(currentGen[c].getRotationGene());
		Random *p = new Random(currentGen[c].getPositionGene());
		do{
			int i, j;
			i = 0; j = 0;
			i = r->Next(3);
			j = p->Next(ts.getBoardWidth() - 1);
			result = ts.addPiece(i, j);
			currentGen[c].addToScore(result);
		} while (result != 0);
		calculateAggHeight(c);
		calculateNHoles(c);

		float wAggH = currentGen[c].getAggHeight() / (ts.getCurrentPiece() + 1);
		if (!wAggH)wAggH = 1;
		float wScore = (float)currentGen[c].getScore() / wAggH;
		currentGen[c].setWScore(wScore);
		if (currentGen[c].getScore() > highscore->getScore()){ 
			highscore = &currentGen[c]; 
			nGenNHS = 0;
		}
		if (currentGen[c].getWScore() > highscore->getWScore()){
			highWscore = &currentGen[c];
		}
		delete r; delete p;
	}
	nGenNHS++;
}

void Player::reproduce(){
	//Highest Scoring DNA should Persist
	Random *r = new Random(time(NULL));
	currentGen[0] = DNA(highscore->getRotationGene(), highscore->getPositionGene());
	currentGen[1] = DNA(highWscore->getRotationGene(), highWscore->getPositionGene());
	int mutationsize = 1 * nGenNHS;
	for (int i = 2; i < genSize; i++){
		currentGen[i].setScore(0);
		switch (r->Next(1, 7)){
			case 1:
				currentGen[i] = DNA(currentGen[i].getRotationGene()-mutationsize, highscore->getPositionGene()+mutationsize);
				break;
			case 2:
				currentGen[i] = DNA(highscore->getRotationGene()-mutationsize, currentGen[i].getPositionGene()-mutationsize);
				break;
			case 3:
				currentGen[i] = DNA(currentGen[i].getRotationGene()+mutationsize, highWscore->getPositionGene()-mutationsize);
				break;
			case 4:
				currentGen[i] = DNA(highWscore->getRotationGene()+mutationsize, currentGen[i].getPositionGene()+mutationsize);
				break;
			default:
				currentGen[i] = DNA();
				break;
		}
	}
	delete r;
}

void Player::calculateAggHeight(int c){
	int *board = ts.getBoardState();
	int bW = ts.getBoardWidth(), bH = ts.getBoardHeight();
	float aggHeight = 0;
	for (int i = 0; i < bW; i++){
		for (int j = 0; j < bH; j++){
			if (board[j*bW + i]){
				aggHeight += bH - j;
				break;
			}
		}
	}
	delete board;

	currentGen[c].setAggHeight(aggHeight);
}

void Player::calculateNHoles(int c){
	int *board = ts.getBoardState();
	int bW = ts.getBoardWidth(), bH = ts.getBoardHeight();
	int holes = 0;
	for (int i = 0; i < bW; i++){
		for (int j = 0; j < bH; j++){
			if (!board[j*bW + i] && board[j+1*bW + i]){
				holes++;
			}
		}
	}
	delete board;

	holes = ceil(holes/(ts.getCurrentPiece()+1));

	currentGen[c].setHoles(holes);
}

DNA::DNA(){ 
	Random *r = new Random(); 
	rotationGene = r->Next(); 
	positionGene = r->Next(); 
	score = 0; 
	delete r; 
}
DNA::DNA(int geneSeed){ 
	Random *r = new Random(geneSeed); 
	rotationGene = r->Next(); 
	positionGene = r->Next(); 
	score = 0; 
	delete r; 
}
DNA::DNA(int rotationGene, int positionGene){ 
	this->rotationGene = rotationGene; 
	this->positionGene = positionGene; 
	score = 0; 
}
int DNA::getRotationGene(){ return rotationGene; }
int DNA::getPositionGene(){ return positionGene; }
void DNA::addToScore(int s){ score += s; }
int DNA::getScore(){ return score; }
void DNA::setScore(int s){ score = s; }
float DNA::getAggHeight(){ return aggHeight; }
void DNA::setAggHeight(float h){ aggHeight = h; }
int DNA::getHoles(){ return holes; }
void DNA::setHoles(int h){ holes = h; }
float DNA::getWScore(){ return wScore; }
void DNA::setWScore(float wS){ wScore = wS; }