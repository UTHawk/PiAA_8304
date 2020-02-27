#include <iostream>
#include <Windows.h>

#include "TableTop.h"


bool isOptimalLength(int n);
void optimalSolution(int n);


int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

    std::cout << "Введите длину строны квадрата [2; 40]: ";
    int n = 0; //Длина стороны квадрата
	std::cin >> n;
	if (std::cin.bad()) {
		std::cout << "\nОшибка ввода";
        exit(1);
	}
	if (n < 2 || n > 40) {
		std::cout << "\nОшибка ввода";
        exit(1);
	}

    if (isOptimalLength(n)) { //Если можно дать ответ сразу - вызываем соответствующую функцию
		optimalSolution(n);
	}
	else { //Иначе - вызываем менее производительную функцию с бэктрекингом
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
        std::cout << "\nЧисло квадратов: 4\n"; //Получится 4 одинаковых квадрата
		std::cout << "1 1 " << n/2 << std::endl;
		std::cout << 1 + n/2 << " 1 " << n/2 << std::endl;
		std::cout << "1 " << 1 + n/2 << " " << n/2 << std::endl;
		std::cout << 1 + n/2 << " " << 1 + n/2 << " " << n/2 << std::endl;
	}
    else if (n % 3 == 0) {
        std::cout << "\nЧисло квадратов: 6\n" << std::endl; // Получится квадрат размером 2/3 от размера заданного и 5 квадратов размером 1/3
		std::cout << "1 1 " << 2 * n / 2 << std::endl;
		std::cout << 1 + 2 * n / 2 << " 1 " << n / 2 << std::endl;
		std::cout << "1 " << 1 + 2 * n / 2 << " " << n / 2 << std::endl;
		std::cout << 1 + 2 * n / 2 << " " << 1 + n / 2 << " " << n / 2 << std::endl;
		std::cout << 1 + n / 2 << " " << 1 + 2 * n / 2 << " " << n / 2 << std::endl;
		std::cout << 1 + 2 * n / 2 << " " << 1 + 2 * n / 2 << " " << n / 2 << std::endl;
	}
    else if (n % 5 == 0) {
        std::cout << "\nЧисло квадратов: 8\n" << std::endl; // Получится квадрат размером 3/5 от размера заданного, 3 квадрата 2/5 и 4 квадрата 1/5
		std::cout << "1 1 " << 3 * n / 5 << std::endl;
		std::cout << 1 + 3 * n / 5 << " 1 " << 2 * n / 5 << std::endl;
		std::cout << "1 " << 1 + 3 * n / 5 << " " << 2 * n / 5 << std::endl;
		std::cout << 1 + 3 * n / 5 << " " << 1 + 3 * n / 5 << " " << 2 * n / 5 << std::endl;
		std::cout << 1 + 2 * n / 5 << " " << 1 + 3 * n / 5 << " " << n / 5 << std::endl;
		std::cout << 1 + 2 * n / 5 << " " << 1 + 4 * n / 5 << " " << n / 5 << std::endl;
		std::cout << 1 + 3 * n / 5 << " " << 1 + 2 * n / 5 << " " << n / 5 << std::endl;
		std::cout << 1 + 4 * n / 5 << " " << 1 + 2 * n / 5 << " " << n / 5 << std::endl;
	}
}
