#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <fstream>


struct elem {
	int capacity;
	int flow;
};

bool comp_min(std::pair<char, elem> i, std::pair<char, elem> j) {
	if (i.second.capacity == j.second.capacity)
		return i.first < j.first;//для сортировки по минимальной пропускной способности
	return i.second.capacity < j.second.capacity;
}

bool comp_lex(std::pair<char, elem> i, std::pair<char, elem> j) {
	return i.first < j.first;//компаратор для сортировки в лексикографическом порядке
}
void net(char start, char finish, std::map<char, char> prev, std::map<char, //обновление значений
	std::vector<std::pair<char, elem>>>& card, int min, std::ostream& output) {//элементов
	std::vector<char> result;
	char current = finish;
	result.push_back(current);
	while (current != start) {//записываем путь в вектор
		current = prev[current];
		result.push_back(current);
	}
	output << "Found way: ";
	for (unsigned long int i = 0; i < result.size(); ++i) {
		output << result[result.size() - i - 1];//выводим найденный путь
	}
	output << std::endl << std::endl;
	output << "Changes of edges:" << std::endl;//отладочный вывод
	for (unsigned long int i = 0; i < result.size() - 1; ++i) {//изменяем значения элементов
		for (auto& next : card[result[result.size() - i - 1]]) {//для обычных путей
			if (next.first == result[result.size() - i - 2]) {
				output << "Capacity " << result[result.size() - i - 1] << next.first << ": " << next.second.capacity;
				next.second.capacity -= min;//выводим изменения
				output << " -> " << next.second.capacity << std::endl;
				output << "Flow " << result[result.size() - i - 1] << next.first << ": " << next.second.flow;
				next.second.flow += min;//выводим изменения
				output << " -> " << next.second.flow << std::endl << std::endl;
				for (auto& j : card[result[result.size() - i - 2]]) {//для обратных путей
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
	std::sort(card[current].begin(), card[current].end(), comp_min);//сортируем по возрастанию пропускной способности
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
	int count;
	char start;
	char finish;
	std::map<char, std::vector<std::pair<char, elem>>> card;//хранение графа
	char first, second;
	int len;
			std::cin >> count;
			std::cin >> start >> finish;
			for (int i = 0; i < count; ++i) {
				std::cin >> first >> second >> len;
				card[first].push_back({ second, {len, 0} });
			}
		
	
	
	std::map<char, char> prev;//для сохранения пути
	prev[start] = start;
	std::map<char, bool> visited;//проверка на посещение
	int test = 0;
	int flow = 0;
	
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
				std::sort(k.second.begin(), k.second.end(), comp_lex);
				for (auto i : k.second)
					std::cout << k.first << " " << i.first << " " << std::max(0, i.second.flow) << std::endl;
			}
		
		

	return 0;
}