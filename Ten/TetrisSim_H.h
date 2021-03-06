#ifndef TETRISSIM_H
#define TETRISSIM_H

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Random.h"

class Matrix{
public:
	int *matrix;
	int width, height;

	Matrix(int w, int h);
	Matrix(int w, int h, int* m);
	~Matrix();

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
	int w, h, currentPiece, nPieces, bFilePO = false;
	int* pieceOrder;

	void rowCleared(int r);
	void parsePOFile(char* poFileName);
public:
	TetrisSim();
	TetrisSim(int w, int h);
	TetrisSim(int w, int h, char* poFileName);
	TetrisSim(int w, int h, int poSeed);

	int getPieceOrder();
	int getNextPiece();

	int getCurrentPiece();
	int getNPieces();
	int getCurrentPieceWidth(int rotation);

	int getBoardWidth();
	int getBoardHeight();
	int* getBoardState();

	/*t=Type i=Rotation j=X Location*/
	int addPiece(int i, int j);
	int addPiece(int t, int i, int j);

	void resetSim();
};

#endif