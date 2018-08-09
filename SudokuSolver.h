#pragma once
#include <fstream>
#include <stack>
#include "Cell.h"
using namespace std;

class SudokuSolver
{
private:

	Cell map[9][9];
	stack<Cell> cells;

public:
	SudokuSolver(const string fileName)
	{

		parseMap(fileName);

		int startRow, startColumn;

			if (insertFirstCell(&startRow, &startColumn))
			{
				for (int i = startRow; i < 9; i++)
				{
					for (int j = startColumn; j < 9; j++)
					{
						if (!map[i][j].getInitial())
						{
							while (map[i][j].getBacktrack() <= 9)
							{
								if (isValid(map[i][j].getBacktrack(), i, j, map[i][j].getblock()))
								{
									map[i][j] = Cell(map[i][j].getBacktrack(), false, detectBlock(i, j), map[i][j].getBacktrack(), i, j);
									cells.push(map[i][j]);
									break;
								}
								map[i][j].incrementBacktrack();
							}
							if (map[i][j].getBacktrack() > 9)
							{
								map[i][j].setBacktrack(1);
								map[i][j].setValue(0);
								Cell discard = cells.top();
								i = discard.getRow(); j = discard.getColumn();
								map[i][j].incrementBacktrack();
								j--;
								cells.pop();
							}
						}
						map[startRow][startColumn].setInitial(false);
					}

				}
			}
			else
			{
				cout << endl << "Sudoku is unsolvable.";
			}
		}
		

	~SudokuSolver()
	{
		
	}

	bool insertFirstCell(int *stRow,int *stColumn) // return true if sudoku is solvable
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (map[i][j].getValue() == 0)
				{
					while (map[i][j].getBacktrack() <= 9)
					{
						if (isValid(map[i][j].getBacktrack(),i,j, map[i][j].getblock()))
						{
							map[i][j] = Cell(map[i][j].getBacktrack(), true, detectBlock(i,j), map[i][j].getBacktrack(), i, j);
							cells.push(map[i][j]);
							*stRow = i; *stColumn = j;
							return true;
						}
						map[i][j].incrementBacktrack();
					}
					return false;
				}
			}
		}
		cout << endl << "This sudoku is already finished.";
	}

	short int detectBlock(const short int i, const short int j)
	{
		if (j < 3 && i < 3) // block 1
			return 1;
		else if ((j >= 3 && j < 6) && i < 3) // block 2
			return 2;
		else if ((j >= 6 && j < 9) && i < 3) // block 3
			return 3;
		else if (j < 3 && (i >= 3 && i < 6)) // block 4
			return 4;
		else if ((j >= 3 && j < 6) && (i >= 3 && i < 6)) // block 5
			return 5;
		else if ((j >= 6 && j < 9) && (i >= 3 && i < 6)) // block 6
			return 6;
		else if (j < 3 && (i >= 6 && i < 9)) // block 7
			return 7;
		else if ((j >= 3 && j < 6) && (i >= 6 && i < 9)) // block 8
			return 8;
		else if ((j >= 6 && j < 9) && (i >= 6 && i < 9)) // block 9
			return 9;
	}

	bool isValid(const short int value, const short int row, const short int column, const short int block)
	{
		if (!horizontalCheck(value, row))
			return false;
		if (!verticalCheck(value, column))
			return false;
		if (!blockCheck(value, block))
			return false;

		return true;
	}

	bool horizontalCheck(const short int value, const short int row)
	{
		for (int j = 0; j < 9; j++)
			if (map[row][j].getValue() == value)
				return false;
		return true;
	}

	bool verticalCheck(const short int value, const short int column)
	{
		for (int i = 0; i < 9; i++)
			if (map[i][column].getValue() == value)
				return false;
		return true;
	}

	bool blockCheck(const short int value, const short int block)
	{
		if (block == 1)
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					if (map[i][j].getValue() == value)
						return false;
		}
		else if (block == 2)
		{
			for (int i = 0; i < 3; i++)
				for (int j = 3; j < 6; j++)
					if (map[i][j].getValue() == value)
						return false;
		}
		else if (block == 3)
		{
			for (int i = 0; i < 3; i++)
				for (int j = 6; j < 9; j++)
					if (map[i][j].getValue() == value)
						return false;
		}
		else if (block == 4)
		{
			for (int i = 3; i < 6; i++)
				for (int j = 0; j < 3; j++)
					if (map[i][j].getValue() == value)
						return false;
		}
		else if (block == 5)
		{
			for (int i = 3; i < 6; i++)
				for (int j = 3; j < 6; j++)
					if (map[i][j].getValue() == value)
						return false;
		}
		else if (block == 6)
		{
			for (int i = 3; i < 6; i++)
				for (int j = 6; j < 9; j++)
					if (map[i][j].getValue() == value)
						return false;
		}
		else if (block == 7)
		{
			for (int i = 6; i < 9; i++)
				for (int j = 0; j < 3; j++)
					if (map[i][j].getValue() == value)
						return false;
		}
		else if (block == 8)
		{
			for (int i = 6; i < 9; i++)
				for (int j = 3; j < 6; j++)
					if (map[i][j].getValue() == value)
						return false;
		}
		else if (block == 9)
		{
			for (int i = 6; i < 9; i++)
				for (int j = 6; j < 9; j++)
					if (map[i][j].getValue() == value)
						return false;
		}
		return true;
	}

	void parseMap(const string fn)
	{
		ifstream in;

		in.open(fn);

		if (in.fail())
			return;
		
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				short int val;
				in >> val;
				if (val == 0)
					map[i][j] = Cell(val, false, detectBlock(i,j), 1, i, j); // not initialy exists
				else
					map[i][j] = Cell(val, true, detectBlock(i, j), 0, i, j); // initially exists
			}
		}

		in.close();
		
	}

	void print()
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				cout << map[i][j].getValue() << " ";
			}
			cout << endl;
		}
	}
};

