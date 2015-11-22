#include "Ten_H.h"

void main(int argc, char** argv){
	init_window(argc, argv);
	other_init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutVisibilityFunc(visible);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);
	glutMainLoop();

	getchar(); getchar();
}