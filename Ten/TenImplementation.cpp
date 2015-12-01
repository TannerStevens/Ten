#include "Ten_H.h"

std::list<Visualizer> visualizers;

void idle(){
	for (std::list<Visualizer>::iterator it = visualizers.begin(); it != visualizers.end(); ++it){
		it->update();
	}

	glutPostRedisplay();
}
void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (std::list<Visualizer>::iterator it = visualizers.begin(); it != visualizers.end(); ++it){
		it->display();
	}
	glFlush();
}

void mouse(int button, int state, int x, int y){
	int vY = height - y;
	printf("\n%i %i", x, vY);
	//y check is flipped since VP y's are upside down/backwards/flipped.
	for (std::list<Visualizer>::iterator it = visualizers.begin(); it != visualizers.end(); ++it){
		if (x >= it->x && x <= it->x + it->w && vY >= it->y && vY <= it->y + it->h) it->mouse(button, state, x, y);
	}
}

void keyboard(unsigned char key, int x, int y){
	int vY = height - y;
	int mod = glutGetModifiers();
	if (mod == GLUT_ACTIVE_ALT){ //Global Commands
		if (key == 'i') Inspect(DNA());
		else if (key == 'p'){
			for (std::list<Visualizer>::iterator it = visualizers.begin(); it != visualizers.end(); ++it){
				it->pauseHandler(-1);
			}
		}
	}
	else{
		for (std::list<Visualizer>::iterator it = visualizers.begin(); it != visualizers.end(); ++it){
			if (x >= it->x && x <= it->x + it->w && vY >= it->y && vY <= it->y + it->h) it->keyboard(key, x, y);
		}
	}
}

void visible(int state){

}

void reshape(int w, int h){
	width = w;
	height = h;
	std::list<Visualizer>::iterator it = visualizers.begin(); 
	int size = ceil(pow(visualizers.size(), .5));
	GLfloat Width = floor(glutGet(GLUT_WINDOW_WIDTH) / size);
	GLfloat Height = floor(glutGet(GLUT_WINDOW_HEIGHT) / size);

	for (int i = size - 1; i >= 0; --i){
		for (int j = 0; j < size; ++j){
			it->updateVP(Width*j, Height*i, Width, Height);
			if (++it == visualizers.end()){
				i = -1;
				break;
			}
		}
	}
}

void other_init(){
	visualizers.push_back(Visualizer(0, 0, width / 2, height / 2, Player(new TetrisSim(10, 20))));
	visualizers.push_back(Visualizer(width / 2, 0, width / 2, height / 2, Player(new TetrisSim(10, 20))));
	visualizers.push_back(Visualizer(0, height / 2, width / 2, height / 2, Player(new TetrisSim(10, 20))));
	visualizers.push_back(Visualizer(width / 2, height / 2, width / 2, height / 2, Player(new TetrisSim(10, 20))));

	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, new GLfloat[]{0, 0, .5, 1});

	glPointSize(6);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}

void init_window(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	width = height = 500;
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 50);
	int n = glutCreateWindow("Stevte01-CS321_Final_Project:TEN");
}