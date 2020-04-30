#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <stack>
#include <utility>
#include <queue>
#include <set>
#include <string>
#include <locale>
#include <fstream>


std::string operator*(std::string s, size_t count){
	std::string ret;
	for (size_t i = 0; i < count; ++i)
	{
		ret = ret + s;
	}
	return ret;
}


class Graph {

	int source;
	int drain;

	std::vector<std::vector<std::pair<int, int>>> matrix;
	//std::vector<std::vector<int>> lastDirection;
	std::map<int, std::vector<std::pair<int, int>>> children;
	std::vector<bool> visited;


public:
	Graph(int source, int drain) :source(source - 97), drain(drain - 97) {

		for (int i = 0; i < std::max(source - 97 + 1, drain - 97 + 1); i++) {

			visited.push_back(false);
			matrix.push_back(std::vector<std::pair<int, int>>());
			//lastDirection.push_back(std::vector <int>());
			for (int j = 0; j < std::max(source - 97 + 1, drain - 97 + 1); j++) {
				matrix[i].push_back({-1,0});
				//lastDirection[i].push_back(0);
			}
		}

	}

	void expandMatrix(int maxSize) {

		for (int i = 0; i < matrix.size(); i++) {
			for (int j = matrix.size(); j < maxSize; j++) {
				matrix[i].push_back({ -1, 0 });
				//lastDirection[i].push_back(0);
			}
		}

		for (int i = matrix.size(); i < maxSize; i++) {

			visited.push_back(false);
			matrix.push_back(std::vector<std::pair<int, int>>());
			for (int j = 0; j < maxSize; j++) {
				matrix[i].push_back({ -1, 0 });
			}
		}
	}

	void setNode(int from, int whereto, int weight) {
		if (std::max(from - 97, whereto - 97) >= matrix.size()) {
			expandMatrix(std::max(from - 97 + 1, whereto - 97 + 1));
		}
		matrix[from - 97][whereto - 97] = { weight,0 };// a = 97
		children[from - 97].push_back({ whereto - 97, from-97 });
	}

	void printMatrix() {

		for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < matrix[i].size(); j++) {
				std::cout << (char)(i + 97) << " " << (char)(j + 97) << " " << matrix[i][j].first << "\n"; // a = 97
			}
		}
	}
	void printRealFlows() {
		for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < matrix[i].size(); j++) {
				if (matrix[i][j].first > -1) {
					if (matrix[j][i].first > -1 && matrix[i][j].second > 0 && matrix[j][i].second > 0) {
						int min = std::min(matrix[i][j].first, matrix[j][i].first);
						matrix[i][j].second -= min;
						matrix[j][i].second -= min;
					}
					std::cout << (char)(i + 97) << " " << (char)(j + 97) << " " << matrix[i][j].second << "\n"; // a = 97
				}
					
			}
		}
	}
	bool isDoubleEdge(int v1, int v2) {//проверяет является ли ребро двойным, и идет ли сейчас проход назад
		if (matrix[v1][v2].first > -1 && matrix[v2][v1].first > -1) {
			if (v1 > v2) {//ребро обратное
				return true;
			}
			return false;
		}
		else return false;
	}
	int dfs(int vertex, int min, int depth) {         // min — пропускная способность в текущем подпотоке
		std::string space = " ";
		space = space*depth;
		//std::cout << space << "Текущая вершина: " << (char)(vertex+97) << "\n";
		if (vertex == drain)
			return min;

		visited[vertex] = true;
		for (int j = 0; j < children[vertex].size(); j++) {
			int i = children[vertex][j].first;
			//std::cout << space << "Ребенок: " << (char)(i + 97) << "\n";
			if ((matrix[vertex][i].first > -1) && !visited[i] && (matrix[vertex][i].second < matrix[vertex][i].first) /*&& !isDoubleEdge(vertex, i)*/) {
				//std::cout << space << "Поиск пути" << "\n";
				int delta = dfs(i, std::min(min, matrix[vertex][i].first - matrix[vertex][i].second), depth + 1);
				if (delta > 0) {
					//std::cout << space << "Найденный путь: "<< delta << "\n";
					matrix[vertex][i].second += delta;
					//if (matrix[i][vertex].first > -1 ) {
					//	matrix[i][vertex].second -= delta;
					//}
					return delta;

				}

			}
			//else if ((matrix[vertex][i].first > -1) && !visited[i] && isDoubleEdge(vertex, i) && matrix[i][vertex].second > 0) {
			//	int delta = dfs(i, std::min(min, matrix[i][vertex].second));
			//	if (delta > 0) {

			//		matrix[i][vertex].second -= delta;

			//		return delta;

			//	}
			//}
		}
		//for (int i = 0; i < matrix.size(); i++) {
			
		//}
		return 0;
	}

	bool comp(std::pair<int, int> v1, std::pair<int, int> v2) {
		return matrix[v1.second][v1.first] > matrix[v2.second][v2.first];
	}

	int maxFlow() {
		int flow = 0;
		int curFlow = 0;
		

	
		for (int i = 0; i < matrix.size(); i++) {
			std::sort(children[i].begin(), children[i].end(), [&, this](std::pair<int, int> v1, std::pair<int, int> v2) {return matrix[v1.second][v1.first] < matrix[v2.second][v2.first]; });
		}
		while ((curFlow = dfs(source, 999999, 0)) != 0) {
			flow += curFlow;
			curFlow = 0;
			for (int i = 0; i < matrix.size(); i++) {
				visited[i] = false;
			}
		}

		return flow;
	}
};

int main()
{
	setlocale(LC_ALL, "Russian");

	char start = '\0';
	char end = '\0';

	int edges = 0;
	std::cin >> edges;
	std::cin >> start >> end;

	char from = '\0';
	char whereto = '\0';
	int weight = 0;

	Graph graph(start, end);
	while (edges) {
		std::cin >> from >> whereto >> weight;
		graph.setNode(from, whereto, weight);
		edges--;
	}
	//graph.printMatrix();
	int flow = graph.maxFlow();
	std::cout << flow << "\n";
	graph.printRealFlows();
	return 0;
}

