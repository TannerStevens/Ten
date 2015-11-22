#include "Ten_H.h"

void main(int argc, char** argv){
	init_window(argc, argv);
	other_init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutVisibilityFunc(visible);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();

	getchar(); getchar();
}