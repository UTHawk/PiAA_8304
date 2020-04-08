#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

struct coord {//структура для хранения координат
	int x;
	int y;
	int len;
};

void solve(std::vector<std::vector<int>>& rectangle, std::vector<coord>& potential, std::vector<coord>& answer, int min, int x, int y, int& count, int paint){
	coord point;
	bool check = false;
	for (int i = 0; i < x; ++i){
		for (int j = 0; j < y; ++j)
			if (rectangle[i][j] == 0){//поиск первой свободной
				point.x = i;//клетки в матрице
				point.y = j;
				check = true;
				break;
			}
		if (check)
			break;
	}
	if (check == false){//если свободная клетка не найдена
		count += 1;
		answer = potential;//то случай запоминается как минимальный
		for (int i = 0; i < x; ++i) {
			std::cout << std::endl;
			for (int j = 0; j < y; ++j)
				std::cout << rectangle[i][j]<<" ";//и выводится частичное решение
		}
		std::cout << std::endl;
		return;
	}
	for (int size = 1; size < min; ++size){//проверка возможности вставки квадрата со стороной size
		if (point.x + size > x || point.y + size > y)
			return;//если выходит за границы
		if (potential.size() + 1 >= answer.size())
			return;//если количество разбиений превосходит уже имеющееся
		for (int i = point.x; i < point.x + size; ++i)
			for (int j = point.y; j < point.y + size; ++j)
				if (rectangle[i][j] != 0)//если пересекается
					return;//с уже закрашенными квадратами
		for (int i = point.x; i < point.x + size; ++i)
			for (int j = point.y; j < point.y + size; ++j)//если прошло все проверки
				rectangle[i][j] = paint;//то закрашиваем
		point.len = size;
		potential.push_back(point);
		solve(rectangle, potential, answer, min, x, y, count, paint+1);//продолжаем рекурсию
		for (int i = point.x; i < point.x + size; ++i)
			for (int j = point.y; j < point.y + size; ++j)
				rectangle[i][j] = 0;//возвращаем 0 для проверки других вариантов
		potential.pop_back();
	}
}

int main(){
	int x = 0;
	std::cout << "x = ";
	std::cin >> x;
	if (x < 2) {
		std::cout << "Wrong Input!" << std::endl;
		return 0;
	}
	std::cout << "\ny = ";// считывание сторон прямоугольника
	int y = 0;
	std::cin >> y;
	if (y < 2) {//длина стороны должна быть не менее 2
		std::cout << "Wrong Input!" << std::endl;
		return 0;
	}
	int min = 0;
	int max = 0;
	if (y > x) {
		min = x;
	}//поиск минимальной стороны
	else {
		min = y;
	}
	std::vector<std::vector<int>> rectangle(x);
	for (int i = 0; i < x; ++i){
		std::vector<int> side(y);
		for (int j = 0; j < y; ++j)//создание двумерного вектора
			side[j] = 0;
		rectangle[i] = side;
	}
	std::vector<coord> potential;//создание векторов для хранения
	std::vector<coord> answer(x * y + 1);//конечного ответа и текущего ответа
	int count = 0;
	clock_t time = clock();//считывание времени
	solve(rectangle, potential, answer, min, x, y, count, 1);//запуск рекурсии
	time = clock() - time;//подсчет сколько времени заняла рекурсия
	std::cout << "Time: ";
	std::cout << (double)(time) / CLOCKS_PER_SEC << std::endl;
	std::cout << "Count: " << count << std::endl;
	std::cout << "Pieces: " << answer.size() << std::endl;//вывод всех результатов
	for (size_t i = 0; i < answer.size(); ++i)
		std::cout << answer[i].x
		<< " " <<
		answer[i].y
		<< " " <<
		answer[i].len
		<< std::endl;
	return 0;
}