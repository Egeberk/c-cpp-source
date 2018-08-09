#pragma once
#include <cstdlib>
#include <iostream>
#include <array>
#include <ctime>
using namespace std;

class Cell
{
private:
	short int row, column;
	short int value;
	bool initial; // true if value exists before the solve process
	short int block;
	short int backtrackCounter;
	bool classCreated;

public:
	Cell()
	{

	}

	Cell(const short int v, const bool i, const short int b, const short int bc, const short int r, const short int c)
	{
		classCreated = true;
		value = v;
		initial = i;
		block = b;
		backtrackCounter = bc;
		row = r;
		column = c;
	}
	~Cell()
	{

	}
	void setBacktrack(const short int bc)
	{
		backtrackCounter = bc;
	}
	const short int getBacktrack()
	{
		return backtrackCounter;
	}
	void setValue(const short int v)
	{
		value = v;
	}
	const short int getValue()
	{
		return value;
	}
	void setInitial(const bool i)
	{
		initial = i;
	}
	const bool getInitial()
	{
		return initial;
	}
	void setBlock(const short int b)
	{
		block = b;
	}
	const short int getblock()
	{
		return block;
	}
	void incrementBacktrack()
	{
		if(classCreated)
			backtrackCounter++;
	}
	short int getColumn()
	{
		return column;
	}
	void setColumn(short int c)
	{
		column = c;
	}
	short int getRow()
	{
		return row;
	}
	void setRow(short int r)
	{
		row = r;
	}
};

