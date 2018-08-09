#pragma once
#include <cstdlib>
#include <iostream>
#include <array>
#include <ctime>
using namespace std;

// Generates a finished sudoku automatically when called.

class MapGenerator
{
private:
	short int map[9][9];
	array<short int, 9>  baseLine{ 1,2,3,4,5,6,7,8,9 };
public:

	MapGenerator()
	{
		short int shiftCount;

		baseLineGenerator();

		arrCopy(0);

		for (int i = 1; i < 9; i++)
		{
			if (i == 3 || i == 6)
				shiftCount = 1;
			else
				shiftCount = 3;

			shifter(shiftCount);

			arrCopy(i);
		}
	}

	~MapGenerator()
	{

	}

	void baseLineGenerator()
	{
		srand(time(0));

		random_shuffle(baseLine.begin(), baseLine.end());
	}

	void shifter(const short int shiftCount)
	{
		for (int n = 0; n < shiftCount; n++)
		{
			short int firstElem = baseLine[0];

			for (int i = 0; i < 8; i++)
				baseLine[i] = baseLine[i + 1];

			baseLine[8] = firstElem;
		}
	}

	void print()
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
				cout << map[i][j] << "  ";

			cout << endl<<endl;
		}
	}

	void arrCopy(const int i)
	{
		for (int j = 0; j < 9; j++)
			map[i][j] = baseLine[j];
	}

	short int getMap()
	{
		return map[9][9];
	}


};

