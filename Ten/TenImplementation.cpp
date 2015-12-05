#include "Ten_H.h"

std::list<Visualizer> visualizers;

int POsetting = 0; //0=Seed, 1=poFileName
int poSeed;
char* poFileName;

int bGPauseRendering = false;

void idle(){
	for (std::list<Visualizer>::iterator it = visualizers.begin(); it != visualizers.end(); ++it){
		it->update();
	}

	glutPostRedisplay();
}
void display(void){
	if (!bGPauseRendering){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (std::list<Visualizer>::iterator it = visualizers.begin(); it != visualizers.end(); ++it){
			it->display();
		}
		glFlush();
	}
}

void mouse(int button, int state, int x, int y){
	int vY = height - y;
	for (std::list<Visualizer>::iterator it = visualizers.begin(); it != visualizers.end(); ++it){
		if (x >= it->x && x <= it->x + it->w && vY >= it->y && vY <= it->y + it->h) it->mouse(button, state, x, y);
	}
}

void keyboard(unsigned char key, int x, int y){
	int vY = height - y;
	int mod = glutGetModifiers();
	if (mod == GLUT_ACTIVE_ALT){ //Global Commands
		if (key == 'p'){
			for (std::list<Visualizer>::iterator it = visualizers.begin(); it != visualizers.end(); ++it){
				it->pauseHandler(-1);
			}
		}
		else if (key == '*'){
			switch (POsetting){
				case 0:
					visualizers.push_back(Visualizer(0, 0, width, height, Player(TetrisSim(10, 20, poSeed))));
					break;
				case 1:
					visualizers.push_back(Visualizer(0, 0, width, height, Player(TetrisSim(10, 20, poFileName))));
					break;
			}
			reshape(width, height);
		}
		else if (key == '/' && visualizers.size() > 1){
			visualizers.pop_back();
			reshape(width, height);
		}
		else{ //Propagate to all Visualizers
			for (std::list<Visualizer>::iterator it = visualizers.begin(); it != visualizers.end(); ++it){
				it->keyboard(key, x, y);
			}
		}
	}
	else{
		if (key == '\\' && visualizers.size() > 1){
			for (std::list<Visualizer>::iterator it = visualizers.begin(); it != visualizers.end(); ++it){
				if (x >= it->x && x <= it->x + it->w && vY >= it->y && vY <= it->y + it->h){
					visualizers.erase(it);
					reshape(width, height);
					break;
				}
			}
		}
		else {
			for (std::list<Visualizer>::iterator it = visualizers.begin(); it != visualizers.end(); ++it){
				if (x >= it->x && x <= it->x + it->w && vY >= it->y && vY <= it->y + it->h) it->keyboard(key, x, y);
			}
		}
	}
}

void visible(int state){
	if (state == GLUT_VISIBLE){
		bGPauseRendering = false;
	}
	else{
		bGPauseRendering = true;
	}
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
	poSeed = time(NULL);
	initInspector(TetrisSim(10, 20, poSeed));
	visualizers.push_back(Visualizer(0, 0, width, height, Player(TetrisSim(10, 20, poSeed))));

	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, -1, 1);
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
	glutCreateWindow("Stevte01-CS321_Final_Project:TEN");
}