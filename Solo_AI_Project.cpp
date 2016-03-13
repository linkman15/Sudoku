// Solo_AI_Project.cpp : Sodoku
//

#include "stdafx.h"
using namespace std;

// 
//

#define ROWS 9
#define COLS 9
#define SQUARE 9

/*struct square {
	int square_num;
	Cell cells[SQUARE];
};*/

class Cell {
	int number;
	int row;
	int col;
	int square;
	bool locked;
public:
	Cell();
	Cell(int n, int r, int c, int s, bool lock);
	void update_number(int num);
	void switch_lock();
	int get_number();
	int get_row();
	int get_col();
	int get_square();
	bool get_lock();
};

Cell::Cell() {
	number = 0;
	row = 0;
	col = 0;
	square = 0;
	locked = false;
}

Cell::Cell(int n, int r, int c, int s, bool lock) {
	number = n;
	row = r;
	col = c;
	square = s;
	locked = lock;
}

void Cell::update_number(int num) {
	if (!locked) {
		number = num;
	}
}

void Cell::switch_lock() {
	locked = !locked;
}

int Cell::get_number() {
	return number;
}

int Cell::get_row() {
	return row;
}

int Cell::get_col() {
	return col;
}

int Cell::get_square() {
	return square;
}

bool Cell::get_lock() {
	return locked;
}

class Grid {
	Cell rows[ROWS][COLS];
	
public:
	Grid();
	Grid(int start[ROWS][COLS]);
	void display_grid();
	bool is_unlocked(int row, int col);
	void update_cell(int row, int col, int num);
	int get_square_loc(int row, int col);
	Cell get_cell(int row, int col);
};

Grid::Grid() {
	//Cell basic = Cell();
	//Fill rows, cols, and squares with 0s
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			//rows[i][j] = basic;
			rows[i][j] = Cell(0, i+1, j+1, get_square_loc(i, j), false);
		}
	}
}

Grid::Grid(int start[ROWS][COLS]) {
	//rows = start;
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			//rows[i][j] = start[i][j];
			if (start[i][j] == 0) {
				rows[i][j] = Cell(0, i + 1, j + 1, get_square_loc(i, j), false);
			}
			else {
				rows[i][j] = Cell(start[i][j], i + 1, j + 1, get_square_loc(i, j), true);
			}
		}
	}
}

Cell Grid::get_cell(int row, int col) {
	return rows[row][col];
}

bool Grid::is_unlocked(int row, int col) {
	return rows[row][col].get_lock();
}

void Grid::update_cell(int row, int col, int num) {
	rows[row][col].update_number(num);
	printf("Row: %d, Col: %d, Square: %d\n", rows[row][col].get_row(), rows[row][col].get_col(), rows[row][col].get_square());
}

int Grid::get_square_loc(int row, int col) {
	int r_loc, c_loc;

	if (row < 3) { //Rows 1-3
		r_loc = 1;
	}
	else if (row >= 3 && row < 6) { //Rows 4-6
		r_loc = 2;
	}
	else { //Rows 7-9
		r_loc = 3;
	}

	if (col < 3) { //Colomns 1-3
		c_loc = 1;
	}
	else if (col >= 3 && col < 6) { //Cols 4-6
		c_loc = 2;
	}
	else { //Rows 7-9
		c_loc = 3;
	}

	switch (r_loc) {
	case 1:
		switch (c_loc) {
		case 1:
			return 1;
			break;
		case 2:
			return 2;
			break;
		case 3:
			return 3;
			break;
		}
		break;
	case 2:
		switch (c_loc) {
		case 1:
			return 4;
			break;
		case 2:
			return 5;
			break;
		case 3:
			return 6;
			break;
		}
		break;
	case 3:
		switch (c_loc) {
		case 1:
			return 7;
			break;
		case 2:
			return 8;
			break;
		case 3:
			return 9;
			break;
		}
		break;
	}
	return 0;
}

void Grid::display_grid() {
	int i, j;
	printf("ROWS\n");
	for (i = 0; i < ROWS; i++) {		
		if (i % 3 == 0) {
			printf("      +++++++++++++++++++++++++++++++++++++\n   ");
		}
		else {
			printf("      +-----------+-----------+-----------+\n   ");
		}
		printf("%d  ", i+1);
		for (j = 0; j < COLS; j++) {
			if (j % 3 == 0) {
				printf("+");
			}
			else {
				printf("|");
			}
			if (rows[i][j].get_lock() == true) {
				printf("'%d'", rows[i][j].get_number());
			}
			else {
				printf(" %d ", rows[i][j].get_number());
			}
		}
		printf("+\n");
	}
	printf("      +++++++++++++++++++++++++++++++++++++\n");
	printf("        1   2   3   4   5   6   7   8   9 COLS\n");
}

class AI {
public:
	bool constrant_check(Grid g, Cell c);
	bool complete_puzzle(Grid g);
};

bool AI::constrant_check(Grid g, Cell c) {
	bool result = true;
	int i;
	int num = c.get_number();
	int rw = c.get_row()-1;
	int cl = c.get_col()-1;
	int sq = c.get_square();
	//Check that cell number is between 1-9
	if (num < 1 || num > 9) {
		printf("The cell number is not between one and nine\n");
		result = false;
	}

	if(result) printf("Number check successful\n");

	//Check that the numbers in the Cell's row are different
	for (i = 0; i < COLS; i++) {
		Cell curr = g.get_cell(rw, i);
		if (c.get_number() == curr.get_number() && cl != i) {
			printf("Two Cells in row %d have the same number\n", rw+1);
			result = false;
			break;
		}
	}

	if (result) printf("Row check successful\n");

	//Check that the numbers in the Cell's column are different
	for (i = 0; i < ROWS; i++) {
		Cell curr = g.get_cell(i, cl);
		if (c.get_number() == curr.get_number() && rw != i) {
			printf("Two Cells in column %d have the same number\n", cl+1);
			result = false;
			break;
		}
	}

	if (result) printf("Column check successful\n");

	//Check that the numbers in the Cell's square are different

	int j;
	switch (sq) {
	case 1:
		i = 0;
		j = 0;
		break;
	case 2:
		i = 0;
		j = 3;
		break;
	case 3:
		i = 0;
		j = 6;
		break;
	case 4:
		i = 3;
		j = 0;
		break;
	case 5:
		i = 3;
		j = 3;
		break;
	case 6:
		i = 3;
		j = 6;
		break;
	case 7:
		i = 6;
		j = 0;
		break;
	case 8:
		i = 6;
		j = 3;
		break;
	case 9:
		i = 6;
		j = 6;
		break;
	}

	int i_to = i + 3;
	int j_to = j + 3;

	//printf("i: %d, j: %d\n", i, j);
	//printf("i_to: %d, j_to: %d\n", i_to, j_to);

	for (; i < i_to; i++) {
		for (j = j_to - 3; j < j_to; j++) {
			Cell curr = g.get_cell(i, j);
			//printf("square %d\n", curr.get_number());
			if (c.get_number() == curr.get_number() && rw != i && cl != j) {
				printf("Two Cells in square %d have the same number\n", sq);
				result = false;
				break;
			}
		}
	}

	return result;
}

bool AI::complete_puzzle(Grid g) {
	int i;
	int j;


	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			if (!constrant_check(g, g.get_cell(i,j))) {
				return false;
			}
		}
	}

	return true;
}

void show_rules() {
	printf("Objective: Fill every cell in the board with a number between 1-9.\n");
	printf("Each coloumn, row, and square must have different\n");
	printf("Cells with quotations are locked.\n");
}

int main()
{
	int rw, cl, nm;
	bool finished = false;
	Grid Sodoku; //= Grid();
	AI checker;
	printf("WELCOME TO SODOKU\n\n\n");
	printf("Select a puzzle\n");

	int start_1[ROWS][COLS] = {
		{ 5,3,0,0,7,0,0,0,0 },
		{ 6,0,0,1,9,5,0,0,0 },
		{ 0,9,8,0,0,0,0,6,0 },
		{ 8,0,0,0,6,0,0,0,3 },
		{ 4,0,0,8,0,3,0,0,1 },
		{ 7,0,0,0,2,0,0,0,6 },
		{ 0,6,0,0,0,0,2,8,0 },
		{ 0,0,0,4,1,9,0,0,5 },
		{ 0,0,0,0,8,0,0,7,9 }
	};

	int start_2[ROWS][COLS] = {
		{ 0,2,0,0,0,4,3,0,0 },
		{ 9,0,0,0,2,0,0,0,8 },
		{ 0,0,0,6,0,9,0,5,0 },
		{ 0,0,0,0,0,0,0,0,1 },
		{ 0,7,2,5,0,3,6,8,0 },
		{ 6,0,0,0,0,0,0,0,0 },
		{ 0,8,0,2,0,5,0,0,0 },
		{ 1,0,0,0,9,0,0,0,3 },
		{ 0,0,9,8,0,0,0,6,0 }
	};

	int start_3[ROWS][COLS] = {
		{ 0,0,0,0,7,0,1,0,0 },
		{ 0,0,0,9,0,0,0,0,2 },
		{ 3,4,0,0,0,8,0,0,0 },
		{ 6,7,1,0,0,0,0,2,0 },
		{ 0,0,5,0,1,0,9,0,0 },
		{ 0,2,0,0,0,0,6,8,1 },
		{ 0,0,0,6,0,0,0,4,9 },
		{ 5,0,0,0,0,9,0,0,0 },
		{ 0,0,6,0,8,0,0,0,0 }
	};

	int win_test[ROWS][COLS] = {
		{ 5,3,4,6,7,8,9,1,2 },
		{ 6,7,2,1,9,5,3,4,8 },
		{ 1,9,8,3,4,2,5,6,7 },
		{ 8,5,9,7,6,1,4,2,3 },
		{ 4,2,6,8,5,3,7,9,1 },
		{ 7,1,3,9,2,4,8,5,6 },
		{ 9,6,1,5,3,7,2,8,4 },
		{ 2,8,7,4,1,9,6,3,5 },
		{ 3,4,5,0,8,0,0,7,9 }
	};

	while (1) {
		printf("1: Puzzle 1\n");
		printf("2: Puzzle 2\n");
		printf("3: Puzzle 3\n");
		nm = NULL;
		scanf("%d", &nm);
		if (nm == 1) {
			Sodoku = Grid(start_1);
			break;
		}
		else if (nm == 2){
			//Sodoku = Grid();
			Sodoku = Grid(start_2);
			break;
		}
		else if (nm == 3) {
			Sodoku = Grid(start_3);
			break;
		}
		else if (nm == 4) {
			Sodoku = Grid(win_test);
			break;
		}
		else if (nm == 5) {
			Sodoku = Grid();
			break;
		}
		else {
			printf("Incorrect input\n");
		}
	}

	for (;;) {
		Sodoku.display_grid();

		while (1) {
			//break;
			printf("Select an option\n");
			printf("1: Enter a number	");
			printf("2: Check for a win\n");
			printf("3: Display board again	");
			printf("4: How to play\n");


			nm = NULL;
			scanf("%d", &nm);

			if (nm == 1)break;
			else if (nm == 2) {
				//Check for win
				finished = checker.complete_puzzle(Sodoku);
				if (finished) {
					break;
				}
				else {
					printf("Game is not complete\n");
				}
			}
			else if (nm == 3) {
				Sodoku.display_grid();
			}
			else if (nm == 4) {
				//Display rules
				show_rules();
			}
			else {
				printf("Not an option\n");
			}

		}

		if (finished) {
			break;
		}

		while (1) {
			printf("Choose a row: ");
			rw = NULL;
			scanf("%d", &rw);
			printf("Choose a column: ");
			cl = NULL;
			scanf("%d", &cl);
			if (rw < 1 || rw > 9 || cl < 1 || cl > 9) {
				printf("Not a square, pick again\n");
			}
			else if (Sodoku.is_unlocked(rw-1, cl-1)) {
				printf("Square is locked, pick again\n");
			}
			else {
				printf("Choosen square: %d,%d\n", rw, cl);
				break;
			}
		}
		while (1) {
			printf("Choose a new number between 1-9: ");
			nm = NULL;
			scanf("%d", &nm);
			if (nm < 0 || nm > 9) {
				printf("Incorrect input");
			}
			else {
				break;
			}
		}
		Sodoku.update_cell(rw-1, cl-1, nm);
		if (checker.constrant_check(Sodoku, Sodoku.get_cell(rw - 1, cl - 1))) {
			printf("Good\n");
		}

	}
	Sodoku.display_grid();

	printf("You have won Sodoku!!!");

    return 0;
}

