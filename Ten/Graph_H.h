#include <gl/freeglut.h>
#include <list>

class Point;
class Graph;

class Point{
private:
	int* coords;
public:
	Point();
	Point(int x, int y);

	void setPoint(int x, int y);
	int* getPoint();
};

class Graph{
private:
	int lX, mX, lY, mY;
	std::list<Point> points;
public:
	int sThreshold;

	Graph();
	Graph(int sThreshold);

	void addPoint(int y);
	void addPoint(int x, int y);
	void drawGraph();

	void prunePoints();
};