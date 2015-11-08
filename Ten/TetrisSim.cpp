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

	int w = board->getWidth();
	int h = board->getHeight();
	int tH = tP->getHeight();
	int tW = tP->getWidth();

	int ly = h; //Y level of the Lowest Part of a Piece
	int looking = true;
	while (looking){
		if (--ly < 0) return 0;
		for (int s = 0; s < tH && looking; s++){
			for (int t = 0; t < tW && looking; t++){
				if (tP->getValueAt(tH - 1 - s, t) + board->getValueAt(ly - s, t + j) > 1)goto next;
				else if (s == tH - 1 && t == tW - 1) looking = false;
			}
		}
		next:;
	}
	for (int s = 0; s < tH; s++){
		for (int t = 0; t < tW; t++){
			board->addtoValueAt(ly - s, t + j, tP->getValueAt(tH - 1 - s, t));
		}
	}

	int score = 0;
	int rowc = 0;
	for (int s = 0; s < tH; s++){
		for (int t = 0; t < w; t++){
			rowc+=board->getValueAt(ly - s, t);
		}
		if (rowc == w){ 
			score += 10; 
			rowCleared(ly - s);
			s--; tH--;
		}
		rowc = 0;
	}

	board->display();
	return score > 0 ? score : 1;
}

void TetrisSim::rowCleared(int r){
	for (int s = r; s >= 1; s--){
		for (int t = 0; t < board->width; t++){
			board->setValueAt(s, t, board->getValueAt(s-1, t));
		}
	}
}
/*0
00-03
{{1,1},
{1,1}}
*/

/*1
10
{{0,1},
{0,1},
{1,1}}

11
{{1,1,1},
{0,0,1}}

12
{{1,1},
{1,0},
{1,0}}

13
{{1,0,0},
{1,1,1}}
*/

/*2
20
{{1,0},
{1,0},
{1,1}}

21
{{0,0,1},
{1,1,1}}

22
{{1,1},
{0,1},
{0,1}}

23
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