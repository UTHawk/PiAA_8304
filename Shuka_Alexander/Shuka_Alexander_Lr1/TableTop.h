#pragma once
#include "Square.h"
#include <iostream>


class TableTop
{
public:
    TableTop(int length);
    ~TableTop();
    void startBacktracking();

private:
	void paintSquare(int x, int y, int length);
	void printSquare(Square** square);
	void backtracking(int length, int x, int y);
	bool findAvaibleCoord(int x, int y, int& savedX, int& savedY);
	bool canPaintSquare(int x, int y, int length);
	void clearSquare(int x, int y, int len);
	void checkMinSquare();
	void writeRes();

private:
    Square** bufSquare;
    Square** minSquare;
    int length;
    int minSquareNum;
    int colorCount;
    int squareCount;
};

