#include "Inspector_H.h"

Inspector active;

Inspector::Inspector(){}
Inspector::Inspector(TetrisSim ts){
	int lWin = glutGetWindow();
	this->ts = ts;
	this->pieces = (int *)calloc(ceil(ts.getBoardWidth()*ts.getBoardHeight()), sizeof(int));
	this->pieceCount = 0;
	this->lBoard = ts.getBoardState();
	this->tpBase = OBJLoader("tetrisbase.obj");
	this->winID = glutCreateWindow("Inspector");
	glutDisplayFunc(display_inspector);
	glutKeyboardFunc(keyboard_inspector);
	glClearColor(0, 0, 0, 0);

	this->transformations[0] = 0;
	this->transformations[1] = 0;
	this->transformations[2] = 0;
	this->transformations[3] = 0;
	this->transformations[4] = 0;
	this->transformations[5] = 0;
	this->transformations[6] = 1;
	this->transformations[7] = 1;
	this->transformations[8] = 1;

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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	/*
	Use board-lBoard to identify pieces and then add them to a seperate list of pieces
	Pieces should be .obj's
	means that the objLoader needs its draw method integrated
	*/
	glScaled(transformations[6] / (GLfloat)ts.getBoardWidth(), transformations[7] / (GLfloat)ts.getBoardHeight(), transformations[8]);
	glTranslated(-ts.getBoardWidth() + transformations[0], -ts.getBoardHeight() + transformations[1], transformations[2]);
	glRotated(transformations[5], 0, 0, 1);
	glRotated(transformations[4], 0, 1, 0);
	glRotated(transformations[3], 1, 0, 0);
	//Random r = Random();
	//glColor3f(r.NextDouble(0, 1), r.NextDouble(0, 1), r.NextDouble(0, 1)); Change Koefficients instead

	for (int n = 0; n < pieceCount; n++){
		int pIndex = n * 4;
		drawTetrisPiece(pieces[pIndex], pieces[pIndex + 1], pieces[pIndex + 2], pieces[pIndex + 3]);
	}

	//

	char k[5];
	sprintf_s(k, 5, "%i", ins.getScore());
	glutSetWindowTitle(k);
	glPopMatrix();

	glFlush();

	glutSetWindow(1);
	glutPostRedisplay();
}

void Inspector::keyboard(unsigned char key, int x, int y){
	if (key == ' ') nextStep();
	else if (key == 'w') this->transformations[1] += .1;
	else if (key == 's') this->transformations[1] -= .1;
	else if (key == 'a') this->transformations[0] -= .1;
	else if (key == 'd') this->transformations[0] += .1;
	else if (key == 'q') this->transformations[2] += .1;
	else if (key == 'e') this->transformations[2] -= .1;

	else if (key == '8') this->transformations[4]++;
	else if (key == '2') this->transformations[4]--;
	else if (key == '4') this->transformations[3]++;
	else if (key == '6') this->transformations[3]--;
	else if (key == '7') this->transformations[5]++;
	else if (key == '9') this->transformations[5]--;

	else if (key == '+'){
		this->transformations[6] += .1;
		this->transformations[7] += .1;
		this->transformations[8] += .1;
	}
	else if (key == '-'){
		this->transformations[6] -= .1;
		this->transformations[7] -= .1;
		this->transformations[8] -= .1;
	}

	else if (key == '5'){
		this->transformations[0] = 0;
		this->transformations[1] = 0;
		this->transformations[2] = 0;
		this->transformations[3] = 0;
		this->transformations[4] = 0;
		this->transformations[5] = 0;
		this->transformations[6] = 1;
		this->transformations[7] = 1;
		this->transformations[8] = 1;
	}
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
	int t = ts.getNextPiece();
	int result = ts.addPiece(i,j);
	ins.addToScore(result);

	int* board = ts.getBoardState();
	int ly = 0;
	for (int h = ts.getBoardHeight()-1; h > -1 ; h--){
		for (int w = 0; w < ts.getBoardWidth(); w++){
			if (board[h*ts.getBoardWidth() + w] != lBoard[h*ts.getBoardWidth() + w]){
				ly = ts.getBoardHeight() - 1 - h;
				h = -1;
				break;
			}
		}
	}

	delete lBoard;
	lBoard = board;

	int pIndex = pieceCount++ * 4;
	pieces[pIndex] = t;
	pieces[pIndex + 1] = j;
	pieces[pIndex + 2] = ly;
	pieces[pIndex + 3] = i;

	glutPostRedisplay();
}

void Inspector::drawTetrisPiece(int t, int x, int y, int i){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	//Add Rotation States since rotating with openGL isn't working out..
	glTranslatef(x, y, 0);
	if (t == 0){
		tpBase.draw();
		glTranslatef(1, 0, 0);
		tpBase.draw();
		glTranslatef(-1, 1, 0);
		tpBase.draw();
		glTranslatef(1, 0, 0);
		tpBase.draw();
	}
	else if (t == 1){
		tpBase.draw();
		glTranslatef(1, 0, 0);
		tpBase.draw();
		glTranslatef(0, 1, 0);
		tpBase.draw();
		glTranslatef(0, 1, 0);
		tpBase.draw();
	}
	else if (t == 2){
		tpBase.draw();
		glTranslatef(1, 0, 0);
		tpBase.draw();
		glTranslatef(-1, 1, 0);
		tpBase.draw();
		glTranslatef(0, 1, 0);
		tpBase.draw();
	}
	else if (t == 3){
		tpBase.draw();
		glTranslatef(0, 1, 0);
		tpBase.draw();
		glTranslatef(0, 1, 0);
		tpBase.draw();
		glTranslatef(0, 1, 0);
		tpBase.draw();
	}
	else if (t == 4){
		tpBase.draw();
		glTranslatef(0, 1, 0);
		tpBase.draw();
		glTranslatef(1, 0, 0);
		tpBase.draw();
		glTranslatef(0, 1, 0);
		tpBase.draw();
	}
	else if (t == 5){
		glTranslatef(1, 0, 0);
		tpBase.draw();
		glTranslatef(0, 1, 0);
		tpBase.draw();
		glTranslatef(-1, 0, 0);
		tpBase.draw();
		glTranslatef(0, 1, 0);
		tpBase.draw();
	}
	else if (t == 6){
		tpBase.draw();
		glTranslatef(1, 0, 0);
		tpBase.draw();
		glTranslatef(1, 0, 0);
		tpBase.draw();
		glTranslatef(-1, 1, 0);
		tpBase.draw();
	}
	glPopMatrix();

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
