#ifndef SQUARE_H
#define SQUARE_H
#include "point.h"

class square
{
public:
    Point* point;
    int width;
    square(Point* point, int width);
    ~square();
    square* copy();
};

#endif // SQUARE_H
