#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <limits>
#include <algorithm>
#include <cmath>

using namespace std;

bool verboseMode = false;

using VertexName = char;
using EdgeLength = float;
using PathLength = EdgeLength;
using Path = string;
struct Vertex {
    VertexName name;
    map<Vertex *, EdgeLength> neighbours;
    bool isClosed = false;
    Vertex *previous = nullptr;
    PathLength pathLength = numeric_limits<PathLength>::infinity();
    PathLength heuristicValue = numeric_limits<PathLength>::infinity();

    Path getReconstructedPath() {
        Path path;
        for (Vertex *current = this; current != nullptr; current = current->previous)
            path += current->name;
        reverse(path.begin(), path.end());
        return path;
    };
};
struct VertexWithPriority {
    Vertex *vertex;
    PathLength priority = numeric_limits<PathLength>::signaling_NaN(); // pathLength + heuristicFunction
    bool operator<(const VertexWithPriority &other) const {
        return this->priority > other.priority;
    }
};

void printPriorityQueue(priority_queue<VertexWithPriority> queue) {
    while (!queue.empty()) {
        auto top = queue.top();
        cout << "    " << top.vertex->name
              << ": length = " << top.vertex->pathLength
              << ", path = " << top.vertex->getReconstructedPath()
              << ", priority = " << top.priority
              << ", is closed = " << (top.vertex->isClosed ? "true" : "false")
              << endl;
        queue.pop();
    }
}

struct NoPathExists : public runtime_error {
    NoPathExists() : runtime_error("No path exists.") {};
};
Path findPathUsingAStarAlgorithm(Vertex *startVertex, VertexName endName) {
    priority_queue<VertexWithPriority>  openVertices;
    startVertex->pathLength = 0;
    openVertices.push({startVertex, 0 + startVertex->heuristicValue});

    if (verboseMode) {
        cout << "  Current priority queue:" << endl;
        printPriorityQueue(openVertices);
    }

    while (!openVertices.empty()) {
        auto currentVertex = openVertices.top().vertex;
        openVertices.pop();
        if (currentVertex->isClosed)
            continue;
        if (verboseMode) cout << "Starting to work with min-priority non-closed vertex: " << currentVertex->name << endl;

        if (currentVertex->name == endName) {
            if (verboseMode) cout << "  End is reached." << endl;
            return currentVertex->getReconstructedPath();
        }

        if (verboseMode) cout << "  Updating neighbors:" << endl;
        bool someNeighborIsUpdated = false;
        for (auto &neighborEdge : currentVertex->neighbours) {
            Vertex *edgeEndVertex = neighborEdge.first;
            if (edgeEndVertex->isClosed)
                continue;
            EdgeLength edgeLength = neighborEdge.second;
            if (edgeEndVertex->pathLength > currentVertex->pathLength + edgeLength) {
                someNeighborIsUpdated = true;
                edgeEndVertex->pathLength = currentVertex->pathLength + edgeLength;
                edgeEndVertex->previous = currentVertex;
                PathLength priority = edgeEndVertex->pathLength + edgeEndVertex->heuristicValue;
                if (verboseMode) cout << "    " << edgeEndVertex->name
                    << ": length = " << edgeEndVertex->pathLength
                    << ", path = " << edgeEndVertex->getReconstructedPath()
                    << ", priority = " << priority
                    << endl;
                openVertices.push({edgeEndVertex, priority});
            }
        }
        if (verboseMode) {
            if (someNeighborIsUpdated) {
                cout << "  Current priority queue:" << endl;
                printPriorityQueue(openVertices);
            }
            else
                cout << "    Nothing to update." << endl;
        }

        currentVertex->isClosed = true;
        if (verboseMode) cout << "Vertex " << currentVertex->name << " is closed now." << endl;
    }
    throw NoPathExists();
}

int main(int argc , char *argv[] ) {
    if (argc > 1)
        verboseMode = true;

    cout << "Use the following syntax to set the heuristic value for some vertex: '@ a 5' - sets 5 for vertex 'a'." << endl;

    VertexName start, end;
    cin >> start >> end;

    map<VertexName, Vertex *> vertices;
    while (true) {
        VertexName edgeStart, edgeEnd;
        EdgeLength edgeLength;
        cin >> edgeStart;
        if (cin.eof()) {
            break;
        }
        cin >> edgeEnd >> edgeLength;
        if (cin.fail()) {
            cerr << "Incorrect input." << endl;
            return 1;
        }
        auto endVertexPtr = vertices[edgeEnd];
        if (endVertexPtr == nullptr)
            endVertexPtr = vertices[edgeEnd] = new Vertex({edgeEnd});
        if (edgeStart != '@') {
            auto startVertexPtr = vertices[edgeStart];
            if (startVertexPtr == nullptr)
                startVertexPtr = vertices[edgeStart] = new Vertex({edgeStart});
            startVertexPtr->neighbours[endVertexPtr] = edgeLength;
        }
        else {
            endVertexPtr->heuristicValue = edgeLength;
        }
    }

    for (auto vertex : vertices) {
        if (isinf(vertex.second->heuristicValue)) {
            cout << "You need enter a heuristic value for every vertex." << endl;
            return 1;
        }
    }

    auto path = findPathUsingAStarAlgorithm(vertices[start], end);

    if (verboseMode) cout << "Ultimate path: ";
    cout << path << endl;

    return 0;
}
