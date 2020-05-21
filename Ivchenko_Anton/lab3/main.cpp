#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>

using namespace std;

struct  Edge {
	char end_v;
	int capacity;
	int flow;
};

void FordFulkersonAlgorithm(std::map<char, vector<Edge>>& graph, char start, char  end, std::ostream& out) {
	
	char i = start;
	int minElement = 0;
	std::vector<int> maxflowlist;
	std::vector<char> visited;
	std::map<char, char> parent;
	std::vector<int> flowlist;
	std::vector<char> path;
	visited.push_back(i);
	path.push_back(i);
	char next;

	while (i) {

		int empty = 1;
		for (int q = 0; q < graph[i].size(); q++) {
			if ((graph[i])[q].capacity > 0) { empty = 0; break; }
		}
		if (empty) {

			if (i == start) {
				int sum = 0;
				for (int q = 0; q < maxflowlist.size(); q++) {
					sum += maxflowlist[q];

				}out << sum << endl;
				for (char i = start; i < end; i++) {
					for (int j = 0; j < graph[i].size(); j++) {
						out << i << ' ' << graph[i][j].end_v << ' ' << graph[i][j].flow;
						out << endl;
					}
				}return;
			}
			else {
				char par = parent[i];
				for (int k = 0; k < graph[par].size(); k++) {
					if ((graph[par])[k].end_v == i)
						graph[par][k].capacity = 0;
				}i = par;
				path.pop_back();
				flowlist.pop_back();
				continue;
			}
		}
		int cap = 0;
		int mindiff = 1000;

		for (auto j : graph[i]) {

			int vis = 0;
			for (int k = 0; k < visited.size(); k++) {
				if (j.end_v == visited[k])
					vis = 1;
			}
			if (vis) continue;

			int diff = abs(j.end_v - i);
			if (diff < mindiff) {

				mindiff = diff;
				next = j.end_v;
				cap = j.capacity;
			}
			else if (diff == mindiff) {
				if (j.end_v < next) {
					next = j.end_v;
					cap = j.capacity;
				}
			}

		}out << next << ':' << cap << endl;
		flowlist.push_back(cap);
		parent[next] = i;
		path.push_back(next);
		visited.push_back(next);

		if (next == end) {
			int minElement = 1000;
			for (auto a : flowlist) {
				if (a < minElement) minElement = a;
			}
			for (int k = 0; k < path.size() - 1; k++) {
				for (int j = 0; j < graph[path[k]].size(); j++) {
					if (graph[path[k]][j].end_v == path[k + 1]) {
						graph[path[k]][j].capacity = graph[path[k]][j].capacity - minElement;
						graph[path[k]][j].flow = graph[path[k]][j].flow + minElement;
					}
				}
			}
			i = start;
			maxflowlist.push_back(minElement);
			flowlist.clear();
			path.clear();
			path.push_back(start);
			visited.clear();
			visited.push_back(start);
		}
		else { i = next; }
	}
}
int main() {

	int chose;
	std::cout << "console: 0, file :1" << std::endl;
	std::cin >> chose;

	char u, v, source, destination;
	int cap, n;
	std::map<char, vector<Edge>> graph;
	std::cin >> n;
	std::cin >> source;
	std::cin >> destination;

	for (int i = 0; i < n; i++) {
		cin >> u >> v >> cap;
		Edge edge{ v, cap, 0 };
		graph[u].push_back(edge);
	}

	if (chose == 0) {

		FordFulkersonAlgorithm(graph, source, destination, std::cout);
	}
	else {
		std::ofstream file;
		file.open("result.txt");

		if (!file.is_open()) {
			std::cout << "Incorrect!\n";
			return 0;
		}
		else {

			FordFulkersonAlgorithm(graph, source, destination, file);
		}
	}

	return 0;
}
