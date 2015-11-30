#include "Ten_H.h"

std::list<Visualizer> visualizers;

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

void idle(){
	for (std::list<Visualizer>::iterator it = visualizers.begin(); it != visualizers.end(); ++it){
		it->update();
	}

	glutPostRedisplay();
}
void display(void){
	glClear(GL_COLOR_BUFFER_BIT);

	for (std::list<Visualizer>::iterator it = visualizers.begin(); it != visualizers.end(); ++it){
		it->display();
	}
	glFlush();
}

void mouse(int button, int state, int x, int y){
	
}

/*void display_inspection(void){
	glClear(GL_COLOR_BUFFER_BIT);
	printf("\n##########################################");
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

		TetrisSim *ts = new TetrisSim(10, 20);

		int result = 0, score = 0, c = 0;
		//Random *r = new Random(play->currentGen[selected].getRotationGene());
		//Random *p = new Random(play->currentGen[selected].getPositionGene());
		do{
			int i, j;
			i = 0; j = 0;
			//i = r->Next(3);
			//j = p->Next(ts->getBoardWidth() - 1);
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
		//} while (c++<ins_prog);
		//delete r; delete p;

		delete ts;
	glPopMatrix();
}*/

void keyboard(unsigned char key, int x, int y){
	
}

void visible(int state){

}

void reshape(int w, int h){
	/*std::list<Visualizer>::iterator it = visualizers.begin(); 
	++it;
	int size = ceil(sqrt(it->play.genSize));
	for (int i = size - 1; c < play.genSize - 1 && i >= 0; --i){
		for (int j = 0; c < play.genSize - 1 && j < size; ++j){
		}
	}
	it->updateVP();*/
}

void other_init(){
	visualizers.push_back(Visualizer(0, 0, width / 2, height / 2, Player(new TetrisSim(10, 20))));
	visualizers.push_back(Visualizer(width / 2, 0, width / 2, height / 2, Player(new TetrisSim(10, 20))));
	visualizers.push_back(Visualizer(0, height / 2, width / 2, height / 2, Player(new TetrisSim(10, 20))));
	visualizers.push_back(Visualizer(width / 2, height / 2, width / 2, height / 2, Player(new TetrisSim(10, 20))));

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