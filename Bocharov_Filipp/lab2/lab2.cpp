#include <iostream>
#include <vector>
#include <map>

#include <queue>
#include <fstream>
#include <string> 


int heuristic(char& first, char& second) {
	return std::abs(first - second);
}

void print(char& start, char& finish, std::map<char, char>& prev) {
	std::vector<char> result;
	char current = finish;
	result.push_back(current);
	while (current != start) {
		current = prev[current];
		result.push_back(current);
	}
		std::cout << "Result: ";
		for (unsigned long int i = 0; i < result.size(); ++i) {
			std::cout << result[result.size() - i - 1];
		}
	
}

int main() {
	
	char start, finish;
	std::map<char, std::vector<std::pair<char, int>>> card;// граф
	char first, second;
	float len;
	
			std::cin >> start >> finish;
			while (std::cin >> first >> second >> len) {
				if (len == -1)
					break;
				card[first].push_back(std::make_pair(second, len));
			}
		
	
	std::priority_queue<std::pair<int, char>, std::vector<std::pair<int, char>>, std::greater<std::pair<int, char>>> priorities;
	priorities.push(std::make_pair(0, start));//объявление очереди через двоичную кучу, вносим начальную вершину
	std::map<char, char> prev;
	std::map<char, int> cost;
	prev[start] = start;
	cost[start] = 0;
	int s;
	while (!priorities.empty()) {//пока очередь не станет пустой
		char current = priorities.top().second;//берем вершину с наименьшим приоритетом
		priorities.pop();
		std::cout << "Visiting: " << current << std::endl;
		std::cout << "Current ";
		print(start, current, prev);
		std::cout << std::endl;
		if (current == finish)//если доходим до конца
			break;//то завершаем цикл
		for (auto& next : card[current]) {//для текущей вершины проверяем все возможные пути
			int new_cost = cost[current] + next.second;//считаем стоимость пути в каждую из новых вершин
			if (!cost.count(next.first) || new_cost < cost[next.first]) {//если впервые в этой вершине
				cost[next.first] = new_cost;//или новый возможный путь короче, то обновляем стоимость
				int priority = new_cost + heuristic(next.first, finish);//считаем приоритет
				priorities.push(std::make_pair(priority, next.first));//заносим в очередь
				prev[next.first] = current;//в следующую вершину пришли из текущей
				s = new_cost;
			}
		}
	}
	
	print(start, finish, prev);
	std::cout << std::endl << "shortest way = "<<s;
	return 0;
}