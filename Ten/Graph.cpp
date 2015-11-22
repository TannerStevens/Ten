#include "Graph_H.h"

Point::Point() : Point(0,0){}
Point::Point(int x, int y){
	this->coords = (int *)calloc(2, sizeof(int));
	coords[0] = x;
	coords[1] = y;
}

void Point::setPoint(int x, int y){
	coords[0] = x; coords[1] = y;
}
int* Point::getPoint(){ return coords; }


Graph::Graph():Graph(500){

}
Graph::Graph(int sThreshold){
	this->sThreshold = sThreshold;
}

void Graph::addPoint(int y){
	addPoint(points.size(), y);
}
void Graph::addPoint(int x, int y){
	points.push_back(Point(x, y));

	if (x > mX) mX = x;
	else if (x < lX) lX = x;
	if (y > mY) mY = y;
	else if (y < lY) lY = y;

	if (points.size() > sThreshold) prunePoints();
}

void Graph::prunePoints(){
	std::list<Point> tP;

	int c = 0, sampleSize = 5;
	float sum = 0;
	for (std::list<Point>::iterator cPoint = points.begin(); cPoint != points.end(); ++cPoint){
		sum += cPoint->getPoint()[1];
		if (c++ == sampleSize){
			int avg = floor(sum / (sampleSize+1));
			tP.push_back(Point(tP.size(), avg));
			c = 0;
			sum = 0;
		}
	}

	points = tP;
}

void Graph::drawGraph(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslated(-.5, -.5, 0);
	GLdouble sX = 1 / ((GLdouble)mX + 1);
	GLdouble sY = 1 / ((GLdouble)mY + 1);
	glScaled(sX, sY, 1);

	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
		glVertex2i(lX, 0);
		glVertex2i(mX, 0);

		glVertex2i(0, lY);
		glVertex2i(0, mY);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glVertex2i(0, 0);
		for (std::list<Point>::iterator cPoint = points.begin(); cPoint != points.end(); ++cPoint){
			glVertex2iv(cPoint->getPoint());
		}
	glEnd();
}