#include <limits.h>
#include <iostream>
#include <map>
#include <queue>

using namespace std;

using Graph = map<char, map<char, int>>;
bool breadthFirstSearch(Graph& graph, char start, char end, map<char, char>& path);
void FordFulkersonAlgorithm(Graph& graph, char start, char end);


void FordFulkersonAlgorithm(Graph& graph, char start, char end) {
	Graph flowGraph = graph;
	char u, v;

	map<char, char> path;
	int maxFlow = 0;

	while (breadthFirstSearch(flowGraph, start, end, path)) {
		int delta = INT_MAX;
		for (v = end; v != start; v = path[v]) {
			u = path[v];
			delta = min(delta, flowGraph[u][v]);
		}
		for (v = end; v != start; v = path[v]) {
			u = path[v];
			flowGraph[u][v] -= delta;
			flowGraph[v][u] += delta;
		}
		maxFlow += delta;
	}

	cout << "Network Max Flow Value: " << maxFlow << endl;
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
			cout << "Actual flow through "<< u << " - " << v << " :" << flow << endl;
		}
	}
}

bool breadthFirstSearch(Graph& graph, char start, char end, map<char, char>& path) {
	queue<char> queue;
	queue.push(start);

	map<char, bool> visited;
	visited[start] = true;

	while (!queue.empty()) {
		char vertex = queue.front();
		queue.pop();

		for (auto neighbor : graph[vertex]) {
			char v = neighbor.first;
			int throughput = neighbor.second;

			if (!(visited[v]) && throughput > 0) {
				queue.push(v);
				visited[v] = true;
				path[v] = vertex;
			}
		}
	}

	return visited[end];
}


int main() {
	Graph graph;
	char start, end, u, v;
	int throughput, vertexCount;

	cout << "Enter the number of edges" << endl;
	cin >> vertexCount;
	cout << "source" << endl;
	cin >> start;
	cout << "stock" << endl;
	cin >> end;

	cout << "Enter the edges of the graph" << endl;
	for (int i = 0; i < vertexCount; ++i) {
		cin >> u >> v >> throughput;
		graph[u][v] = throughput;
	}

	FordFulkersonAlgorithm(graph, start, end);

	return 0;
}

