#include <iostream>
#include "chrono"
#include "fstream"
#include <iomanip>

struct Point {
    int size = 0;
    int number = 0;
};

class Square {
    int N = 0;
    Point** matrix = nullptr;
    Point** bestMatrix = nullptr;
    int current_count = 0;
    int min_count = 0;

public:
    void printMatrix(Point** matrix) {

        int size;
        std::cout << min_count << std::endl;

        for (int i = 1; i <= min_count; i++) { // Идём по порядковым номерам квадратов
            for (int y = 0; y < N; y++) {
                size = 0;
                for (int x = 0; x < N; x++) {
                    if (matrix[y][x].number == i) { // Если у точки на поле порядковый номер совпадает с искомым
                        size = matrix[y][x].size;
                        std::cout << x + 1 << " " << y + 1 << " " << size << std::endl;
                        break; // Пропускаем перебор по строке (т.к. квадрат уже найден)
                    }
                }
                if (size) // Если квадрат был найден, пропускаем перебор по строкам,
                    break; // и переходим к поиску следующего квадрата (i+1)
            }
        }
    }

    explicit Square(int N) : N(N), current_count(0), min_count(1600), matrix(new Point* [N]), bestMatrix(new Point* [N]) {

        for (int i = 0; i < N; i++) {
            matrix[i] = new Point [N];
            bestMatrix[i] = new Point [N];

            for (int j = 0; j < N; j++){
                matrix[i][j].size = 0;
                matrix[i][j].number = 0;
            }
        }
    }

    void printIntermediateMatrix(Point** matrix) {
        std::cout << "****************************************************************\n";
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                std::cout << matrix[i][j].number << "    ";
            }
            std::cout << std::endl;
        }
        std::cout << "****************************************************************\n";
    }

    void fill() {
//        std::cout << "N: " << N << std::endl;
        if(!(N % 2) || !(N % 3) || !(N % 5)) {          // Обработка частных случаев длин сторон квадратов, кратным двум, трём, пяти
            if (!(N % 2)) {
                int N1 = N / 2;
                setSquare(0, 0, N1);
                printIntermediateMatrix(matrix);
                setSquare(N1, 0, N1);
                printIntermediateMatrix(matrix);
                setSquare(0, N1, N1);
                printIntermediateMatrix(matrix);
                setSquare(N1, N1, N1);
            }
            else if (!(N % 3)) {
                int N1 = N / 3;
                setSquare(0, 0, 2 * N1);
                printIntermediateMatrix(matrix);
                setSquare(2 * N1, 0, N1);
                printIntermediateMatrix(matrix);
                setSquare(0, 2 * N1, N1);
                printIntermediateMatrix(matrix);
                setSquare(2 * N1, N1, N1);
                printIntermediateMatrix(matrix);
                setSquare(N1, 2 * N1, N1);
                printIntermediateMatrix(matrix);
                setSquare(2 * N1, 2 * N1, N1);
            }
            else if (!(N % 5)) {
                int N1 = N / 5;
                setSquare(0, 0, 3 * N1);
                printIntermediateMatrix(matrix);
                setSquare(3 * N1, 0, 2 * N1);
                printIntermediateMatrix(matrix);
                setSquare(3 * N1, 3 * N1, 2 * N1);
                printIntermediateMatrix(matrix);
                setSquare(0, 3 * N1, 2 * N1);
                printIntermediateMatrix(matrix);
                setSquare(2 * N1, 3 * N1, N1);
                printIntermediateMatrix(matrix);
                setSquare(2 * N1, 4 * N1, N1);
                printIntermediateMatrix(matrix);
                setSquare(3 * N1, 2 * N1, N1);
                printIntermediateMatrix(matrix);
                setSquare(4 * N1, 2 * N1, N1);
            }
            setBestConfiguration();
            printMatrix(bestMatrix);
            printIntermediateMatrix(bestMatrix);
            return;
        }
        setSquare(0, 0, N / 2 + 1);
        setSquare(N / 2 + 1, 0, N / 2);
        setSquare(0, N / 2 + 1, N / 2);

        squaring(N / 2, N / 2, N / 2);
        printMatrix(bestMatrix);
        printIntermediateMatrix(bestMatrix);
    }

private:

    bool isFreePointOnMatrix(int &x, int &y, int xLimit, int yLimit) {
        for (int i = yLimit; i < N; i++) {
            for (int j = xLimit; j < N; j++) {
                if (matrix[i][j].size == 0) {
                    x = j;
                    y = i;
                    return true;
                }
            }
        }
        return false;
    }

    void setSquare(int x, int y, int size) {
        current_count++;
        for (int i = y; i < y + size; i++)
            for (int j = x; j < x + size; j++) {
                matrix[i][j].size = size;
                matrix[i][j].number = current_count;

            }
    }

    void deleteSquare(int x, int y, int size) {
        for (int i = y; i < y + size; i++)
            for (int j = x; j < x + size; j++) {
                matrix[i][j].size = 0;
                matrix[i][j].number = 0;
            }
        current_count--;
    }

    bool isHasFreeSpace(int x, int y, int size) {
        if ((x + size > N) || (y + size > N))
            return false;
        for (int i = y; i < y + size; i++) {
            for (int j = x; j < x + size; j++) {
                if (matrix[i][j].size != 0) {
                    return false;
                }
            }
        }
        setSquare(x, y, size);
        return true;
    }

    void setBestConfiguration() {
        if (current_count < min_count) {
            for (int i = 0; i < N; i++){
                for (int j = 0; j < N; j++){
                    bestMatrix[i][j] = matrix[i][j];
                }
            }
            min_count = current_count;
        }
        printIntermediateMatrix(matrix);
    }

    void squaring(int xLimit, int yLimit, int size) {
        if (current_count >= min_count) {
            return;
        }
        int x = 0;
        int y = 0;

        if (isFreePointOnMatrix(x, y, xLimit, yLimit)) {
            for (int possibleSize = 1; possibleSize < size; possibleSize++) {
                if (isHasFreeSpace(x, y, possibleSize)) {
                    squaring(xLimit, y, size);
                    deleteSquare(x, y, possibleSize);
                }
            }
            return;
        }
        setBestConfiguration();
    }
};

int main() {
    int N = 0;
    std::cin >> N;
    Square square(N);
    square.fill();
    return 0;
}
