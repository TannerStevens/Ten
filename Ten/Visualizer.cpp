#include "Visualizer_H.h"

template <class T> GLfloat distance(T* v){
	return sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
}

template <class T> GLfloat dotProduct(T *v1, T *v2){
	return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
}

template <class T, class J> J* p2pVec(T* sP, T* eP){
	J* rVector = (J*)calloc(3, sizeof(J));

	rVector[0] = (J)(eP[0] - sP[0]);
	rVector[1] = (J)(eP[1] - sP[1]);
	rVector[2] = (J)(eP[2] - sP[2]);

	return rVector;
}

template <class T> T* unitfyVector(T* v){
	T* rVector = (T*)calloc(3, sizeof(T));

	GLfloat s = distance(v);

	rVector[0] = (T)(v[0] / s);
	rVector[1] = (T)(v[1] / s);
	rVector[2] = (T)(v[2] / s);

	return rVector;
}

float rayIntersectsSphere(GLfloat* p, GLfloat* d, GLfloat r, GLfloat* c){
	float t = 0;
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
		return -1;

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

void drawBitmapTexti(int in, float x, float y){
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1, 1, 1);

	char *c, k[5];
	sprintf_s(k, 5,"%i", in);
	glRasterPos2f(x, y);

	for (c = k; *c != '\0'; c++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}


Visualizer::Visualizer(int x, int y, int w, int h, Player play){
	this->pauseUpdates = false;
	this->pauseRendering = false;
	this->polys = (Poly *)calloc(play.genSize, sizeof(Poly));
	this->x = x; this->y = y; this->w = w; this->h = h; this->delay = 0;
	this->highscore = 0;
	this->play = play;

	this->transformations[0] = 0;
	this->transformations[1] = 0;
	this->transformations[2] = 0;
	this->transformations[3] = 5;
	this->transformations[4] = -2;
	this->transformations[5] = 0;
	this->transformations[6] = 1;
	this->transformations[7] = 1;
	this->transformations[8] = 1;
}

void Visualizer::pauseHandler(int r){
	if (r == -1 || (r == 0 && pauseUpdates) || (r == 1 && !pauseUpdates)) {
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
		GLfloat lx = 0, ly = 0, tx = 0, ty = 0, tz = 0;
		for (int i = size - 1; c < play.genSize - 1 && i >= 0; --i){
			for (int j = 0; c < play.genSize - 1 && j < size; ++j){
				c++;
				polys[c].setAttribs(lx, ly, 0, play.currentGen[c].getScore(), play.currentGen[c].getAggHeight(), play.currentGen[c].getHoles());
				polys[c].setMaterials(play.currentGen[c].getRotationGene(), play.currentGen[c].getPositionGene());
				lx = polys[c].getRX();
				tx = lx > tx ? lx : tx;
				ty = polys[c].getTY() > ty ? polys[c].getTY() : ty;
				tz = play.currentGen[c].getAggHeight() > tz ? play.currentGen[c].getAggHeight() : tz;
			}
			ly = ty;
			lx = 0;
		}
		mx = tx; my = ly; mz = tz;
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

			glTranslated(-1 + transformations[0], -1 + transformations[1], transformations[2]);
			glScaled(transformations[6] / mx, transformations[7] / my, transformations[8] / mz);
			glRotated(transformations[5], 0, 0, 1);
			glRotated(transformations[4], 0, 1, 0);
			glRotated(transformations[3], 1, 0, 0);
			for (int i = 0; i < play.genSize; i++){
				polys[i].drawPoly();
				if (this->delay){ glFlush(); Sleep(delay); }
			}
			drawBitmapTexti(highscore, .1, .1);

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}
}
void Visualizer::keyboard(unsigned char key, int x, int y){
	if (key == 'p') pauseHandler(-1);
	else if (key == ',') delay = 20;
	else if (key == '.') delay = 0;

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
		this->transformations[6]+=.1; 
		this->transformations[7]+=.1;
		this->transformations[8]+=.1;
	}
	else if (key == '-'){
		this->transformations[6]-=.1;
		this->transformations[7]-=.1;
		this->transformations[8]-=.1;
	}

	else if (key == '5'){
		this->transformations[0] = 0;
		this->transformations[1] = 0;
		this->transformations[2] = 0;
		this->transformations[3] = 5;
		this->transformations[4] = -2;
		this->transformations[5] = 0;
		this->transformations[6] = 1;
		this->transformations[7] = 1;
		this->transformations[8] = 1;
	}
}
void Visualizer::mouse(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			GLint viewport[4]; //var to hold the viewport info
			GLdouble modelview[16]; //var to hold the modelview info
			GLdouble projection[16]; //var to hold the projection matrix info
			GLfloat winX, winY; //variables to hold screen x,y,z coordinates
			GLdouble worldCV[4]; //variables to hold world x,y,z coordinates
			GLdouble worldFV[4]; //variables to hold world x,y,z coordinates

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glScalef(1 / mx, 1 / my, 1 / mz);
			glTranslatef(-mx, -my, 0);
			glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
			glPopMatrix();

			viewport[0] = this->x;
			viewport[1] = this->y;
			viewport[2] = this->w;
			viewport[3] = this->h;

			winX = (float)x;
			winY = (float)glutGet(GLUT_WINDOW_HEIGHT) - (float)y;
			//winY = (float)y;

			GLint r1, r2;
			//get the world coordinates from the screen coordinates
			r1 = gluUnProject(winX, winY, 1, modelview, projection, viewport, &worldCV[0], &worldCV[1], &worldCV[2]);
			r2 = gluUnProject(winX, winY, -1, modelview, projection, viewport, &worldFV[0], &worldFV[1], &worldFV[2]);
			printf("\n%f %f %f", worldCV[0], worldCV[1], worldCV[2]);
			printf("\n%f %f %f", worldFV[0], worldFV[1], worldFV[2]);

			GLfloat wPos[] = { (GLfloat)worldCV[0], (GLfloat)worldCV[1], (GLfloat)worldCV[2] };
			GLfloat* dir = unitfyVector(p2pVec<GLdouble, GLfloat>(worldCV, worldFV));

			//std::list< std::pair<int, int> > intersections;
			printf("\n");
			for (int i = 0; i < play.genSize; i++){
				GLfloat* pPos = polys[i].getPos();
				GLfloat r = distance(p2pVec<GLfloat, GLfloat>(pPos, polys[i].getCPos()));

				int d = rayIntersectsSphere(wPos, dir, r, polys[i].getCPos());
				printf("%i:%i ", i, d);
				if (d > -1){
					//intersections.emplace_back(d, i);
					Inspect(play.currentGen[i]);
					break;
				}
			}
		}
	}
}