#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stack>
#include <set>


struct Node;


struct Child
{
    Child(Node* node, double pathLen) :
        node(node), pathLen(pathLen) { }

    Node* node;
    double pathLen;
};


struct Node
{
    Node(int name) : name(name) { }

    double g;
    double f;
    int name;
    std::vector <Child *> children;
};


int h(Node* node1, Node* node2) {
    return abs(node1->name - node2->name);
}


Node* minF(std::set<Node *> vec) {
    Node* min = nullptr;

    for (auto i : vec) {
        if (!min) {
            min = i;
        }
        else if (min->f > i->f) {
            min = i;
        }
        else if (min->f == i->f) {
            if (min->name < i->name) {
                min = i;
            }
        }
    }

    return min;
}


void printWay(std::map<Node*, Node*>& from,
              Node* start, Node* end) {
    std::stack<int> stack;
    Node* current = end;

    while (from.find(current) != from.end()) {
        stack.push(current->name);
        current = from[current];
    }
    stack.push(start->name);

    while (!stack.empty()) {
        std::cout << stack.top();
        stack.pop();
    }
    std::cout << "\n";
}


bool aWithStar(Node* start, Node* end) {
    std::set<Node*> closed;
    std::set<Node*> open;
    std::map<Node*, Node*> from;

    open.insert(start);

    start->g = 0;
    start->f = start->g + h(start, end);

    while (!open.empty()) {
        Node* curr = minF(open);

        if (curr == end) {
            printWay(from, start, end);
            return true;
        }

        open.erase(open.find(curr));
        closed.insert(curr);

        for (auto child : curr->children) {
            if (closed.find(curr) != closed.end()) {
                double tmpG = curr->g + child->pathLen;

                if (closed.find(child->node) == closed.end() ||
                        child->node->g > tmpG) {
                    child->node->g = tmpG;
                    from[child->node] = curr;
                    child->node->f = child->node->g + h(child->node, end);

                    if (open.find(child->node) == open.end()) {
                        open.insert(child->node);
                    }
                }
            }
        }
    }

    return false;
}


int main() {
    int startVertex = 0;
    int endVertex = 0;

    std::cin >> startVertex >> endVertex;
    Node* start = new Node(startVertex);
    Node* end = new Node(endVertex);

    std::map<int, Node*> map;
    map[start->name] = start;
    map[end->name] = end;

    int firstVertex = 0;
    int secondVertex = 0;
    double coast = 0;

    while (!std::cin.eof()) {
        std::cin >> firstVertex >> secondVertex >> coast;

        if (map.find(firstVertex) != map.end() &&
                map.find(secondVertex) != map.end()) {
            Child* child = new Child(map[secondVertex], coast);
            map[firstVertex]->children.push_back(child);
        }
        else if (map.find(firstVertex) != map.end()) {
            Node* node = new Node(secondVertex);
            map[secondVertex] = node;

            Child* child = new Child(map[secondVertex], coast);
            map[firstVertex]->children.push_back(child);
        }
        else if (map.find(secondVertex) != map.end()) {
            Node* node = new Node(firstVertex);
            map[firstVertex] = node;

            Child* child = new Child(map[secondVertex], coast);
            node->children.push_back(child);
        }
        else {
            Node* node = new Node(firstVertex);
            map[firstVertex] = node;

            Node* node2 = new Node(secondVertex);
            map[secondVertex] = node2;

            Child* child = new Child(map[secondVertex], coast);
            map[firstVertex]->children.push_back(child);
        }
    }


    if (!aWithStar(start, end)) {
        std::cout << "No way!!!\n";
    }

    return 0;
}
