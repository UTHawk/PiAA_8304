#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stack>
#include <set>



struct Vertex;
struct Child;

bool aStar(Vertex* start, Vertex* end);
Vertex* vertexWithMinF(std::set<Vertex*> open);
int h(Vertex* vertex1, Vertex* vertex2);

struct Vertex
{
	Vertex(int name) : name(name) { }

	double g;
	double f;
	int name;
	std::vector <Child*> children;
};


struct Child
{
	Child(Vertex* vertex, double pathLen) : vertex(vertex), pathLen(pathLen) { }

	Vertex* vertex;
	double pathLen;
};


int h(Vertex* vertex1, Vertex* vertex2) {
	return abs(vertex1->name - vertex2->name);
}


Vertex* vertexWithMinF(std::set<Vertex*> open) {
	Vertex* vertexWithMinF = nullptr;

	for (auto i : open) {
		if (!vertexWithMinF) {
			vertexWithMinF = i;
		}
		else if (vertexWithMinF->f > i->f) {
			vertexWithMinF = i;
		}
		else if (vertexWithMinF->f == i->f) {
			if (vertexWithMinF->name < i->name) {
				vertexWithMinF = i;
			}
		}
	}

	return vertexWithMinF;
}



bool aStar(Vertex* start, Vertex* end) {
	std::set<Vertex*> open;
	std::set<Vertex*> closed;
	std::map<Vertex*, Vertex*> from;

	start->g = 0;
	start->f = start->g + h(start, end);
	open.insert(start);
	

	while (!open.empty()) {
		Vertex* current = vertexWithMinF(open);

		if (current == end) {
			std::cout << "Path between start vertex and end vertex found:\n";
			std::stack<int> vertexNames;

			while (from.find(current) != from.end()) {
				vertexNames.push(current->name);
				current = from[current];
			}
			vertexNames.push(start->name);

			std::cout << vertexNames.top();
			vertexNames.pop();
			while (!vertexNames.empty()) {
				std::cout << "->" << vertexNames.top();
				vertexNames.pop();
			}
			std::cout << "\n";
			return true;
		}

		closed.insert(current);
		open.erase(open.find(current));

		for (auto child : current->children) {
			if (closed.find(current) != closed.end()) {
				double tmpChildG = current->g + child->pathLen;

				if (child->vertex->g > tmpChildG || closed.find(child->vertex) == closed.end())  {
					from[child->vertex] = current;
					child->vertex->g = tmpChildG;
					child->vertex->f = child->vertex->g + h(child->vertex, end);

					if (open.find(child->vertex) == open.end()) {
						open.insert(child->vertex);
					}
				}
			}
		}
	}

	return false;
}


int main() 
{
	int startVertex = 0;
	int endVertex = 0;

	std::cin >> startVertex >> endVertex;
	Vertex* start = new Vertex(startVertex);
	Vertex* end = new Vertex(endVertex);

	std::map<int, Vertex*> connectionMap;
	connectionMap[start->name] = start;
	connectionMap[end->name] = end;

	int firstVertex = 0;
	int secondVertex = 0;
	double length = 0;

	while (!std::cin.eof()) {
		std::cin >> firstVertex >> secondVertex >> length;

		if (connectionMap.find(firstVertex) != connectionMap.end() &&
			connectionMap.find(secondVertex) != connectionMap.end()) {
			Child* child = new Child(connectionMap[secondVertex], length);
			connectionMap[firstVertex]->children.push_back(child);
		}
		else if (connectionMap.find(firstVertex) != connectionMap.end()) {
			Vertex* vertex = new Vertex(secondVertex);
			connectionMap[secondVertex] = vertex;

			Child* child = new Child(connectionMap[secondVertex], length);
			connectionMap[firstVertex]->children.push_back(child);
		}
		else if (connectionMap.find(secondVertex) != connectionMap.end()){
			Vertex* vertex = new Vertex(firstVertex);
			connectionMap[firstVertex] = vertex;

			Child* child = new Child(connectionMap[secondVertex], length);
			vertex->children.push_back(child);
		}
		else {
			Vertex* vertex = new Vertex(firstVertex);
			connectionMap[firstVertex] = vertex;

			Vertex* vertex2 = new Vertex(secondVertex);
			connectionMap[secondVertex] = vertex2;

			Child* child = new Child(connectionMap[secondVertex], length);
			connectionMap[firstVertex]->children.push_back(child);
		}
	}


	if (!aStar(start, end)) {
		std::cout << "No path found between start and end vertex\n";
	}

	return 0;
}