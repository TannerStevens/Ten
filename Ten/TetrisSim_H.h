#ifndef TetrisSim_H
#define TetrisSim_H

#include <malloc.h>
#include <stdio.h>

class Matrix{
public:
	int *matrix;
	int width, height;

	Matrix(int w, int h);
	Matrix(int w, int h, int* m);

	int getWidth();
	int getHeight();
	int getValueAt(int row, int col);
	void setValueAt(int row, int col, int value);
	void addtoValueAt(int row, int col, int value);
	void display();
};

class Piece{
private:
	Matrix *Mrot0, *Mrot1, *Mrot2, *Mrot3;
public:
	Piece();
	Piece(Matrix *r0, Matrix *r1, Matrix *r2, Matrix *r3);

	Matrix* getPieceMatrix(int i);
};

class TetrisSim{
private:
	Matrix *board;
	Piece Pieces[7];
	int w, h;
public:
	TetrisSim();
	TetrisSim(int w, int h);

	/*t=Type i=Rotation j=X Location*/
	int addPiece(int t, int i, int j);
};

#endif