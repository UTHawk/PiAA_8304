#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

char source, destination;

struct edge {
    char destination{};
    int max{};
    int current{};
    bool is_reverse = false;
};


struct node {
    std::vector<edge> destinations;
};

std::map<char, node> dict;
std::map<char, bool> visited;



void input() {
    int quantity;

    std::cout << "Enter edges quantity\n";
    std::cin >> quantity;

    std::cout << "Enter source vertex\n";
    std::cin >> source;

    std::cout << "Enter destination vertex\n";
    std::cin >> destination;


    node tmp = node();
    char from, to;
    int len;
    edge to_child{};

    std::cout << "Enter edges parameters " << quantity << " times\n";
    for(int i = 0; i < quantity; ++i){
        std::cin >> from >> to >> len;

        bool f = false;
        for(auto& k : dict[from].destinations) {
            if(k.destination == to) {
                k = {to, len, 0, false};
                f = true;
                break;
            }
        }
        if(f)
            continue;

        if(dict.find(from) == dict.end()) {
            to_child = {to, len, 0};
            tmp.destinations.push_back(to_child);
            dict[from] = tmp;
            visited[from] = false;
        }
        else {
            to_child = {to, len, 0};
            dict[from].destinations.push_back(to_child);
        }
        tmp.destinations.clear();

        if(from != source) {
            if(dict.find(to) == dict.end()) {
                to_child = {from, len, 0, true};
                tmp.destinations.push_back(to_child);
                dict[to] = tmp;
                visited[from] = false;
            }
            else{
                to_child = {from, len, 0, true};
                dict[from].destinations.push_back(to_child);
            }
            tmp.destinations.clear();

        }
    }

    for(auto& i : dict) {
        std::sort(i.second.destinations.begin(), i.second.destinations.end(), [](edge e1, edge e2){return e1.max < e2.max;});
    }
}



int ff_algorithm(char u, int c_min) {

    if(u == destination)
        return c_min;

    visited[u] = true;

    for(auto& v : dict[u].destinations) {
        if (v.destination != u) {
            if (!visited[v.destination] && (v.current < v.max && !v.is_reverse)) {

                int flow = ff_algorithm(v.destination, std::min(c_min, v.max - v.current));
                if (flow > 0) {
                    v.current += flow;
                    return flow;
                }
            } else if (!visited[v.destination] && (v.is_reverse && v.current > 0)) {
                int flow = ff_algorithm(v.destination, std::min(c_min, v.current));
                if (flow > 0) {
                    v.current -= flow;
                    return flow;
                }
            }
        }
    }
    return 0;
}


int main() {

    input();

    int maxFlow = 0;
    int iterationResult = 0;
    while (true) {
        iterationResult = ff_algorithm(source, 1000);
        if(iterationResult <= 0)
            break;
        for (auto& i : visited)
            i.second = false;
        maxFlow += iterationResult;

    }

    for(auto& i : dict) {
        std::sort(i.second.destinations.begin(), i.second.destinations.end(), [](edge e1, edge e2){return e1.destination < e2.destination;});
    }


    std::cout << "Miximum flow: " << maxFlow << "\nEdges:" << std::endl;
    for(auto& i : dict) {
        for(auto& j : i.second.destinations) {
            if(!j.is_reverse)
                std::cout << i.first << " " << j.destination << " " << j.current << std::endl;
        }
    }
    return 0;
}

/*
5
a
d
a c 3
a b 4
b d 2
b c 2
c d 5

7
a
f
a b 7
a c 6
b d 6
c f 9
d e 3
d f 4
e c 2
 */