#include "Ten_H.h"

Poly::Poly(){
	x = 0; y = 0; z = 0; w = 1; l = 1; h = 1; b = 1;
}
Poly::Poly(GLfloat x, GLfloat y, GLfloat z){
	this->x = x; this->y = y; this->z = z;
	w = 1; l = 1; h = 1; b = 1;
}
void Poly::setPos(GLfloat x, GLfloat y, GLfloat z){
	this->x = x; this->y = y; this->z = z;
}
GLfloat* Poly::getPos(){
	GLfloat* r = (GLfloat*)calloc(3, sizeof(GLfloat));
	r[0] = x;
	r[1] = y;
	r[2] = z;
	return r;
}
GLfloat Poly::getRX(){ return x+w+b; }
GLfloat Poly::getTY(){ return y+l+b; }
GLfloat Poly::getBZ(){ return z; }

void Poly::setAttribs(GLfloat x, GLfloat y, GLfloat z, int aScore, int hScore, int bScore){
	this->x = x; this->y = y; this->z = z;
	aScore2area(aScore);
	hScore2height(hScore);
	bScore2border(bScore);
}
void Poly::aScore2area(int aScore){
	GLfloat s = 0;
	if (aScore == 0) s = 1;
	else s = sqrt(aScore);
	this->w = s;
	this->l = s;
}
void Poly::hScore2height(int hScore){
	this->h = hScore;
}
void Poly::bScore2border(int bScore){
	this->b = bScore;
}
void Poly::drawPoly(){
	//Draw Border
	glBegin(GL_LINE_LOOP);
		glColor3f(1, 0, 0);
		glVertex3f(x, y, z);//Lower Left
		glVertex3f(x, y + l + b, z);//Upper Left
		glVertex3f(x + w + b, y + l + b, z);//Upper Right
		glVertex3f(x + w + b, y, z);//Lower Right
	glEnd();

	//Draw Poly
	GLfloat pX = x + b, pY = y + b;
	GLfloat cX = (pX + pX + w) / 2;
	GLfloat cY = (pY + pY + l) / 2;

	glBegin(GL_TRIANGLES);
		glColor3f(.5, .5, 1);
		glVertex3f(pX, pY, z);
		glVertex3f(pX + w, pY, z);
		glVertex3f(cX, cY, z - h);

		glVertex3f(pX, pY + l, z);
		glVertex3f(pX, pY, z);
		glVertex3f(cX, cY, z - h);

		glVertex3f(pX + w, pY + l, z);
		glVertex3f(pX, pY + l, z);
		glVertex3f(cX, cY, z - h);

		glVertex3f(pX + w, pY, z);
		glVertex3f(pX + w, pY + l, z);
		glVertex3f(cX, cY, z - h);
	glEnd();
}


Player *play;
Poly *polys;

VP simVP, hsgraphVP;
Graph hsgraph;

int updateC = -1;
int updateSim = true;

void updatePiece(int c){
	hsgraph.addPoint(play->highscore->getScore());

	if (updateSim){
		if (c == 0){
			polys[c].setAttribs(0, 0, 0, play->currentGen[c].getScore(), 2, 0);
		}
		else{
			int size = ceil(sqrt(play->genSize)), c = -1;
			GLfloat lx = polys[c - 1].getRX();
			GLfloat ly = c < size ? 0 : polys[c % size].getTY();
			polys[c].setAttribs(lx, ly, 0, play->currentGen[c].getScore(), 2, 0);
		}

		GLfloat tx = polys[c].getRX();
		GLfloat ly = polys[c].getTY();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glScaled(1 / tx, 1 / ly, 1 / 2);
		glTranslated(-tx, -ly, 0);

		updateC = c;
	}

	glutPostRedisplay();
}

//Repr callback Func
void updateBoard(){
	hsgraph.addPoint(play->highscore->getScore());

	if (updateSim){
		int size = ceil(sqrt(play->genSize)), c = -1;
		GLfloat lx = 0, ly = 0, tx = 0;
		for (int i = size - 1; c < play->genSize - 1 && i >= 0; --i){
			for (int j = 0; c < play->genSize - 1 && j < size; ++j){
				c++;
				polys[c].setAttribs(lx, ly, 0, play->currentGen[c].getScore(), 2, 0);
				lx = polys[c].getRX();
				tx = lx > tx ? lx : tx;
			}
			ly = polys[c].getTY();
			lx = 0;
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glScaled(1 / tx, 1 / ly, 1 / 2);
		glTranslated(-tx, -ly, 0);

		updateC = -1;
	}

	glutPostRedisplay();
}

void idle(){
	play->evaluate();
}
void display(void){
	glClear(GL_COLOR_BUFFER_BIT);

	if (updateSim){
		simVP.activate();
		int c = updateC > -1 ? updateC : play->genSize;
		glMatrixMode(GL_MODELVIEW);
		for (int i = 0; i < c; i++){
			polys[i].drawPoly();
		}
	}

	hsgraphVP.activate();
	hsgraph.drawGraph();

	glFlush();
	if (updateSim) Sleep(175);
}

void keyboard(unsigned char key, int x, int y){
	if (key == 'r'){
		updateSim = !updateSim;
		if (updateSim) hsgraph.sThreshold = 1000;
		else hsgraph.sThreshold = 100;
	}
}

void visible(int state){
	if (state == GLUT_NOT_VISIBLE){
		play->setReprCallback(NULL);
		play->setEvalCallback(NULL);
	}
	else play->setReprCallback(updateBoard);
}

void reshape(int w, int h){
	simVP.setH(h);
	simVP.setW(w*.6);
	simVP.setX(w*.4);
	simVP.setY(0);

	hsgraphVP.setH(h/2);
	hsgraphVP.setW(w*.4);
	hsgraphVP.setX(0);
	hsgraphVP.setY(h/2);
}

void other_init(){
	play = new Player(new TetrisSim(10, 20));
	polys = (Poly *)calloc(play->genSize, sizeof(Poly));
	play->setReprCallback(updateBoard);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	//glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPointSize(6);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE);
}

void init_window(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	width = height = 500;
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Stevte01-CS321_Final_Project:TEN");
}