#include "Visualizer_H.h"

Visualizer::Visualizer(int x, int y, int w, int h, Player play){
	this->x = x; this->y = y; this->w = w; this->h = h;
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

//Called from Global Idle Function
void Visualizer::update(){
	if (!pauseUpdates){
		play.onlyEvaluate();
		//DO Updates
		play.reproduce();
	}
}
void Visualizer::display(void){
	//DO Display!
	if (!pauseRendering){
		glViewport(x, y, w, h);
		if (extras.empty()){
			//Just Draw Sim
		}
		else{
			//Draw Extras then Sim
		}
	}
}
void Visualizer::keyboard(unsigned char key, int x, int y){

}
void Visualizer::mouse(int button, int state, int x, int y){

}