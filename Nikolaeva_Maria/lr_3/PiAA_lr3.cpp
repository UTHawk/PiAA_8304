#include <limits.h>
#include <iostream>
#include <map>
#include <queue>
#include <fstream>


using Graph = std::map<char, std::map<char, int>>;
bool breadthFirstSearch(Graph& graph, char start, char end, std::map<char, char>& path, std::ofstream& oFile);
void FordFulkersonAlgorithm(Graph& graph, char start, char end, std::ofstream& oFile);


void FordFulkersonAlgorithm(Graph& graph, char start, char end, std::ofstream& oFile) {
	Graph flowGraph = graph;
	char u, v;
	std::string currentPath;

	std::map<char, char> path;
	int maxFlow = 0;

	while (breadthFirstSearch(flowGraph, start, end, path, oFile)) {
		int delta = INT_MAX;
		currentPath = end;

		for (v = end; v != start; v = path[v]) {
			u = path[v];
			delta = std::min(delta, flowGraph[u][v]);
			currentPath = std::string(1, u) + " -> " + currentPath;
		}
		for (v = end; v != start; v = path[v]) {
			u = path[v];
			flowGraph[u][v] -= delta;
			flowGraph[v][u] += delta;
		}
		maxFlow += delta;
		oFile << currentPath << "\n" << "flow " << maxFlow << "\n\n\n";
	}

	oFile << "Network Max Flow Value: " << maxFlow << std::endl;
	int flow;
	for (auto& vertex : graph) {
		char u = vertex.first;

		for (auto neighbor : graph[u]) {
			char v = neighbor.first;
			int throughput = neighbor.second;

			if (throughput - flowGraph[u][v] < 0) {
				flow = 0;
			}
			else {
				flow = throughput - flowGraph[u][v];
			}
			oFile << "Actual flow through " << u << " - " << v << " :" << flow << std::endl;
		}
	}
}

bool breadthFirstSearch(Graph& graph, char start, char end, std::map<char, char>& path, std::ofstream& oFile) {
	std::queue<char> queue;
	queue.push(start);

	std::map<char, bool> visited;
	visited[start] = true;

	oFile << "----start searching for a path----\n";

	while (!queue.empty()) {
		char vertex = queue.front();
		queue.pop();
		oFile << "Curent vertex " << vertex << " has connections with:\n";

			for (auto neighbor : graph[vertex]) {
				char v = neighbor.first;
				int throughput = neighbor.second;
				oFile << "\t" << v << "(throughput " << throughput << ")\n";

				if (!(visited[v]) && throughput > 0) {
					queue.push(v);
					visited[v] = true;
					path[v] = vertex;
				}
			}
			oFile << "\n";
	}

	return visited[end];
}


int main() {
	Graph graph;
	char start, end, u, v;
	int throughput, vertexCount;
	
	std::ifstream  inFile;
	inFile.open("D:/test.txt");

	std::ofstream oFile;
	oFile.open("D:/res.txt");

	oFile << "Enter the number of edges" << std::endl;
	inFile >> vertexCount;
	oFile << "source" << std::endl;
	inFile >> start;
	oFile << "stock" << std::endl;
	inFile >> end;

	oFile << "Enter the edges of the graph" << std::endl;
	for (int i = 0; i < vertexCount; ++i) {
		inFile >> u >> v >> throughput;
		graph[u][v] = throughput;
	}

	FordFulkersonAlgorithm(graph, start, end, oFile);

	return 0;
}

