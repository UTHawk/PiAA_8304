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

std::ostream* out;
std::istream* in;

std::string operator*(std::string s, size_t count){
	std::string ret;
	for (size_t i = 0; i < count; ++i)
	{
		ret = ret + s;
	}
	return ret;
}

bool comp(std::pair<int, int> v1, std::pair<int, int> v2) {

	if ((abs(v1.first - v1.second)) == (abs(v2.first - v2.second))) {
		return v1.first < v2.first;
	}
	return abs(v1.first - v1.second) < abs(v2.first - v2.second);
}

class Graph {

	int source;
	int drain;

	std::vector<std::vector<std::pair<int, int>>> matrix; //first - capacity, second - current flow

	std::map<int, std::vector<std::pair<int, int>>> children; // в паре first - вершина, second - родитель
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
						int min = std::min(matrix[i][j].second, matrix[j][i].second);
						matrix[i][j].second -= min;
						matrix[j][i].second -= min;
					}
					if (matrix[i][j].second < 0)
						matrix[i][j].second -= matrix[i][j].second;
					if (matrix[j][i].second < 0)
						matrix[j][i].second -= matrix[j][i].second;

					*out << (char)(i + 97) << " " << (char)(j + 97) << " " << matrix[i][j].second << "\n"; // a = 97
				}
					
			}
		}
	}



	int getDistance(int v1, int v2) {
		return abs(v2 - v1);
	}

	int search() {

		std::vector<int> vertexForSearch;
		std::map<int, std::vector<std::pair<int, int>>> localChildren; //для восстановления пути
		vertexForSearch.push_back(source);
		visited[source] = true;

		int curVertex = source;
		if (curVertex == drain) {
			return 0;
		}


		while (!vertexForSearch.empty()) {
			int minDistance = 9999;
			int ind1 = -1, ind2 = -1;

			std::vector<int> toDelete;

			for (int i = 0; i < vertexForSearch.size(); i++) { //поиск ребра соответствующего услови.
				curVertex = vertexForSearch[i];
				int timeToPop = true;
				for (auto& next : children[curVertex]) {
					if ((!visited[next.first]) && (matrix[curVertex][next.first].second < matrix[curVertex][next.first].first)) {

						timeToPop = false;

						int distance = getDistance(curVertex, next.first);
						if (distance < minDistance) {
							minDistance = distance;
							ind1 = curVertex;
							ind2 = next.first;
						}
						else if (distance == minDistance) {
							if (ind2 > next.first) {
								minDistance = distance;
								ind1 = curVertex;
								ind2 = next.first;
							}
						}
					}
				}

				if (timeToPop) {
					toDelete.push_back(i);
				}

			}

			std::sort(toDelete.begin(), toDelete.end()); //удаляем вершины из которых не осталось путей
			for (int i = 0; i < toDelete.size(); i++) {
				auto it = vertexForSearch.begin();
				std::advance(it, toDelete[i]);
				vertexForSearch.erase(it);
				for (int j = 0; j < toDelete.size(); j++) {
					toDelete[j]--;
				}
			}
			if (ind2 > -1) { //если нашли следующую вершину то добавляем ее

				*out << "Проход по ребру: " << (char)(ind1 + 97) << "->" <<(char)(ind2 + 97) << "\n";

				vertexForSearch.push_back(ind2);
				visited[ind2] = true;
				//localChildren[ind1].push_back({ ind2, ind1 });
				localChildren[ind2].push_back({ ind1, ind2 });
				curVertex = ind2;
			}

			if (curVertex == drain) { 
				break;
				//localChildren[drain].push_back({ ind1, ind2 });
			}
		}

		if (curVertex != drain) {
			*out << "Больше путей не найдено\n\n\n";
			return 0;
		}


		//восстановление пути и вычисление минимального потока
		std::stack<int> path;
		path.push(curVertex);
		int delta = 0;
		std::pair<int, int> curPair = localChildren[drain][0];
		curVertex = curPair.first;
		path.push(curVertex);
		delta = matrix[curVertex][drain].first - matrix[curVertex][drain].second;

		while (curVertex != source) {
			curPair = localChildren[curVertex][0];
			//curVertex = curPair.first;
			path.push(curPair.first);

			delta = std::min(delta, matrix[curPair.first][curVertex].first - matrix[curPair.first][curVertex].second);
			curVertex = curPair.first;
		}

		path.pop(); //исток
		int v1 = source;
		int v2 = -1;

		*out << "\n\nНайденный путь: " << (char)(source + 97);
		while (!path.empty()) {

			*out << " -> " << (char)(path.top() + 97);
			v2 = path.top();

			matrix[v1][v2].second += delta;
			if (matrix[v2][v1].first > -1) { //обратное ребро
				matrix[v2][v1].second -= delta;
			}
			path.pop();

			v1 = v2;
		}
		*out << "\nПоток: " << delta << "\n\n\n";

		

		return delta;


	}


	int maxFlow() {
		int flow = 0;
		int curFlow = 0;
	

		while((curFlow = search()) != 0){//пока есть путь
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

	int inputMode, outputMode;
	std::cout << "Ввод из... (0 - из консоли, 1 - из файла): ";
	std::cin >> inputMode;
	std::cout << "Вывод из... (0 - из консоли, 1 - из файла): ";
	std::cin >> outputMode;


	std::ifstream inFile("input.txt");
	std::ofstream outFile("output.txt");
	in = inputMode == 0 ? &std::cin : &inFile;
	out = outputMode == 0 ? &std::cout : &outFile;

	*in >> edges;
	*in >> start >> end;

	char from = '\0';
	char whereto = '\0';
	int weight = 0;

	Graph graph(start, end);
	while (edges) {
		*in >> from >> whereto >> weight;
		graph.setNode(from, whereto, weight);
		edges--;
	}
	//graph.printMatrix();
	int flow = graph.maxFlow();
	*out << flow << "\n";
	graph.printRealFlows();

	inFile.close();
	outFile.close();

	return 0;
}

