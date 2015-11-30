#include "Visualizer_H.h"

void drawBitmapTexti(int in, float x, float y){
	glColor3f(0, 0, 0);

	char *c, k[5];
	sprintf_s(k, 5,"%i", in);
	glRasterPos2f(x, y);

	for (c = k; *c != '\0'; c++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
	}
}


Visualizer::Visualizer(int x, int y, int w, int h, Player play){
	this->pauseUpdates = false;
	this->pauseRendering = false;
	this->polys = (Poly *)calloc(play.genSize, sizeof(Poly));
	this->x = x; this->y = y; this->w = w; this->h = h;
	this->highscore = 0;
	this->play = play;
}


void Visualizer::addExtra(int type, int stat){
	/*
	Type
		0:Graph
		1:List
	Stat
		0:HighScore
	*/
}
void Visualizer::updateExtra_highscore(){
	//For all extra's with .second == 0, add point with newest highscore
}

void Visualizer::pauseHandler(int r){
	if (r == -1 || (r == 0 && pauseUpdates) || (r == 1 && !pauseUpdates)) {
		if (pauseUpdates){//If Paused then switch to 'Play'/start updating again
			
		}
		else{
			
		}
		pauseUpdates = !pauseUpdates;
	}
}

void Visualizer::updateVP(int x, int y, int w, int h){
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

//Called from Global Idle Function
void Visualizer::update(){
	if (!pauseUpdates){
		play.onlyEvaluate();
		this->highscore = play.highscore->getScore();
		//DO Updates
		int size = ceil(sqrt(play.genSize)), c = -1;
		GLfloat lx = 0, ly = 0, tx = 0, ty = 0;
		for (int i = size - 1; c < play.genSize - 1 && i >= 0; --i){
			for (int j = 0; c < play.genSize - 1 && j < size; ++j){
				c++;
				polys[c].setAttribs(lx, ly, 0, play.currentGen[c].getScore(), 2, 0);
				lx = polys[c].getRX();
				tx = lx > tx ? lx : tx;
				ty = polys[c].getTY() > ty ? polys[c].getTY() : ty;
			}
			ly = ty;
			lx = 0;
		}
		mx = tx; my = ly; mz = 2;
		//
		play.reproduce();
	}
}
void Visualizer::display(void){
	//DO Display!
	if (!pauseRendering){
		glViewport(x, y, w, h);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

			glScaled(1 / mx, 1 / my, 1 / mz);
			glTranslated(-mx, -my, 0);
			if (extras.empty()){
				//Just Draw Sim
				for (int i = 0; i < play.genSize; i++){
					polys[i].drawPoly();
				}
			}
			else{
				//Draw Extras then Sim
			}
			drawBitmapTexti(highscore, .1, .1);

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}
}
void Visualizer::keyboard(unsigned char key, int x, int y){

}
void Visualizer::mouse(int button, int state, int x, int y){

}