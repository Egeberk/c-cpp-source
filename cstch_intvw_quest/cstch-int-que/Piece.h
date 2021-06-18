#pragma once
#include <string>
#include "Point.h"
using namespace std;

class Piece
{
private:

	string ident;

	float value;

	Point coordinates;

	bool isThreatened;

public:

	Piece()
	{
		ident = "";
		value = 0;
		isThreatened = false;
	}


	// Getters - Setters
	string getIdent()
	{
		return ident;
	}

	void setIdent(const string i)
	{
		ident = i;
	}

	float getValue()
	{
		return isThreatened ? (value / 2) : value;
	}

	void setValue(const int v)
	{
		value = v;
	}

	Point getCoord()
	{
		return coordinates;
	}

	void setCoord(const int row, const int col)
	{
		coordinates.setRow(row);
		coordinates.setCol(col);
	}

	void setThreatened(bool t)
	{
		isThreatened = t;
	}
};

