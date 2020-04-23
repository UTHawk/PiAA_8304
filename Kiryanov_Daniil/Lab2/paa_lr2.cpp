#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <fstream>
#include <string> 

std::string input_way = "C:/Users/Danielka/source/repos/paa_lr2_2/input";//пути до файлов
std::string output_way = "C:/Users/Danielka/source/repos/paa_lr2_2/output";

int heuristic(char& first, char& second){//эвристика
	return std::abs(first-second);
}

void print(char& start, char& finish, std::map<char, char>& prev, int menu) {//вывод результата
	std::vector<char> result;
	char current = finish;
	result.push_back(current);
	while (current != start) {//проходим от конца до начала
		current = prev[current];
		result.push_back(current);//записываем результат в вектор
	}
	if (menu == 1) {//вывод в консоль
		std::cout << "Result: ";
		for (unsigned long int i = 0; i < result.size(); ++i) {
			std::cout << result[result.size() - i - 1];
		}
	}
	else {//вывод в файл
		std::ofstream file;
		file.open(output_way);
		if (!file.is_open()) {
			std::cout << "Can't open file!\n";
		}
		file << "Result: ";
		for (unsigned long int i = 0; i < result.size(); ++i) {
			file << result[result.size() - i - 1];
		}

	}
}

int main() {
	std::cout << "How do you want to input?" << std::endl << std::endl
		<< "Press 1 to input from console." << std::endl//выбор как считать
		<< "Press 2 to input from file." << std::endl;
	char start, finish='0';
	std::map<char, std::vector<std::pair<char, int>>> card;//хранение графа
	char first, second;
	float len;
	int menu=0;
	while (menu != 1 && menu != 2) {//пока не введется нужная цифра
		std::cin >> menu;
		if (menu == 1) {//считывание с консоли
			std::cin >> start >> finish;
			while (std::cin >> first >> second >> len) {
				if (len == -1)
					break;
				card[first].push_back(std::make_pair(second, len));
			}
		}
		else if (menu == 2) {//считывание из файла
			std::ifstream file;
			file.open(input_way);
			if (!file.is_open()) {
				std::cout << "Can't open file!" << std::endl;
				return 0;
			}
			file >> start >> finish;
			while (file >> first >> second >> len) {
				card[first].push_back(std::make_pair(second, len));
			}
		}
		else {//если неверно введена цифра, выводится сообщение
			std::cout << std::endl << "Wrong choice! Try again!" << std::endl;
		}//а цикл продолжается
	}
	std::priority_queue<std::pair<int, char>, std::vector<std::pair<int, char>>, std::greater<std::pair<int, char>>> priorities;
	priorities.push(std::make_pair(0, start));//объявление очереди через двоичную кучу, вносим начальную вершину
	std::map<char, char> prev;//откуда пришли в вершину
	std::map<char, int> cost;//стоимость всего пути до вершины
	prev[start] = start;
	cost[start] = 0;
	while (!priorities.empty()) {//пока очередь не станет пустой
		char current = priorities.top().second;//берем вершину с наименьшим приоритетом
		priorities.pop();
		std::cout << "Visiting: " << current << std::endl;
		std::cout << "Current ";
		print(start, current, prev, 1);
		std::cout << std::endl;
		if (current == finish)//если доходим до конца
			break;//то завершаем цикл
		for (auto& next : card[current]) {//для текущей вершины прогоняем все возможные пути
			int new_cost = cost[current] + next.second;//считаем стоимость пути в каждую из новых вершин
			if (!cost.count(next.first) || new_cost < cost[next.first]) {//если впервые в этой вершине
				cost[next.first] = new_cost;//или новый возможный путь короче, то обновляем стоимость
				int priority = new_cost + heuristic(next.first, finish);//считаем приоритет
				priorities.push(std::make_pair(priority, next.first));//заносим в очередь
				prev[next.first] = current;//в следующую вершину пришли из текущей
			}
		}
	}
	menu = 0;
	std::cout << std::endl << "How do you want to output?" << std::endl << std::endl
		<< "Press 1 to output by console." << std::endl//выбор как вывести
		<< "Press 2 to output into file." << std::endl;
	while (menu != 1 && menu != 2) {//пока не введется нужная цифра
		std::cin >> menu;
		if (menu != 1 && menu != 2)//если неверно, то выводится сообщение и цикл продолжается
			std::cout << std::endl << "Wrong choice! Try again!" << std::endl;
	}
	print(start, finish, prev, menu);//функция вывода результата
	return 0;
}
