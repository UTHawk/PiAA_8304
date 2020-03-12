

#ifndef backtrack_hpp
#define backtrack_hpp

#include <iostream>
#include <stdio.h>


struct Square
{
    Square(){
        size = 0;
        number = 0;
    }

    int size;
    int number;
};

class Track
{
public:
    Track(int length);
    ~Track();
    void startBacktracking();
    void optimalSolution();

private:
    void paintSquare(int x, int y, int length);
    void backtracking(int length, int x, int y);
    bool findAvaibleCoord(int x, int y, int& savedX, int& savedY);
    bool canPaintSquare(int x, int y, int length);
    void clearSquare(int x, int y, int len);
    void checkMinSquare(); 
    void writeRes();

private:
    Square** tempSquare;
    Square** minimalSquare;
    int length;
    int minSquareNum;
    int squareCount;
};

#endif /* backtrack_hpp */
