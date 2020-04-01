#include "square.h"

square::square(Point* point, int width) {
    this->point = point;
    this->width = width;
}
square::~square() {
    delete (this->point);
}

square* square::copy() {
    return new square(new Point(this->point->x, this->point->y), this->width);
}
