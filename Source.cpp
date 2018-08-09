
#include "MapGenerator.h";

#include "SudokuSolver.h"

int main()
{
	SudokuSolver ss("C:\\Users\\Ege\\Desktop\\Sample Sudoku2.txt");
	ss.print();

	getchar();
	return 0;
}