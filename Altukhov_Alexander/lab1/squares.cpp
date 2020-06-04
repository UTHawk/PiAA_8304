

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <locale>
#include <ctime>

struct Square {
	int x;
	int y;
	int w;
	bool mainSquare;
};

class Field {

	std::vector< std::vector<int> > arr;

	int width;
	int lastX;
	int lastY;
	int startX;
	int startY;
	int squaresAmount;
	int currentPiece;

	std::vector< Square > consequence;

	bool itsTimeToStop;

	unsigned long int operationsNum;

public:

	Field(int n) {

		width = n;
		arr.resize(n);

		for (int i = 0; i < n; i++) {
			arr[i].resize(n);
		}

		for (int i = 0; i < width; i++) {

			for (int j = 0; j < width; j++) {
				arr[i][j] = 0;
			}
		}

		lastX = 0;
		lastY = 0;
		squaresAmount = 0;
		currentPiece = 0;
		itsTimeToStop = false;

		operationsNum = 0;
	}

	void setStartPoint(int val) {
		startX = val;
		startY = val;
	}

	bool isEmpty() {
		return consequence.empty();
	}
	int getOperationsNum() {
		return operationsNum;
	}
	int getSquaresAmount() {
		return squaresAmount;
	}
	int getCurrentPiece() {
		return currentPiece;
	}
	void setLastX(int val) {
		lastX = val;
	}
	void setLastY(int val) {
		lastY = val;
	}
	void setCurrentPiece(int val) {
		currentPiece = val;
	}
	bool isItTimeToStop() {
		return itsTimeToStop;
	}

	bool fill(int minAmount) {
		
		for (int i = startY; i < width; i++) {

			for (int j = startX; j < width; j++) {
				if (!arr[i][j]) {
					lastY = i;
					lastX = j;
					if (squaresAmount >= minAmount) {
						return false;
					}

					while (!arr[i][j]) {
						operationsNum++;
						if (tryToFit()) {
							setSquare();
							currentPiece = width / 2;
							break;
						}
						else {
							currentPiece--;
						}
					}
				}
			}
		}
		return true;
	
	}

	bool tryToFit() {
	
		if (lastX + currentPiece > width) { return false; } 
		if (lastY + currentPiece > width) { return false; } 
		
		for (int i = lastY; i < lastY + currentPiece; i++) {
			for (int j = lastX; j < lastX + currentPiece; j++) {
				if (arr[i][j]){
					return false;
				}
			}
		}
		return true;
	
	}

	void setSquare(bool mainSquare = false) {

		squaresAmount++;
		for (int i = lastY; i < lastY + currentPiece; i++) {
			for (int j = lastX; j < lastX + currentPiece; j++) {
				arr[i][j] = squaresAmount;
			}
		}

		consequence.push_back({ lastX, lastY, currentPiece, mainSquare });

	}

	void print() {
		for (int i = 0; i < width; i++) {

			for (int j = 0; j < width; j++) {
				std::cout << " " << arr[i][j];
			}
			std::cout << "\n";
		}
	}

	void resetSquare(int x, int y, int w) {

		squaresAmount--;

		for (int i = y; i < y + w; i++) {
			for (int j = x; j < x + w; j++) {
				arr[i][j] = 0;
			}
		}

		consequence.pop_back();

	}

	void stepBack() {

		if (isEmpty()) { return; }
		Square lastSquare = consequence.back();
		if (lastSquare.mainSquare) {
			itsTimeToStop = true;
		}
		resetSquare(lastSquare.x, lastSquare.y, lastSquare.w);

		lastX = lastSquare.x;
		lastY = lastSquare.y;


		if (lastSquare.w == 1) {
			stepBack();
		}
		else{
			currentPiece = lastSquare.w - 1;
			setSquare();
		}
		
	}

	void printAnswer() {
		while (!(isEmpty())) {
			Square sq = consequence.back();
			consequence.pop_back();
			std::cout << sq.x + 1 << " " << sq.y + 1 << " " << sq.w;
			std::cout << "\n";
		}
	}
};

int main() {

	setlocale(LC_ALL, "Russian");

	int n;
	//std::cout << "Введите размер столешницы\n";
	std::cin >> n;

	if (n <= 1) {
		std::cout << "Некорректный размер квадрата\n";
		return 0;
	}

	int time = clock();

	Field field(n);

	Field minField(n);
	if (n % 2 == 0) {
		std::cout << 4 << "\n";
		std::cout << 1 << " " << 1 << " " << n / 2 << "\n";
		std::cout << 1 + n / 2 << " " << 1 << " " << n / 2 << "\n";
		std::cout << 1 << " " << 1 + n / 2 << " " << n / 2 << "\n";
		std::cout << 1 + n / 2 << " " << 1 + n / 2 << " " << n / 2 << "\n";
		return 0;
	}
	else if (n % 3 == 0) {
		std::cout << 6 << "\n";
		std::cout << 1 << " " << 1 << " " << 2 * n / 3 << "\n";
		std::cout << 1 + 2 * n / 3 << " " << 1 << " " << n / 3 << "\n";
		std::cout << 1 << " " << 1 + 2 * n / 3 << " " << n / 3 << "\n";
		std::cout << 1 + 2 * n / 3 << " " << 1 + n / 3 << " " << n / 3 << "\n";
		std::cout << 1 + n / 3 << " " << 1 + 2 * n / 3 << " " << n / 3 << "\n";
		std::cout << 1 + 2 * n / 3 << " " << 1 + 2 * n / 3 << " " << n / 3 << "\n";
		return 0;

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
		return 0;
	}


	field.setLastX(0);
	field.setLastY(0);
	field.setCurrentPiece(std::ceil(static_cast<double>(n) / 2));
	field.setSquare(true);

	field.setStartPoint(n / 2);

	field.setCurrentPiece((n / 2));
	field.setLastX(std::ceil(static_cast<double>(n) / 2));
	field.setSquare(true);

	field.setLastX(0);
	field.setLastY(std::ceil(static_cast<double>(n) / 2));
	field.setSquare(true);

	//field.print();
	//std::cout << "\n========================================\n";


	field.setLastX(0);
	field.setLastY(0);

	int minAmount = 2*n;

	field.setCurrentPiece(n/2);


	do{
		field.setCurrentPiece(n / 2);

		bool fillToFull = field.fill(minAmount);
		
		if (fillToFull) {
		
			if (field.getSquaresAmount() < minAmount) {
				minField = field;
				//std::cout << "\nЛУЧШЕЕ\n";
				//minField.print();
				//std::cout << "\n========================================\n";
				minAmount = field.getSquaresAmount();
			}
		}


		field.stepBack();

		//field.print();
		//std::cout << "\n========================================\n";


	} while ((!field.isItTimeToStop())  );


			

	//std::cout << "Минимальное количество квадратов: " << minAmount << "\n";
	//std::cout << "Расстановка: \n";
	//minField.print();
	
	std::cout << minAmount << "\n";
	minField.printAnswer();
	std::cout << std::endl << "runtime = " << (clock()-time) / 1000.0 << std::endl; // время работы программы
	std::cout << "operations: " << field.getOperationsNum() << "\n";
	return 0;
}
//(7;88)(11;1551)(13;4193)(17;31628)(23;435225)(29;3859404)