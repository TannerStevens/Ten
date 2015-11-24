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
GLfloat* Poly::getCPos(){
	GLfloat* r = (GLfloat*)calloc(3, sizeof(GLfloat));
	GLfloat pX = x + b, pY = y + b;
	r[0] = (pX + pX + w) / 2;
	r[1] = (pY + pY + l) / 2;
	r[2] = z;
	return r;
}
GLfloat Poly::getRX(){ return x+w+2*b; }
GLfloat Poly::getTY(){ return y+l+2*b; }
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
		glVertex3f(x + w + (2 * b), y, z);//Lower Right
		glVertex3f(x + w + (2 * b), y + l + (2 * b), z);//Upper Right
		glVertex3f(x, y + l + (2 * b), z);//Upper Left
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

std::vector<VP> vps;
Graph hsgraph;

int selected = -1;
int ins_prog = 1;

int updateC = -1;
int updateSim = true;
int pauseUpdates = false;

template <class T> GLfloat distance(T* v){
	return sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
}

template <class T> GLfloat dotProduct(T *v1, T *v2){
	return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
}

template <class T, class J> J* p2pVec(T* sP, T* eP){
	J* rVector = (J*)calloc(3, sizeof(J));

	rVector[0] = (J) (eP[0] - sP[0]);
	rVector[1] = (J) (eP[1] - sP[1]);
	rVector[2] = (J) (eP[2] - sP[2]);

	return rVector;
}

template <class T> T* unitfyVector(T* v){
	T* rVector = (T*)calloc(3, sizeof(T));

	GLfloat s = distance(v);

	rVector[0] = (T) (v[0] / s);
	rVector[1] = (T) (v[1] / s);
	rVector[2] = (T) (v[2] / s);

	return rVector;
}

float rayIntersectsSphere(GLfloat* p, GLfloat* d, GLfloat r, GLfloat* c){
	float t=0;
	glColor3f(0, 1, 0);
	//Compute A, B and C coefficients
	GLfloat *vpc = p2pVec<GLfloat, GLfloat>(p, c);
	float A = dotProduct(d, d);
	float B = dotProduct(new GLfloat[]{2 * vpc[0], 2 * vpc[1], 2 * vpc[2]}, d);
	float C = dotProduct(vpc, vpc) - (r * r);

	//Find discriminant
	float disc = B * B - 4 * A * C;

	// if discriminant is negative there are no real roots, so return 
	// false as ray misses sphere
	if (disc < 0)
		return false;

	// compute q as described above
	float distSqrt = sqrtf(disc);
	float q;
	if (B < 0)
		q = (-B - distSqrt) / 2.0;
	else
		q = (-B + distSqrt) / 2.0;

	// compute t0 and t1
	float t0 = q / A;
	float t1 = C / q;

	// make sure t0 is smaller than t1
	if (t0 > t1)
	{
		// if t0 is bigger than t1 swap them around
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}

	// if t1 is less than zero, the object is in the ray's negative direction
	if (t1 < 0)
		return t;

	// if t0 is less than zero, the intersection point is at t1
	if (t0 < 0)
	{
		t = t1;
		return t;
	}
	// else the intersection point is at t0
	else
	{
		t = t0;
		return t;
	}
}


void updatePiece(int c){
	hsgraph.addPoint(play->highscore->getScore());

	if (updateSim){
		if (c == 0){
			polys[c].setAttribs(0, 0, 0, play->currentGen[c].getScore(), 2, 5);
		}
		else{
			int size = ceil(sqrt(play->genSize)), c = -1;
			GLfloat lx = polys[c - 1].getRX();
			GLfloat ly = c < size ? 0 : polys[c % size].getTY();
			polys[c].setAttribs(lx, ly, 0, play->currentGen[c].getScore(), 2, 5);
		}

		GLfloat tx = polys[c].getRX();
		GLfloat ly = polys[c].getTY();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glScaled(1 / tx, 1 / ly, 1 / 2);
		glTranslated(-tx, -ly, 0);
		vps[0].updateModelView();

		updateC = c;
	}

	glutPostRedisplay();
}

//Repr callback Func
void updateBoard(){
	hsgraph.addPoint(play->highscore->getScore());

	if (updateSim){
		int size = ceil(sqrt(play->genSize)), c = -1;
		GLfloat lx = 0, ly = 0, tx = 0, ty = 0;
		for (int i = size - 1; c < play->genSize - 1 && i >= 0; --i){
			printf("\n");
			for (int j = 0; c < play->genSize - 1 && j < size; ++j){
				c++;
				polys[c].setAttribs(lx, ly, 0, play->currentGen[c].getScore(), 2, 1);
				printf(" %i", play->currentGen[c].getScore());
				lx = polys[c].getRX();
				tx = lx > tx ? lx : tx;
				ty = polys[c].getTY() > ty ? polys[c].getTY() : ty;
			}
			ly = ty;
			lx = 0;
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glScaled(1 / tx, 1 / ly, 1 / 2);
		glTranslated(-tx, -ly, 0);
		vps[0].updateModelView();

		updateC = -1;
	}

	glutPostRedisplay();
}

void display_sim(void){
	int c = updateC > -1 ? updateC : play->genSize;
	for (int i = 0; i < c; i++){
		polys[i].drawPoly();
	}
}
void display_graph(void){
	hsgraph.drawGraph();
}

void idle(){
	play->evaluate();
}
void display(void){
	glClear(GL_COLOR_BUFFER_BIT);

	if (updateSim){
		vps[0].activate();
	}

	vps[1].activate();

	glFlush();
	if (updateSim) Sleep(175);
}

template <class T> void printArr(T* arr, int n){
	printf("\n");
	for (int i = 0; i < n; i++){
		printf(" %g", arr[i]);
	}
	printf("\n");
}

void mouse_sim(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			//** Pre-process and determine which Viewport got the Click Event, and then use
			// the appropriate Modelview and Project Matrices
			GLint* viewport = vps[0].getV(); //var to hold the viewport info
			GLdouble* modelview = vps[0].getModelView(); //var to hold the modelview info
			modelview[10] = .5;
			GLdouble* projection = vps[0].getProjection(); //var to hold the projection matrix info
			GLfloat winX, winY; //variables to hold screen x,y,z coordinates
			GLdouble worldCV[3]; //variables to hold world x,y,z coordinates
			GLdouble worldFV[3]; //variables to hold world x,y,z coordinates

			winX = (float)x;
			winY = (float)viewport[3] - (float)y;

			GLint r1, r2;
			//get the world coordinates from the screen coordinates
			r1 = gluUnProject(winX, winY, 1, modelview, projection, viewport, &worldCV[0], &worldCV[1], &worldCV[2]);
			r2 = gluUnProject(winX, winY, -1, modelview, projection, viewport, &worldFV[0], &worldFV[1], &worldFV[2]);
			/*printf("\n%f %f %f", worldCV[0], worldCV[1], worldCV[2]);*/
			//printf("\n%f %f %f", worldFV[0], worldFV[1], worldFV[2]);

			GLfloat wPos[] = { (GLfloat)worldCV[0], (GLfloat)worldCV[1], (GLfloat)worldCV[2] };
			GLfloat* dir = unitfyVector(p2pVec<GLdouble, GLfloat>(worldCV, worldFV));

			glViewport(vps[0].getX(), vps[0].getY(), vps[0].getW(), vps[0].getH());
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			
				glBegin(GL_POINTS);
					glVertex3dv(worldCV);
					glVertex3dv(worldFV);
				glEnd();
				glFlush();

			std::list< std::pair<float, int> > intersections;
			int c = updateC > -1 ? updateC : play->genSize;
			for (int i = 0; i < c*r1*r2; i++){
				GLfloat* pPos = polys[i].getPos();
				printArr(pPos, 3);
				GLfloat r = distance(p2pVec<GLfloat, GLfloat>(pPos, polys[i].getCPos()));

				float d = rayIntersectsSphere(wPos, dir, r, polys[i].getCPos());
				if (d){
					glBegin(GL_POINTS);
						glVertex3fv(polys[i].getCPos());
					glEnd();

					intersections.emplace_back(d, i);
				}
			}

			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();

			if (intersections.size()>0){
				pauseHandler(1);

				selected = intersections.front().second;

				vps[2].activate();
				glutDisplayFunc(display_inspection);
				glutKeyboardFunc(keyboard_inspection);
			}
		}
	}
}

void mouse(int button, int state, int x, int y){
	//Need to figure out how to filter 'active' VP's, ex: Inspection VP
	for (std::vector<VP>::iterator it = vps.begin(); it != vps.end(); ++it){
		if (x > it->getX() && x < it->getX() + it->getW()){
			if (y > it->getY() && y < it->getY() + it->getH()){
				if(it->mouse) it->mouse(button, state, x, y);
			}
		}
	}
}

void pauseHandler(int r){
	if (r == -1 || (r == 0 && pauseUpdates) || (r == 1 && !pauseUpdates)) {
		if (pauseUpdates){//If Paused then switch to 'Play'/start updating again
			glutIdleFunc(idle);
		}
		else{
			glutIdleFunc(NULL);
		}
		pauseUpdates = !pauseUpdates;
	}
}

void display_inspection(void){
	glClear(GL_COLOR_BUFFER_BIT);
	printf("\n##########################################");
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

		TetrisSim *ts = new TetrisSim(10, 20);

		int result = 0, score = 0, c = 0;
		Random *r = new Random(play->currentGen[selected].getRotationGene());
		Random *p = new Random(play->currentGen[selected].getPositionGene());
		do{
			int i, j;
			i = 0; j = 0;
			i = r->Next(3);
			j = p->Next(ts->getBoardWidth() - 1);
			result = ts->addPiece(i, j);
			score += result;

			int* matrix = ts->getBoardState();
			printf("\n  ");
			for (int i = 0; i < 10; i++){ printf(" %i", i); }
			printf("\n");
			for (int m = 0; m < 20; m++){
				printf("\n%i ", m);
				for (int n = 0; n < 10; n++){
					printf(" %i", matrix[m*10 + n]);
				}
			}
			printf("\n%i\n", score);
		} while (c++<ins_prog);
		delete r; delete p;

		delete ts;
	glPopMatrix();
}

void keyboard_inspection(unsigned char key, int x, int y){
	if (key == 'q'){
		glutDisplayFunc(display);
		glutKeyboardFunc(keyboard);
		glutMouseFunc(mouse);
	}
	else if (key == ' '){
		ins_prog++;
	}

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
	if (key == 'r'){
		updateSim = !updateSim;
		if (updateSim) hsgraph.sThreshold = 1000;
		else hsgraph.sThreshold = 100;
	}
	else if (key == 'p'){
		pauseHandler(-1);
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
	vps[0].setH(h);
	vps[0].setW(w*.6);
	vps[0].setX(w*.4);
	vps[0].setY(0);

	vps[1].setH(h/2);
	vps[1].setW(w*.4);
	vps[1].setX(0);
	vps[1].setY(h/2);

	vps[2].setH(h);
	vps[2].setW(w);
}

void other_init(){
	play = new Player(new TetrisSim(10, 20));
	polys = (Poly *)calloc(play->genSize, sizeof(Poly));
	play->setReprCallback(updateBoard);

	vps.emplace_back(*new VP());
	vps[0].display = display_sim;
	vps[0].mouse = mouse_sim;
	vps.emplace_back(*new VP());
	vps[1].display = display_graph;
	vps.emplace_back(*new VP());
	vps[2].display = display_inspection;
	vps[2].keyboard = keyboard_inspection;

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