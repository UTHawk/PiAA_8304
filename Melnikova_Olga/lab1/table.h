#ifndef TABLE_H
#define TABLE_H
#include "square.h"
#include <cmath>

class table
{
private:
    int** _array;                       //для 0 и 1
    int _emptySpace;                    //кол-во 0
    int _squaresCount;                  //кол-во
    square** _squares;                  //
    void clearTable();

public:
    int width;
    square** minSquares;                //минимальный массив квадратов
    table (int width);
    ~table();
    int pushSquare(square* square);
    square* popSquare();
    Point* getNextEmptyPoint();
    bool isFull();
    square* getMaximumSquare(Point* point);
    int getMinimumCountOfSquares();
};

#endif // TABLE_H
