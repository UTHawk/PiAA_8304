#include <iostream>
#include <ctime>

#include "backtrack.hpp"


int main() {
    std::cout << "Enter square size between 2-40:  ";
    int n = 0;
    std::cin >> n;
    
    if (std::cin.bad()) {
        std::cout << "\nInvalid input\n";
        exit(1);
    }
    if (n < 2 || n > 50) {
        std::cout << "\nInvalid input\n";
        exit(1);
    }
    
    Track track(n);

    if (n % 2 == 0 || n % 3 == 0 || n % 5 == 0) {
        track.optimalSolution();
    }
    else {
         auto startTime = clock();
         track.startBacktracking();
         auto endTime = clock();
//        std::cout << endTime - startTime << std::endl;
         std::cout << "\nTime: " << (double)(endTime - startTime) / CLOCKS_PER_SEC;
    }
    std::cout << std::endl;
    return 0;
}

