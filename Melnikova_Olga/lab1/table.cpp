#include "table.h"

void table::clearTable() {
    for (int x = 0; x < this->width; x++)
        for (int y = 0; y < this->width; y++)
            _array[x][y] = 0;
    _squaresCount = 0;
    _emptySpace = width*width;
}

         //минимальный массив квадратов

table::table (int width) {
    this->width = width;
    _array = new int* [width];
    for (int i = 0; i < width; i++)
        _array[i] = new int[width];
    this->clearTable();               //заполнение 0
    _emptySpace = width*width;
    _squares = new square*[_emptySpace];   //стек квадратов
    _squaresCount = 0;
}
table::~table() {
    for (int i = 0; i < width; i++)
        delete(_array[i]);
    delete(_array);
    delete(_squares);
}

int table::pushSquare(square* square) {
    if (this->isFull()) return 0;
    for (int x = square->point->x; x < square->point->x + square->width; x++)      //по координатам заполняем квадрат _array единицами
        for (int y = square->point->y; y < square->point->y + square->width; y++)
            _array[x][y] = 1;
    delete(_squares[_squaresCount]);    //??????
    _squares[_squaresCount++] = square;  //добавляем в стек
    _emptySpace -= square->width * square->width;  //уменьшаем число свободных
    return 1;
}
square* table::popSquare() {
    if (_squaresCount == 0) return NULL;
    square* lastSquare = _squares[_squaresCount-1];
    for (int x = lastSquare->point->x; x < lastSquare->point->x + lastSquare->width; x++)  //по коорд. послед. в стеке заполняем нулями _array
        for (int y = lastSquare->point->y; y < lastSquare->point->y + lastSquare->width; y++)
            _array[x][y] = 0;
    _squaresCount--;
    _emptySpace += lastSquare->width * lastSquare->width;
    return lastSquare;
}

Point* table::getNextEmptyPoint() {  //нахождение пустой клетки
    for (int x = 0; x < this->width; x++)
        for (int y = 0; y < this->width; y++)
            if (_array[x][y] == 0)
                return new Point(x, y);
    return NULL;
}

bool table::isFull() {   //весь ли квадрат заполнен
    return !_emptySpace;
}

square* table::getMaximumSquare(Point* point) {   //заполнение максимальным квадратом
    int width = 1;
    bool flag = true;
    while (flag) {
        width++;
        flag = (point->x + width - 1 < this->width) &&  //не вышли ли за пределы
                (point->y + width - 1 < this->width);
        if (flag) {
            for (int i = 0; i < width; i++) {  //ищем макс. размер
                if ((_array[point->x + width - 1][point->y + i] == 1) ||
                    (_array[point->x + i][point->y + width - 1] == 1)) {
                    flag = false;
                    break;
                }for (int i = 0; i < width; i++) {  //ищем макс. размер
                    if ((_array[point->x + width - 1][point->y + i] == 1) ||
                        (_array[point->x + i][point->y + width - 1] == 1)) {
                        flag = false;
                        break;
                    }
                }
            }
        }
    }
    return new square(point, width - 1);
}


int table::getMinimumCountOfSquares() {
    int minimumCount = this->_emptySpace;  //изначально максимум
    minSquares = new square*[this->width * this->width];

    //оптимизация
    if ((width%2) == 0){
        pushSquare(new square(new Point(this->width / 2.0, this->width / 2.0), ceil(this->width / 2.0)));
        pushSquare(new square(new Point(ceil(this->width / 2.0), 0), this->width / 2));
        pushSquare(new square(new Point(0, ceil(this->width / 2.0)), this->width / 2));
    }
    else if ((width%3) == 0){
        pushSquare(new square(new Point(0, 0), this-> width * 2 / 3.0));
        pushSquare(new square(new Point(this-> width * 2 / 3.0, 0), this->width / 3.0));
        pushSquare(new square(new Point(0, this-> width * 2 / 3.0), this->width / 3.0));
    }
    else if ((width%5) == 0){
        pushSquare(new square(new Point(0, 0), this-> width * 3 / 5.0));
        pushSquare(new square(new Point(this-> width * 3 / 5.0, 0), this->width / 5.0));
        pushSquare(new square(new Point(0, this-> width * 3 / 5.0), this->width / 5.0));
    }
    else{
        pushSquare(new square(new Point(this->width / 2.0, this->width / 2.0), ceil(this->width / 2.0)));
        pushSquare(new square(new Point(ceil(this->width / 2.0), 0), this->width / 2));
        pushSquare(new square(new Point(0, ceil(this->width / 2.0)), this->width / 2));
    }

    //поиск наилучшего варианта
    while (!this->isFull()) {
        pushSquare(getMaximumSquare(getNextEmptyPoint()));
    }

    minimumCount = _squaresCount;

    for (int i = 0; i < _squaresCount; i++) {
        minSquares[i] = _squares[i]->copy();
    }

    if ( width==2) return minimumCount;

    int resizedSquareIndex;
    while ( 1 ) {

        resizedSquareIndex = _squaresCount - 1;  //удалили единичные квадраты
        while (_squares[resizedSquareIndex--]->width == 1)
            popSquare();
        resizedSquareIndex++;
        if (resizedSquareIndex < 3) break;

        square* poppedSquare = popSquare();  //замена след. мин. на квадрат с шириной-1
        pushSquare(new square(new Point(poppedSquare->point->x, poppedSquare->point->y), poppedSquare->width - 1));

        while (!this->isFull() && (_squaresCount < minimumCount)) {  //заполнение макс.
            pushSquare(getMaximumSquare(getNextEmptyPoint()));
        }

        if (_squaresCount < minimumCount) {  //если нашли минимум - запоминаем
            minimumCount = _squaresCount;
            for (int j = 0; j < _squaresCount; j++) {
                minSquares[j] = _squares[j]->copy();
            }
        }
    }
    return minimumCount;
}
