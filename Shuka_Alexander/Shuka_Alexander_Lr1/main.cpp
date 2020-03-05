#include <iostream>
#include <Windows.h>
#include <ctime>

#include "TableTop.h"


bool isOptimalLength(int n);
void optimalSolution(int n);


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int n = 0;
    std::cin >> n;

    if (isOptimalLength(n)) {
        optimalSolution(n);
    }
    else {
        TableTop tableTop(n);

        tableTop.startBacktracking();

    }

    std::cout << std::endl;
    return 0;
}


bool isOptimalLength(int n) {
    if (n % 2 == 0 || n % 3 == 0 || n % 5 == 0) {
        return true;
    }
    return false;
}


void optimalSolution(int n) {
    if (n % 2 == 0) {
        std::cout << 4 << "\n";
        std::cout << 1 << " " << 1 << " " << n/2 << "\n";
        std::cout << 1 + n/2 << " " << 1 << " " << n/2 << "\n";
        std::cout << 1 << " " << 1 + n/2 << " " << n/2 << "\n";
        std::cout << 1 + n/2 << " " << 1 + n/2 << " " << n/2 << "\n";
    }
    else if (n % 3 == 0) {
        std::cout << 6 << "\n";
        std::cout << 1 << " " << 1 << " " << 2 * n / 3 << "\n";
        std::cout << 1 + 2 * n / 3 << " " << 1 << " " << n / 3 << "\n";
        std::cout << 1 << " " << 1 + 2 * n / 3 << " " << n / 3 << "\n";
        std::cout << 1 + 2 * n / 3 << " " << 1 + n / 3 << " " << n / 3 << "\n";
        std::cout << 1 + n / 3 << " " << 1 + 2 * n / 3 << " " << n / 3 << "\n";
        std::cout << 1 + 2 * n / 3 << " " << 1 + 2 * n / 3 << " " << n / 3 << "\n";

    }
    else if (n % 5 == 0) {
        std::cout << 8 << "\n";
        std::cout << 1 << " " << 1 << " " << 3 * n / 5 << "\n";
        std::cout << 1 + 3 * n / 5 << " " << 1 << " " << 2 * n / 5 << "\n";
        std::cout << 1 << " " << 1 + 3 * n / 5 << " " << 2 * n / 5 << "\n";
        std::cout << 1 + 3 * n / 5 << " " << 1 + 3 * n / 5 << " " << 2 * n / 5 << "\n";
        std::cout << 1 + 2 * n / 5 << " " << 1 + 3 * n / 5 << " " << n / 5 << "\n";
        std::cout << 1 + 2 * n / 5 << " " << 1 + 4 * n / 5 << " " << n / 5 << "\n";
        std::cout << 1 + 3 * n / 5 << " " << 1 + 2 * n / 5 << " " << n / 5 << "\n";
        std::cout << 1 + 4 * n / 5 << " " << 1 + 2 * n / 5 << " " << n / 5 << "\n";
    }
}
