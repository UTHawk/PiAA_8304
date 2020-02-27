#include "TableTop.h"


TableTop::TableTop(int length)
{
	this->length = length;
	
	bufSquare = new Square*[length];
    minSquare = new Square*[length];

    for (auto i = 0; i < length; ++i) {
        bufSquare[i] = new Square[length];
        minSquare[i] = new Square[length];
	}

	minSquareNum = length * length;
	colorCount = 0;
	squareCount = 0;
}


TableTop::~TableTop()
{
    for (auto i = 0; i < length; ++i) {
        delete [] bufSquare[i];
        delete [] minSquare[i];
    }

    delete [] bufSquare;
    delete [] minSquare;
}


void TableTop::startBacktracking()
{
	paintSquare(0, 0, length/2 + 1);
	paintSquare(length / 2 + 1, 0, length / 2);
    paintSquare(0, length / 2 + 1, length / 2); //оптимизация алгоритма, оставляем 1/4 часть квадрата

	backtracking(length/2, length/2, length/2);
	writeRes();
}


void TableTop::paintSquare(int x, int y, int length)
{
    ++squareCount;
    for (auto i = y; i < y + length; ++i) {
        for (auto j = x; j < x + length; ++j) {
			bufSquare[i][j].size = length;
			bufSquare[i][j].number = squareCount;
		}
	}

	colorCount++;
}


void TableTop::printSquare(Square** square)
{
	std::cout << "________________________________" << std::endl;
    for (auto i = 0; i < length; ++i) {
        for (int j = 0; j < length; ++j) {
			std::cout << square[i][j].number << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "________________________________" << std::endl;
}


void TableTop::backtracking(int length, int x, int y)
{
	if (squareCount >= minSquareNum) {
		return;
	}

	int savedX;
	int savedY;
	
	if (findAvaibleCoord(x, y, savedX, savedY)) {
        for (auto len = length; len > 0; --len) {
			if (canPaintSquare(savedX, savedY, len)) {
				backtracking(length, x, savedY);
				clearSquare(savedX, savedY, len);
			}
		}
		return;
	}
	checkMinSquare();
}


bool TableTop::findAvaibleCoord(int x, int y, int& savedX, int& savedY)
{
    for (auto i = y; i < length; ++i) {
        for (auto j = x; j < length; ++j) {
			if (bufSquare[i][j].size == 0) {
				savedX = j;
				savedY = i;
				return true;
			}
		}
	}
	return false;
}


bool TableTop::canPaintSquare(int x, int y, int length)
{
	if (x + length > this->length || y + length > this->length) {
		return false;
	}

    for (auto i = y; i < y + length; ++i) {
        for (int j = x; j < x + length; ++j) {
			if (bufSquare[i][j].size != 0) {
				return false;
			}
		}
	}

	paintSquare(x, y, length);

	return true;
}


void TableTop::clearSquare(int x, int y, int len)
{
    for (auto i = y; i < y + len; ++i) {
        for (int j = x; j < x + len; ++j) {
			bufSquare[i][j].number = 0;
			bufSquare[i][j].size = 0;
		}
	}
    --squareCount;
}


void TableTop::checkMinSquare()
{
	if (squareCount < minSquareNum) {
		minSquareNum = squareCount;

        for (auto i = 0; i < length; ++i) {
            for (int j = 0; j < length; ++j) {
				minSquare[i][j] = bufSquare[i][j];
			}
		}
	}
}


void TableTop::writeRes()
{
    std::cout << minSquareNum << std::endl;

    for (auto i = 1; i <= minSquareNum; ++i) {
        for (auto y = 0; y < length; ++y) {

			int len = 0;
            for (auto x = 0; x < length; ++x) {
				if (minSquare[y][x].number == i) {
					len = minSquare[y][x].size;
					std::cout << x + 1 << " " << y + 1 << " " << len << std::endl;
					break;
				}
			}

			if (len) {
				break;
			}
		}
	}
}
