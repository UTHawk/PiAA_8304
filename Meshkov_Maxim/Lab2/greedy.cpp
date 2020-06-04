#include <iostream>
#include <map>
#include <set>
#include <limits>

using namespace std;

bool verboseMode = false;

using VertexName = char;
using EdgeLength = float;
struct EdgeEndWithLength;
using Path = string;
struct Vertex {
    VertexName name;
    set<EdgeEndWithLength> outgoingEdgesEndsWithLengths;
    bool isInPath = false;
};
struct EdgeEndWithLength {
    Vertex *end;
    EdgeLength length = numeric_limits<EdgeLength>::signaling_NaN();
    bool operator<(const EdgeEndWithLength &other) const {
        if (this->length != other.length)
            return this->length < other.length;
        return this->end < other.end;
    }
};


enum PathFindingResult {
    FOUND,
    FAIL,
};
PathFindingResult findPathGreedilyRecursively(Vertex *currentVertex, VertexName endName, Path &path) {
    path += currentVertex->name;
    currentVertex->isInPath = true;
    if (verboseMode) {
        cout << "Adding vertex " << currentVertex->name << " to path." << endl;
        cout << "Current path: " << path << endl;
    }

    if (currentVertex->name == endName) {
        if (verboseMode) cout << "Path is found." << endl;
        return FOUND;
    }

    for (auto &edgeEndWithLength : currentVertex->outgoingEdgesEndsWithLengths) {
        Vertex *edgeEndVertex = edgeEndWithLength.end;
        if (edgeEndVertex->isInPath) {
            continue;
        }
        if (verboseMode) cout << "Unused minimal edge is found." << endl;
        if (findPathGreedilyRecursively(edgeEndVertex, endName, path) == FOUND)
            return FOUND;
    }

    path.pop_back();
    currentVertex->isInPath = false;
    if (verboseMode) {
        cout << "Nowhere to go. Coming back and removing the last vertex from path." << endl;
        cout << "Current path: " << path << endl;
    }
    return FAIL;
}

struct NoPathExists : public runtime_error {
    NoPathExists() : runtime_error("No path exists.") {};
};
Path findPathGreedily(Vertex *startVertex, VertexName endName) {
    Path path;
    if (findPathGreedilyRecursively(startVertex, endName, path) == FAIL)
        throw NoPathExists();
    return path;
}

int main(int argc , char *argv[] ) {
    if (argc > 1)
        verboseMode = true;

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
        auto startVertexPtr = vertices[edgeStart];
        if (startVertexPtr == nullptr)
            startVertexPtr = vertices[edgeStart] = new Vertex({edgeStart});
        auto endVertexPtr = vertices[edgeEnd];
        if (endVertexPtr == nullptr)
            endVertexPtr = vertices[edgeEnd] = new Vertex({edgeEnd});
        startVertexPtr->outgoingEdgesEndsWithLengths.insert({endVertexPtr, edgeLength});
    }

    auto path = findPathGreedily(vertices[start], end);

    if (verboseMode) cout << "Ultimate path: ";
    cout << path << endl;

    return 0;
}
