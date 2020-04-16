#include "backtrack.hpp"


Track::Track(int length)
{
    this->length = length;
    
    tempSquare = new Square*[length];
    minimalSquare = new Square*[length];

    for (auto i = 0; i < length; ++i) {
        tempSquare[i] = new Square[length];
        minimalSquare[i] = new Square[length];
    }

    colorCount = 0;
    minSquareNum = length * length;
    squareCount = 0;
}


Track::~Track()
{
    for (auto i = 0; i < length; ++i) {
        delete [] tempSquare[i];
        delete [] minimalSquare[i];
    }

    delete [] tempSquare;
    delete [] minimalSquare;
}


void Track::startBacktracking()
{
    paintSquare(0, 0, length/2 + 1);
    paintSquare(length / 2 + 1, 0, length / 2);
    paintSquare(0, length / 2 + 1, length / 2); //optimization of the algorithm , leaving 1/4 of the square
    
    backtracking(length/2, length/2, length/2);
    writeRes();
}


void Track::paintSquare(int x, int y, int length)
{
    ++squareCount;
    for (auto i = y; i < y + length; ++i) {
        for (auto j = x; j < x + length; ++j) {
            tempSquare[i][j].size = length;
            tempSquare[i][j].number = squareCount;
        }
    }
    
    colorCount++;

    if (this->length <= 10) {

        std::cout << "Process " << colorCount << std::endl;
        printSquareProcess(tempSquare);
    }
}



void Track::backtracking(int length, int x, int y)
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


bool Track::findAvaibleCoord(int x, int y, int& savedX, int& savedY)
{
    for (auto i = y; i < length; ++i) {
        for (auto j = x; j < length; ++j) {
            if (tempSquare[i][j].size == 0) {
                savedX = j;
                savedY = i;
                return true;
            }
        }
    }
    return false;
}


bool Track::canPaintSquare(int x, int y, int length)
{
    if (x + length > this->length || y + length > this->length) {
        return false;
    }

    for (auto i = y; i < y + length; ++i) {
        for (int j = x; j < x + length; ++j) {
            if (tempSquare[i][j].size != 0) {
                return false;
            }
        }
    }

    paintSquare(x, y, length);
    return true;
}


void Track::clearSquare(int x, int y, int len)
{
    for (auto i = y; i < y + len; ++i) {
        for (int j = x; j < x + len; ++j) {
            tempSquare[i][j].number = 0;
            tempSquare[i][j].size = 0;
        }
    }
    --squareCount;
}


void Track::checkMinSquare()
{
    if (squareCount < minSquareNum) {
        minSquareNum = squareCount;

        for (auto i = 0; i < length; ++i) {
            for (int j = 0; j < length; ++j) {
                minimalSquare[i][j] = tempSquare[i][j];
            }
        }
    }
}

void Track::printSquareProcess(Square** square)
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


void Track::writeRes()
{
    std::cout << "\nResult : \n";

    printSquareProcess(minimalSquare);

    std::cout << "Minimal square: "<< minSquareNum << std::endl;
    for (auto i = 1; i <= minSquareNum; ++i) {
        for (auto y = 0; y < length; ++y) {

            int len = 0;
            for (auto x = 0; x < length; ++x) {
                if (minimalSquare[y][x].number == i) {
                    len = minimalSquare[y][x].size;
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

void Track::optimalSolution() {
    int n = length;
    
    if (n % 2 == 0) {
        std::cout << "4\n";
        std::cout << "1 1 " << n/2 << std::endl;
        std::cout << 1 + n/2 << " 1 " << n/2 << std::endl;
        std::cout << "1 " << 1 + n/2 << " " << n/2 << std::endl;
        std::cout << 1 + n/2 << " " << 1 + n/2 << " " << n/2 << std::endl;
    }
    else if (n % 3 == 0) {
        std::cout << "6" << std::endl;
        std::cout << "1 1 " << 2 * n / 3 << std::endl;
        std::cout << 1 + 2 * n / 3 << " 1 " << n / 3 << std::endl;
        std::cout << "1 " << 1 + 2 * n / 3 << " " << n / 3 << std::endl;
        std::cout << 1 + 2 * n / 3 << " " << 1 + n / 3 << " " << n / 3 << std::endl;
        std::cout << 1 + n / 3 << " " << 1 + 2 * n / 3 << " " << n / 3 << std::endl;
        std::cout << 1 + 2 * n / 3 << " " << 1 + 2 * n / 3 << " " << n / 3 << std::endl;
    }
    else if (n % 5 == 0) {
        std::cout << "8" << std::endl;
        std::cout << "1 1 " << 3 * n / 5 << std::endl;    
        std::cout << 1 + 3 * n / 5 << " 1 " << 2 * n / 5 << std::endl;
        std::cout << "1 " << 1 + 3 * n / 5 << " " << 2 * n / 5 << std::endl;
        std::cout << 1 + 3 * n / 5 << " " << 1 + 3 * n / 5 << " " << 2 * n / 5 << std::endl;
        std::cout << 1 + 2 * n / 5 << " " << 1 + 3 * n / 5 << " " << n / 5 << std::endl;
        std::cout << 1 + 2 * n / 5 << " " << 1 + 4 * n / 5 << " " << n / 5 << std::endl;
        std::cout << 1 + 3 * n / 5 << " " << 1 + 2 * n / 5 << " " << n / 5 << std::endl;
        std::cout << 1 + 4 * n / 5 << " " << 1 + 2 * n / 5 << " " << n / 5 << std::endl;
    }
    std::cout << "\nConstant Time: O(1)\n" << std::endl;
    
}
