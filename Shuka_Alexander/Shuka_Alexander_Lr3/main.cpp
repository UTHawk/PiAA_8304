#include <iostream>
#include <algorithm>
#include <map>
#include <deque>
#include <climits>
#include <queue>


using Graph = std::map<char, std::map<char, int>>;


bool BFS (Graph& graph, char start, char end, std::map<char, char>& path) {
    std::cout << "--------------------------------------------------------\n";
    std::cout << "Start wide search\n";

    std::map<char,bool> visited;
    std::queue<char> queueVertex;

    queueVertex.push(start);
    visited[start] = true;

    while (!queueVertex.empty()) {
        char vertex = queueVertex.front();
        queueVertex.pop();
        std::cout << "Current vertex " << vertex << " and his neighbor: ";
        bool hasNeighbor = false;

        for (auto const neighbor : graph[vertex]) {
            if (neighbor.second > 0 && !(visited[neighbor.first])) {
                queueVertex.push(neighbor.first);
                visited[neighbor.first] = true;
                path[neighbor.first] = vertex;

                std::cout << neighbor.first << " with possible flow =  " <<
                             neighbor.second << "\n";
                hasNeighbor = true;
            }
        }

        if (!hasNeighbor){
            std::cout << "hasn't not visited neighbor";
        }

        std::cout << "\n";
    }

    return visited[end];
}


void printCurrentFlows(Graph& flowGraph, int pathFlow, int maxCurrentFlow,
                       std::string& pathStr){  //Функция печати текущего состояния графа, найденного нового пути и найденного нового потока
    std::cout << "\nFind new path with flow = " << pathFlow <<
                 ": " + pathStr << "\n";
    std::cout << "Flow graph:\n";

    for (auto const& vertex: flowGraph) {
        for (auto const neighbor: flowGraph[vertex.first]) {
            std::cout << "\t" << vertex.first << " " << neighbor.first <<
                         " " << neighbor.second << "\n";
        }
    }

    std::cout << "Current flow of graph = " << maxCurrentFlow << "\n";
}


void printResult(Graph& graph, Graph& flowGraph, int maxFlow) {
    std::cout << "-----------------------------------------------------\n";
    std::cout << "Result of algorithm:\n";
    std::cout << "Flow of graph: " << maxFlow << "\n";

    int flow = 0;
    for (auto const& vertex: graph)
        for (auto const neighbor : graph[vertex.first]) {
            if (neighbor.second - flowGraph[vertex.first][neighbor.first] < 0) {
                flow = 0;
            }
            else {
                flow = neighbor.second - flowGraph[vertex.first][neighbor.first];
            }

            std::cout << vertex.first << " " << neighbor.first << " " << flow << "\n";
        }
}


void FFA(Graph& graph, char start, char finish) {
    Graph flowGraph = graph;
    char fromVertex = 0;
    char toVertex = 0;
    std::map<char,char> path;
    int maxFlow = 0;
    std::string pathStr;

    while (BFS(flowGraph, start, finish, path)) {
        int pathFlow = INT_MAX;
        pathStr = finish;

        for (toVertex = finish; toVertex != start; toVertex = path[toVertex]) {
            fromVertex = path[toVertex];
            pathFlow = std::min(pathFlow, flowGraph[fromVertex][toVertex]);
        }

        for (toVertex = finish; toVertex != start; toVertex = path[toVertex]) {
            fromVertex = path[toVertex];
            flowGraph[fromVertex][toVertex] -= pathFlow;
            flowGraph[toVertex][fromVertex] += pathFlow;
            pathStr = std::string(1, fromVertex) + " --> " + pathStr;
        }

        maxFlow += pathFlow;
        printCurrentFlows(flowGraph, pathFlow, maxFlow, pathStr);
    }

    printResult(graph, flowGraph, maxFlow);
}


int main() {
    Graph graph;
    char start = 0;
    char finish = 0;
    char vertexFrom = 0;
    char vertexTo = 0;
    int pathLength = 0;
    int countVertex = 0;

    std::cin >> countVertex >> start >> finish;

    for (auto i = 0; i < countVertex; ++i) {
        std::cin >> vertexFrom >> vertexTo >> pathLength;
        graph[vertexFrom][vertexTo] = pathLength;
    }

    FFA(graph, start, finish);

    return 0;
}
