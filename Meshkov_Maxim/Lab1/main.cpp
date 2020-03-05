#include <iostream>
#include <vector>

bool verboseMode = false;

struct Square {
    int x, y, size;
};

class Area;
void printArea(const Area &area);

struct Area {
    int n;
    Area(int n) : n(n), filled(n, 0) {}
    std::vector<int> filled; // key is column, value is rows from top
    std::vector<Square> squares, minSquares;
    int minSquaresAmount = 0;

    int getToppestCol() {
        int min = n;
        int minI = -1;
        for (int i = 0; i < n; i++) {
            if (filled[i] < min) {
                minI = i;
                min = filled[i];
            }
        }
        return minI;
    }
    int toBelow(int col) {
        return n - filled[col];
    }
    int toRight(int col) {
        int endCol = col + 1;
        int row = filled[col];
        for ( ; endCol < n; endCol++) {
            if (filled[endCol] > row)
                break;
        }
        return endCol - col;
    }
    void addSquare(int topLeftCol, int size) {
        if (verboseMode) {
            std::cout << "Adding square " << topLeftCol << ' ' << filled[topLeftCol] << ' ' << size << std::endl;
        }
        squares.push_back({topLeftCol, filled[topLeftCol], size});
        for (int i = 0; i < size; i++) {
            filled[topLeftCol + i] += size;
        }
        if (verboseMode) {
            printArea(*this);
        }
    }
    void removeSquare(int topLeftCol, int size) {
        if (verboseMode) {
            std::cout << "Removing square " << topLeftCol << ' ' << filled[topLeftCol] << ' ' << size << std::endl;
        }
        for (int i = 0; i < size; i++) {
            filled[topLeftCol + i] -= size;
        }
        squares.pop_back();
        if (verboseMode) {
            printArea(*this);
        }
    }
    void saveMinSquares() {
        if (squares.size() < minSquaresAmount) {
            minSquaresAmount = squares.size();
            minSquares = squares;
            if (verboseMode) {
                std::cout << "Saving current squares as minimal (" << squares.size() << " squares)." << std::endl;
            }
        }
    }
};

void printArea(const Area &area) {
    std::vector<std::vector<unsigned char>> toPrint(area.n, std::vector<unsigned char>(area.n, '.'));
    for (auto i = 0; i < area.squares.size(); i++) {
        auto square = area.squares[i];
        for (int row = square.y; row < square.y + square.size; row++) {
            for (int col = square.x; col < square.x + square.size; col++) {
                toPrint[row][col] = (i < 26 ? 'a' + i : 'A' + (i - 26));
            }
        }
    }
    for (int row = 0; row < area.n; row++) {
        for (int col = 0; col < area.n; col++) {
            std::cout << toPrint[row][col] << ' ';
        }
        std::cout << std::endl;
    }
}

void printSquares(const std::vector<Square> &squares) {
    std::cout << squares.size() << std::endl;
    for (auto square : squares) {
        std::cout << square.x << " " << square.y << " " << square.size << std::endl;
    }
}

decltype(Area::squares) findMinSquares(int n);

int main(int argc, char *argv[]) {
    if (argc != 1) {
        verboseMode = true;
    }
    int n;
    std::cin >> n;
    auto squares = findMinSquares(n);
    if (verboseMode) {
        std::cout << "Solution: " << std::endl;
    }
    printSquares(squares);
    return 0;
}

void findMinSquaresRecursively(Area &area);

decltype(Area::squares) findMinSquares(int n) {
    Area area(n);
    area.minSquaresAmount = (n + 3) + 1; // +1 to record squares
    if (n % 2 != 0 && n % 3 != 0 && n % 5 != 0) {
        int size = n / 2 + 1;
        area.addSquare(0, size);
        area.addSquare(size, size - 1);
        area.addSquare(0, size - 1);
        findMinSquaresRecursively(area);
    }
    else {
        if (n % 2 == 0) area.minSquaresAmount = 4 + 1; // +1 to record squares
        else if (n % 3 == 0) area.minSquaresAmount = 6 + 1; // +1 to record squares
        else if (n % 5 == 0) area.minSquaresAmount = 8 + 1; // +1 to record squares
        findMinSquaresRecursively(area);
    }
    return area.minSquares;
}

void findMinSquaresRecursively(Area &area) {
    int toppestCol = area.getToppestCol();
    if (toppestCol == -1) {
        area.saveMinSquares();
        return;
    }

    if (area.squares.size() >= area.minSquaresAmount - 1)
        return;

    int maxSize = std::min(area.toBelow(toppestCol), area.toRight(toppestCol));
    maxSize = std::min(maxSize, area.n - 1);

    for (int size = maxSize; size >= 1; size--) {
        area.addSquare(toppestCol, size);
        findMinSquaresRecursively(area);
        area.removeSquare(toppestCol, size);
    }
}

