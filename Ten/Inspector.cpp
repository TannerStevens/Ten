#include "Inspector_H.h"

Inspector active;

Inspector::Inspector(){}
Inspector::Inspector(TetrisSim ts){
	int lWin = glutGetWindow();
	this->ts = ts;
	this->lBoard = ts.getBoardState();
	this->winID = glutCreateWindow("Inspector");
	glutDisplayFunc(display_inspector);
	glutKeyboardFunc(keyboard_inspector);
	glClearColor(1, 1, 1, 0);
	glutSetWindow(lWin);
}

void drawBitmapInteger(int in, float x, float y){
	if(in) glColor3f(0, 0, 0);
	else glColor3f(1, 1, 1);

	char *c, k[5];
	sprintf_s(k, 5, "%i", in);
	glRasterPos2f(x, y);

	for (c = k; *c != '\0'; c++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
	}
}

void Inspector::display(void){
	glutSetWindow(this->winID);

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	/*
	Use board-lBoard to identify pieces and then add them to a seperate list of pieces
	Pieces should be .obj's
		means that the objLoader needs its draw method integrated
	*/
	glScalef(1 / (GLfloat)ts.getBoardWidth(), 1 / (GLfloat)ts.getBoardHeight(), 1);
	glTranslatef(-ts.getBoardWidth(), -ts.getBoardHeight(), 0);
	int* board = ts.getBoardState();
	float x = 0, y = ts.getBoardHeight();
	for (int h = 0; h < ts.getBoardHeight(); h++){
		for (int w = 0; w < ts.getBoardWidth(); w++){
			//if (board[h*ts.getBoardWidth() + w] != lBoard[h*ts.getBoardWidth() + w])
			drawBitmapInteger(board[h*ts.getBoardWidth() + w], x++, y);
		}
		y -= 1;
		x = 0;
	}
	drawBitmapInteger(ins.getScore(), x, ts.getBoardHeight()+2);

	glPopMatrix();

	lBoard = board;
	delete board;
	glFlush();

	glutSetWindow(1);
	glutPostRedisplay();
}

void Inspector::keyboard(unsigned char key, int x, int y){
	if (key == ' ') nextStep();
}

void Inspector::nextStep(){
	++this->step;
	Random r = Random(this->ins.getRotationGene());
	Random p = Random(this->ins.getPositionGene());
	int c = 0;
	while (c++ < this->step){
		r.Next(3);
		p.Next(ts.getBoardWidth() - 1);
	}
	int i = r.Next(3);
	int j = p.Next(ts.getBoardWidth() - 1);
	int result = ts.addPiece(i,j);
	ins.addToScore(result);

	glutPostRedisplay();
}

void Inspect(DNA cur){
	active.ins = cur;
	active.step = -1;
	active.ts.resetSim();
	active.display();
}

void initInspector(TetrisSim ts){
	active = Inspector(ts);
}

void display_inspector(void){
	active.display();
}
void keyboard_inspector(unsigned char key, int x, int y){
	active.keyboard(key, x, y);
}
