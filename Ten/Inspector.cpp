#include "Inspector_H.h"

template <class T> GLfloat distance(T* v){
	return sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
}

template <class T, class J> J* p2pVec(T* sP, T* eP){
	J* rVector = (J*)calloc(4, sizeof(J));

	rVector[0] = (J)(eP[0] - sP[0]);
	rVector[1] = (J)(eP[1] - sP[1]);
	rVector[2] = (J)(eP[2] - sP[2]);
	rVector[3] = 1;

	return rVector;
}

template <class T> T* unitfyVector(T* v){
	T* rVector = (T*)calloc(4, sizeof(T));

	GLfloat s = distance(v);

	rVector[0] = (T)(v[0] / s);
	rVector[1] = (T)(v[1] / s);
	rVector[2] = (T)(v[2] / s);
	rVector[3] = 1;

	return rVector;
}

Inspector active;

Inspector::Inspector(){}
Inspector::Inspector(TetrisSim ts){
	int lWin = glutGetWindow();
	this->ts = ts;
	this->pieces = (int *)calloc(ceil(ts.getBoardWidth()*ts.getBoardHeight()), sizeof(int));
	this->pieceCount = 0;
	this->bObj = false;
	this->lBoard = ts.getBoardState();
	this->tpBase = OBJLoader("dodecahedron.obj");
	this->winID = glutCreateWindow("Inspector");

	glutDisplayFunc(display_inspector);
	glutKeyboardFunc(keyboard_inspector);
	glutPassiveMotionFunc(pMouseMove_inspector);

	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	this->bgTexture = SOIL_load_OGL_texture
		(
			"tetris.jpg",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y
		);

	glLightf(GL_LIGHT0 + 5, GL_SPOT_EXPONENT, 120);
	glLightf(GL_LIGHT0 + 5, GL_SPOT_CUTOFF, 10);
	glLightfv(GL_LIGHT0 + 5, GL_DIFFUSE, new GLfloat[]{1, 1, 1, 1});
	glLightfv(GL_LIGHT0 + 5, GL_SPECULAR, new GLfloat[]{1, 1, 1, 1});
	glLightf(GL_LIGHT0 + 5, GL_LINEAR_ATTENUATION, .15);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, new GLfloat[]{1, 0, 0, 1});
	glMaterialfv(GL_FRONT, GL_SPECULAR, new GLfloat[]{0, 0, 1, 1});
	glMaterialf(GL_FRONT, GL_SHININESS, 4);

	glPointSize(6);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0+5);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

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
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, bgTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1, -1, -.9);
		glTexCoord2f(0.0, 1); glVertex3f(-1, 1, -.9);
		glTexCoord2f(1, 1); glVertex3f(1, 1, -.9);
		glTexCoord2f(1, 0.0); glVertex3f(1, -1, -.9);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	glScaled(transformations[6] / (GLfloat)ts.getBoardWidth(), transformations[7] / (GLfloat)ts.getBoardHeight(), transformations[8] / (GLfloat)ts.getBoardHeight());
	glTranslated(-(ts.getBoardWidth() / 2) + transformations[0], -ts.getBoardHeight() + transformations[1], transformations[2]);
	glRotated(transformations[5], 0, 0, 1);
	glRotated(transformations[4], 0, 1, 0);
	glRotated(transformations[3], 1, 0, 0);

	int* board = ts.getBoardState();
	float x = 0, y = ts.getBoardHeight();
	for (int h = 0; h < ts.getBoardHeight(); h++){
		for (int w = 0; w < ts.getBoardWidth(); w++){
			if (board[h*ts.getBoardWidth() + w]){
				//tpBase.draw(w,y-h,0);
				glPushMatrix();
				glTranslatef(w, y - h, 0);
				if (bObj){
					glScalef(.55, .55, .55);
					tpBase.draw();
				}
				else {
					glScalef(.35, .35, .35);
					glutSolidDodecahedron();
				}
				glPopMatrix();
			}
		}
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
	else if (key == '/') bObj = !bObj;
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
	int j = p.Next(ts.getBoardWidth() - ts.getCurrentPieceWidth(i));
	int result = ts.addPiece(i,j);
	ins.addToScore(result);


	glutPostRedisplay();
}

void Inspector::mouse_movement(int x, int y){
	GLint viewport[4]; //var to hold the viewport info
	GLdouble modelview[16]; //var to hold the modelview info
	GLdouble projection[16]; //var to hold the projection matrix info
	GLfloat winX, winY; //variables to hold screen x,y,z coordinates
	GLdouble worldCV[4]; //variables to hold world x,y,z coordinates
	GLdouble worldFV[4]; //variables to hold world x,y,z coordinates

	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
	glMatrixMode(GL_PROJECTION);
	glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info

	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)glutGet(GLUT_WINDOW_HEIGHT) - (float)y;
	//winY = (float)y;

	GLint r1, r2;
	//get the world coordinates from the screen coordinates
	r1 = gluUnProject(winX, winY, 1, modelview, projection, viewport, &worldCV[0], &worldCV[1], &worldCV[2]);
	r2 = gluUnProject(winX, winY, -1, modelview, projection, viewport, &worldFV[0], &worldFV[1], &worldFV[2]);

	GLfloat wPos[] = { (GLfloat)worldFV[0], (GLfloat)worldFV[1], (GLfloat)worldFV[2], 1 };
	GLfloat* dir = unitfyVector(p2pVec<GLdouble, GLfloat>(worldFV, worldCV));

	glLightfv(GL_LIGHT0 + 5, GL_POSITION, wPos);
	glLightfv(GL_LIGHT0 + 5, GL_SPOT_DIRECTION, dir);
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

void pMouseMove_inspector(int x, int y){
	active.mouse_movement(x, y);
}