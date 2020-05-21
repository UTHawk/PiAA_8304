#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <ctime>

struct Elem{
	char prev;
	std::vector<std::pair<char, int>> paths;
	int lenght_to_start = std::numeric_limits<int>::max();
};

int heuristic(char q1, char first_end){
	return std::abs(q1 - first_end);
}

void write(char end, char start, std::map<char, Elem>& elem){
	for (auto& i : elem)
	{
		if (i.first == end && i.second.lenght_to_start == std::numeric_limits<int>::max())
		{
			std::cout << "no path\n";
			return;
		}
	}

	std::string answer(1, end);

	while (true)
	{
		if (answer.back() == start)
			break;
		answer += elem[answer.back()].prev;
	}

	std::reverse(answer.begin(), answer.end());
	std::cout << answer;
}

int main(){
	std::map<char, Elem> elem;
	char start = 0;
	char first_end = 0;
	char second_end = 0;

	std::cout << "Input start, first end, second end: ";
	std::cin >> start >> first_end >> second_end;

	char start_vertex = 0;
	char end_vertex = 0;
	float lenght = 0;
	while (std::cin >> start_vertex >> end_vertex >> lenght)
	{
		if (lenght == -1)
			break;

		elem[start_vertex].paths.push_back(std::make_pair(end_vertex, lenght));
		if (start_vertex == start)
			elem[start_vertex].lenght_to_start = 0;
	}

	std::vector<char> q;

	for (auto& i : elem)
		q.push_back(i.first);


	auto time_start = clock();
	while (!q.empty())
	{
		char current;
		size_t erase_index;
		int min_priority = -1;
		for (size_t i = 0; i < q.size(); ++i)
		{
			if (elem[q[i]].lenght_to_start == std::numeric_limits<int>::max())
				continue;

			size_t current_priority = elem[q[i]].lenght_to_start + heuristic(q[i], first_end);
			if (current_priority < min_priority || min_priority == -1)
			{
				min_priority = elem[q[i]].lenght_to_start + heuristic(q[i], first_end);
				erase_index = i;
				current = q[i];
			}
		}
		if (min_priority == -1)
			break;

		q.erase(q.begin() + erase_index);

		for (auto& next : elem[current].paths)
		{
			int old_value = elem[next.first].lenght_to_start;
			int new_value = elem[current].lenght_to_start + next.second;
			if (old_value > new_value)
			{
				elem[next.first].lenght_to_start = new_value;
				elem[next.first].prev = current;
			}
		}
	}
	auto time_end = clock();

	std::cout << "\nTime: ";
	std::cout << (double)(time_end - time_start) / CLOCKS_PER_SEC << "\n";
	std::cout << "\nFor first_end: ";
	write(first_end, start, elem);
	std::cout << "\nFor second_end: ";
	write(second_end, start, elem);
	std::cout << "\n\nСложность алгоритма: O(|V|*|V| + |E|) V - мн-во вершин, E - мн-во ребер\n";

	return 0;
}