#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stack>
#include <set>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <regex.h>
#include <memory>


struct Vertex;

struct Child{
    Child(Vertex* child, double cost) : child(child), cost(cost) { }
    Vertex* child;
    double cost;
};

struct Vertex{
    Vertex(char name) : name(name), from(nullptr) {}
    char name;
    std::vector <Child*> children;
    double g;
    double f;
    Vertex* from;
};

std::string printRes(Vertex* end)
{
    std::string res;
    Vertex* curr = end;
    std::stack<char> stack;

    while (curr) {
        stack.push(curr->name);
        curr = curr->from;
    }

    while (!stack.empty()) {
        res.push_back( stack.top());
        stack.pop();
    }
    return res;
}

std::string greedy(Vertex* start, Vertex* end){
    Vertex* curr = start;
    //std::set<Vertex*> open;
    std::set<Vertex*> closed;

    while (curr) {
        if (curr == end) { //нашли конец
            return printRes(curr);
        }

        closed.insert(curr);  //добавляем в просмотренные
        Vertex* minChild = nullptr;

        for (auto i : curr->children) { //ищем минимум
            i->child->g = i->cost;

            if (!minChild && closed.find(i->child) == closed.end()) {
                minChild = i->child;
            }
            else if (!minChild) {
                continue;
            }
            else if (i->child->g < minChild->g &&
                     closed.find(i->child) == closed.end()) {
                minChild = i->child;
            }
        }

        if (minChild) {
            minChild->from = curr;
            curr = minChild;
        }
        else {
            curr = curr->from;
        }

        if (!curr) {
            return "Ошибка ввода";

        }
    }
    return "Выход";
}

int main(int argc, char *argv[]){
    char startVertex;
    char endVertex;

    if(argv[1]!=NULL && (std::string(argv[1]) != "res.txt")){
        std::ifstream fin;
        fin.open(argv[1]);
        fin >> startVertex >> endVertex;

        Vertex* start = new Vertex(startVertex);
        Vertex* end = new Vertex(endVertex);

        std::map<int, Vertex*> connectionMap;
        connectionMap[start->name] = start;
        connectionMap[end->name] = end;

        char firstVertex;
        char secondVertex;
        double length = 0;

        while (fin >> firstVertex >> secondVertex >> length) {

            if (connectionMap.find(firstVertex) != connectionMap.end() &&
                            connectionMap.find(secondVertex) != connectionMap.end()) {
                        Child* child = new Child(connectionMap[secondVertex], length);
                        connectionMap[firstVertex]->children.push_back(child);
                    }
                    else if (connectionMap.find(firstVertex) != connectionMap.end()) {
                        Vertex* node = new Vertex(secondVertex);
                        connectionMap[secondVertex] = node;

                        Child* child = new Child(connectionMap[secondVertex], length);
                        connectionMap[firstVertex]->children.push_back(child);
                    }
                    else if (connectionMap.find(secondVertex) != connectionMap.end()) {
                        Vertex* node = new Vertex(firstVertex);
                        connectionMap[firstVertex] = node;

                        Child* child = new Child(connectionMap[secondVertex], length);
                        node->children.push_back(child);
                    }
                    else {
                        Vertex* node = new Vertex(firstVertex);
                        connectionMap[firstVertex] = node;

                        Vertex* node2 = new Vertex(secondVertex);
                        connectionMap[secondVertex] = node2;

                        Child* child = new Child(connectionMap[secondVertex], length);
                        connectionMap[firstVertex]->children.push_back(child);
                    }
        }
        std::string way = greedy(start,end);

        if(argv[2]!=NULL){
            std::ofstream fout;
            fout.open(argv[2]);
            fout << way;
        }
        else{
            std::cout << way;
        }
    }else if(argv[1]==NULL || (std::string(argv[1]) == "res.txt")){

        std::cin >> startVertex >> endVertex;

        Vertex* start = new Vertex(startVertex);
        Vertex* end = new Vertex(endVertex);

        std::map<int, Vertex*> connectionMap;
        connectionMap[start->name] = start;
        connectionMap[end->name] = end;

        char firstVertex;
        char secondVertex;
        double length = 0;

        while (std::cin >> firstVertex >> secondVertex >> length) {

            if (connectionMap.find(firstVertex) != connectionMap.end() &&
                            connectionMap.find(secondVertex) != connectionMap.end()) {
                        Child* child = new Child(connectionMap[secondVertex], length);
                        connectionMap[firstVertex]->children.push_back(child);
                    }
                    else if (connectionMap.find(firstVertex) != connectionMap.end()) {
                        Vertex* node = new Vertex(secondVertex);
                        connectionMap[secondVertex] = node;

                        Child* child = new Child(connectionMap[secondVertex], length);
                        connectionMap[firstVertex]->children.push_back(child);
                    }
                    else if (connectionMap.find(secondVertex) != connectionMap.end()) {
                        Vertex* node = new Vertex(firstVertex);
                        connectionMap[firstVertex] = node;

                        Child* child = new Child(connectionMap[secondVertex], length);
                        node->children.push_back(child);
                    }
                    else {
                        Vertex* node = new Vertex(firstVertex);
                        connectionMap[firstVertex] = node;

                        Vertex* node2 = new Vertex(secondVertex);
                        connectionMap[secondVertex] = node2;

                        Child* child = new Child(connectionMap[secondVertex], length);
                        connectionMap[firstVertex]->children.push_back(child);
                    }
        }
        std::string way = greedy(start,end);
        if(argv[1]==NULL){
            std::cout << way;
        }
        else if(argv[2]!=NULL){
            std::ofstream fout;
            fout.open(argv[2]);
            fout << way;
        }
        else if(std::string(argv[1]) == "res.txt"){
            std::ofstream fout;
            fout.open(argv[1]);
            fout << way;
        }

    }
}
