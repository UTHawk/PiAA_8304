#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <fstream>

std::string input_way = "C:/Users/Danielka/source/repos/piaa_lr3/input";//пути до файлов
std::string output_way = "C:/Users/Danielka/source/repos/piaa_lr3/output";

struct elem {//структура для хранения
	int capacity;//пропускной способности
	int flow;//и потока элемента
};

bool comp(std::pair<char, elem> i, std::pair<char, elem> j) {//компаратор
	if (i.second.capacity == j.second.capacity)//для сортировки
		return i.first < j.first;//по минимальной пропускной способности
	return i.second.capacity < j.second.capacity;
}

bool cmp(std::pair<char, elem> i, std::pair<char, elem> j) {
	return i.first < j.first;//компаратор для сортировки 
}//в лексикографическом порядке

void net(char start, char finish, std::map<char, char> prev, std::map<char, //обновление значений
	std::vector<std::pair<char, elem>>>& card, int min, std::ostream& output) {//элементов
	std::vector<char> result;
	char current = finish;
	result.push_back(current);
	while (current != start) {//записываем путь в вектор
		current = prev[current];
		result.push_back(current);
	}
	output << "Found way: ";//отладочный вывод
	for (unsigned long int i = 0; i < result.size(); ++i) {
		output << result[result.size() - i - 1];//выводим найденный путь
	}
	output << std::endl << std::endl;
	output << "Changes of edges:" << std::endl;//отладочный вывод
	for (unsigned long int i = 0; i < result.size()-1; ++i) {//изменяем значения элементов
		for (auto& next : card[result[result.size()-i-1]]) {//для обычных путей
			if (next.first==result[result.size() - i-2]) {
				output << "Capacity " << result[result.size() - i - 1] << next.first << ": " << next.second.capacity;
				next.second.capacity -= min;//выводим изменения
				output << " -> " << next.second.capacity << std::endl;
				output << "Flow " << result[result.size() - i - 1] << next.first << ": " << next.second.flow;
				next.second.flow += min;//выводим изменения
				output << " -> " << next.second.flow << std::endl << std::endl;
				for (auto& j : card[result[result.size() - i-2]]) {//для обратных путей
					if (j.first == result[result.size() - i - 1]) {
						output << "Capacity " << result[result.size() - i - 2] << j.first << ": " << j.second.capacity;
						j.second.capacity += min;//выводим изменения
						output << " -> " << j.second.capacity << std::endl;
						output << "Flow " << result[result.size() - i - 2] << j.first << ": " << j.second.flow;
						j.second.flow -= min;//выводим изменения
						output << " -> " << j.second.flow << std::endl << std::endl;
					}
				}
			}
		}

	}
	output << std::endl;
}

int find(std::map<char, std::vector<std::pair<char, elem>>>& card, char current, char finish,//поиск пути
	std::map<char, char>& prev, std::map<char, bool> visited, int result, std::ostream& output) {//и минимального результата
	output << "Visiting: " << current << std::endl;//отладочный вывод
	if (current == finish)//если дошли до конца
		return result;//возвращаем результат
	std::sort(card[current].begin(), card[current].end(), comp);//сортируем по возрастанию пропускной способности
	visited[current] = true;//считаем, что посетили текущую вершину
	for (auto& next : card[current]) {
		if ((!visited[next.first]) && (next.second.capacity > 0)) {//если не посещена и пропускная способность>0
			result = next.second.capacity;//первое ребро - начальный результат
			prev[next.first] = current;//обновляем путь
			int minim = find(card, next.first, finish, prev, visited, result, output);//рекурсия
			if (minim > 0) {//если нашли путь
				if (minim < result)//если меньше
					result = minim;//то обновляем результат
				return result;
			}
		}
	}
	return 0;

}


int main() {
	std::cout << "How do you want to input?" << std::endl << std::endl
		<< "Press 1 to input from console." << std::endl//выбор как считать
		<< "Press 2 to input from file." << std::endl;
	int count = 0;
	char start = '0';
	char finish = '0';
	std::map<char, std::vector<std::pair<char, elem>>> card;//хранение графа
	char first, second;
	int len=0;
	int menu = 0;
	while (menu != 1 && menu != 2) {//пока не введется нужная цифра
		std::cin >> menu;
		if (menu == 1) {//считывание с консоли
			std::cin >> count;
			std::cin >> start >> finish;
			for (int i = 0; i < count; ++i) {
				std::cin >> first >> second >> len;
				card[first].push_back({ second, {len, 0} });
			}
		}
		else if (menu == 2) {//считывание из файла
			std::ifstream file;
			file.open(input_way);
			if (!file.is_open()) {
				std::cout << "Can't open file!" << std::endl;
				return 0;
			}
			file >> count;
			file >> start >> finish;
			for (int i = 0; i < count; ++i) {
				file >> first >> second >> len;
				card[first].push_back({ second, {len, 0} });
			}
		}
		else {//если неверно введена цифра, выводится сообщение
			std::cout << std::endl << "Wrong choice! Try again!" << std::endl;
		}//а цикл продолжается
	}
	std::map<char, char> prev;//для сохранения пути
	prev[start] = start;
	std::map<char, bool> visited;//проверка на посещение
	int test = 0;
	int flow = 0;
	menu = 0;
	std::cout << std::endl << "How do you want to output?" << std::endl << std::endl
		<< "Press 1 to output by console." << std::endl//выбор как вывести
		<< "Press 2 to output into file." << std::endl;
	while (menu != 1 && menu != 2) {//пока не введется нужная цифра
		std::cin >> menu;
		if (menu == 1) {//вывод на консоль
			std::cout << std::endl;
			while (test = find(card, start, finish, prev, visited, 0, std::cout)) {//пока есть путь
				std::cout << std::endl << "Minimal capacity: " << test << std::endl;//найденное минимальное ребро
				flow += test;//максимальное значение
				net(start, finish, prev, card, test, std::cout);//обновляем показатели
			}
			std::cout << "No more ways." << std::endl;
			std::cout << std::endl << "Result: " << std::endl;
			std::cout << flow << std::endl;
			for (auto k : card) {
				std::sort(k.second.begin(), k.second.end(), cmp);
				for (auto i : k.second)
					std::cout << k.first << " " << i.first << " " << std::max(0, i.second.flow) << std::endl;
			}
		}
		else if (menu == 2) {//вывод в файл
			std::ofstream file;
			file.open(output_way);
			if (!file.is_open()) {
				std::cout << "Can't open file!\n";
				return 0;
			}
			while (test = find(card, start, finish, prev, visited, 0, file)) {//пока есть путь
				file << std::endl << "Minimal capacity: " << test << std::endl;//найденное минимальное ребро
				flow += test;//максимальное значение
				net(start, finish, prev, card, test, file);//обновляем показатели
			}
			file << "No more ways." << std::endl;
			file << std::endl << "Result: " << std::endl;
			file << flow << std::endl;
			for (auto k : card) {
				std::sort(k.second.begin(), k.second.end(), cmp);
				for (auto i : k.second)
					file << k.first << " " << i.first << " " << std::max(0, i.second.flow) << std::endl;
			}
		}
		else {//если неверно введена цифра, выводится сообщение
			std::cout << std::endl << "Wrong choice! Try again!" << std::endl;
		}//а цикл продолжается
	}

	return 0;
}