#pragma once
class Point
{
private:
	int row;
	int col;
public:

	// Constructors
	Point()
	{
		row = 0;
		col = 0;
	}

	Point(const int row,const int col)
	{
		this->row = row;
		this->col = col;
	}

	// Overloaded operators for Point
	Point operator+(const Point& point)
	{
		Point p((this->row + point.row), (this->col + point.col));

		return p;
	}

	bool operator==(const Point& point)
	{
		if (this->row != point.row)
			return false;

		if (this->col != point.col)
			return false;

		return true;
	}

	void operator=(const Point& point)
	{
		this->row = point.row;
		this->col = point.col;
	}

	// Getters - Setters
	int getRow()
	{
		return row;
	}

	int getCol()
	{
		return col;
	}

	void setRow(const int r)
	{
		row = r;
	}

	void setCol(const int c)
	{
		col = c;
	}

};

