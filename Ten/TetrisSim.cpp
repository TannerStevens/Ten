#include "TetrisSim_H.h"

Matrix::Matrix(int w, int h){
	matrix = (int *)calloc(w*h, sizeof(int));
	height = h; width = w;
}
Matrix::Matrix(int w, int h, int* m){
	matrix = m;
	height = h; width = w;
}

int Matrix::getWidth(){ return width; }
int Matrix::getHeight(){ return height; }

int Matrix::getValueAt(int row, int col){
	return matrix[row*width + col];
}
void Matrix::setValueAt(int row, int col, int value){
	matrix[row*width + col] = value;
}
void Matrix::addtoValueAt(int row, int col, int value){
	matrix[row*width + col] += value;
}

void Matrix::display(){
	printf("\n  ");
	for (int i = 0; i < width; i++){ printf(" %i", i); }
	printf("\n");
	for (int m = 0; m < height; m++){
		printf("\n%i ",m);
		for (int n = 0; n < width; n++){
			printf(" %i", matrix[m*width + n]);
		}
	}
	printf("\n");
}

Piece::Piece(){};
Piece::Piece(Matrix *r0, Matrix *r1, Matrix *r2, Matrix *r3){
	Mrot0 = r0;
	Mrot1 = r1;
	Mrot2 = r2;
	Mrot3 = r3;
}

Matrix* Piece::getPieceMatrix(int i){
	switch (i){
		case 0:
			return Mrot0;
		case 1:
			return Mrot1;
		case 2:
			return Mrot2;
		case 3:
			return Mrot3;
	}
}

TetrisSim::TetrisSim(){};
TetrisSim::TetrisSim(int w, int h){
	board = new Matrix(w, h);
	
	Pieces[0] = *new Piece(
		new Matrix(2, 2, new int[4]{1, 1, 1, 1}),
		new Matrix(2, 2, new int[4]{1, 1, 1, 1}),
		new Matrix(2, 2, new int[4]{1, 1, 1, 1}),
		new Matrix(2, 2, new int[4]{1, 1, 1, 1})
		);

	Pieces[1] = *new Piece(
		new Matrix(2, 3, new int[6]{0, 1, 0, 1, 1, 1}),
		new Matrix(3, 2, new int[6]{1, 1, 1, 0, 0, 1}),
		new Matrix(2, 3, new int[6]{1, 1, 1, 0, 1, 0}),
		new Matrix(3, 2, new int[6]{1, 0, 0, 1, 1, 1})
		);
	Pieces[2] = *new Piece(
		new Matrix(2, 3, new int[6]{1, 0, 1, 0, 1, 1}),
		new Matrix(3, 2, new int[6]{0, 0, 1, 1, 1, 1}),
		new Matrix(2, 3, new int[6]{1, 1, 0, 1, 0, 1}),
		new Matrix(3, 2, new int[6]{1, 1, 1, 1, 0, 0})
		);
	Pieces[3] = *new Piece(
		new Matrix(1, 4, new int[4]{1, 1, 1, 1}),
		new Matrix(4, 1, new int[4]{1, 1, 1, 1}),
		new Matrix(1, 4, new int[4]{1, 1, 1, 1}),
		new Matrix(4, 1, new int[4]{1, 1, 1, 1})
		);
	Pieces[4] = *new Piece(
		new Matrix(2, 3, new int[6]{0, 1, 1, 1, 1, 0}),
		new Matrix(3, 2, new int[6]{1, 1, 0, 0, 1, 1}),
		new Matrix(2, 3, new int[6]{0, 1, 1, 1, 1, 0}),
		new Matrix(3, 2, new int[6]{1, 1, 0, 0, 1, 1})
		);
	Pieces[5] = *new Piece(
		new Matrix(2, 3, new int[6]{1, 0, 1, 1, 0, 1}),
		new Matrix(3, 2, new int[6]{0, 1, 1, 1, 1, 0}),
		new Matrix(2, 3, new int[6]{1, 0, 1, 1, 0, 1}),
		new Matrix(3, 2, new int[6]{0, 1, 1, 1, 1, 0})
		);
	Pieces[6] = *new Piece(
		new Matrix(2, 3, new int[6]{1, 0, 1, 1, 1, 0}),
		new Matrix(3, 2, new int[6]{0, 1, 0, 1, 1, 1}),
		new Matrix(2, 3, new int[6]{0, 1, 1, 1, 0, 1}),
		new Matrix(3, 2, new int[6]{1, 1, 1, 0, 1, 0})
		);
}

/*t=Type i=Rotation j=X Location*/
int TetrisSim::addPiece(int t, int i, int j){
	Matrix *tP = Pieces[t].getPieceMatrix(i);
	tP->display();

	if (j + tP->getWidth() > board->getWidth()) return 0; // Failed Placement

	int h = board->getHeight();
	int c=0; //Where the Lower Left corner will be placed on the Y Axis
	for (c = 0; c < h-1; c++){
		for (int k = 0; k < tP->getWidth(); k++){
			if (tP->getValueAt(tP->getHeight() - 1, k) == 0)break;
			if (board->matrix[c*board->getWidth() + j + k] != 0) { 
				c--;
				goto stop;
			}//matrix[row*width + col]
		}
	}
	stop:

	int zeroes = 0;
	for (int k = tP->getHeight()-1; k >= 0; k--){
		if (tP->getValueAt(0, k) == 1)break;
		c--;
		zeroes++;
	}

	if (c + zeroes > board->getHeight()) return 0; //Failed Placement

	for (int Col = 0; Col < tP->getWidth(); Col++){
		for (int Row = 0; Row < tP->getHeight(); Row++){
			board->addtoValueAt(c - Row, j + Col, tP->getValueAt( (tP->getHeight() - 1) - Row, Col));
			if (board->getValueAt(c - Row, j + Col) > 1) return 0; //Failed Placement
		}
	}
	board->display();
}
/*0
{{1,1},
{1,1}}
*/

/*1
{{0,1},
{0,1},
{1,1}}

{{1,1,1},
{0,0,1}}

{{1,1},
{1,0},
{1,0}}

{{1,0,0},
{1,1,1}}
*/

/*2
{{1,0},
{1,0},
{1,1}}

{{0,0,1},
{1,1,1}}

{{1,1},
{0,1},
{0,1}}

{{1,1,1},
{1,0,0}}
*/

/*3
{{1},
{1},
{1},
{1}}

{1,1,1,1}
*/

/*4
{{0,1},
{1,1},
{1,0}}

{{1,1,0},
{0,1,1}}
*/

/*5
{{1,0},
{1,1},
{0,1}}

{{0,1,1},
{1,1,0}}
*/

/*6
{{1,0},
{1,1},
{1,0}}

{{0,1,0},
{1,1,1}}

{{0,1},
{1,1},
{0,1}}

{{1,1,1},
{0,1,0}}
*/