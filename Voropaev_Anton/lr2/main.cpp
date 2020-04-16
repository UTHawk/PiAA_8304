#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <map>
#include <string>
#include <fstream>

struct node {
    inline bool operator<(const node& a) {
        if(this->length - this->vertex == a.length - a.vertex)
            return this->vertex < a.vertex;

        return this->length - this->vertex > a.length - a.vertex;
    }

    char vertex = 0;
    std::vector<std::pair<char, int>> destinations;
    double length = 0;
    std::string path;
};


void file_input(char& start, int& quantity, std::vector<char>& multiple_ends,  std::map<char, node>& dict) {

    std::ifstream input("C:/Users/Anton/CLionProjects/PAA_LAB2/input");
    input >> start;

    input >> quantity;
    for (int i = 0; i < quantity; ++i) {
        char tmp_end;
        input >> tmp_end;
        multiple_ends.push_back(tmp_end);
    }

    node tmp;
    char from, to;
    float len;
    std::pair<char, int> to_child;

    while (input >> from >> to >> len) {
        if (len == -1) {
            break;
        }

        if (dict.find(from) == dict.end()) {
            tmp.vertex = from;
            to_child = {to, len};
            tmp.destinations.push_back(to_child);
            dict[from] = tmp;
        } else {
            to_child = {to, len};
            dict[from].destinations.push_back(to_child);
        }
        tmp.destinations.clear();
    }
    for (auto i : multiple_ends) {
        dict[i] = {};
        dict[i].vertex = i;
    }
};


void console_input(char& start, int& quantity, std::vector<char>& multiple_ends,  std::map<char, node>& dict) {
    std::cin >> start;

    std::cout << "Set the number of ends\n";
    std::cin >> quantity;
    for (int i = 0; i < quantity; ++i) {
        char tmp_end;
        std::cin >> tmp_end;
        multiple_ends.push_back(tmp_end);
    }

    node tmp;
    char from, to;
    float len;
    std::pair<char, int> to_child;

    while (std::cin >> from >> to >> len) {
        if (len == -1) {
            break;
        }

        if (dict.find(from) == dict.end()) {
            tmp.vertex = from;
            to_child = {to, len};
            tmp.destinations.push_back(to_child);
            dict[from] = tmp;
        } else {
            to_child = {to, len};
            dict[from].destinations.push_back(to_child);
        }
        tmp.destinations.clear();
    }
    for (auto i : multiple_ends) {
        dict[i] = {};
        dict[i].vertex = i;
    }
}


void A_star_processing(std::vector<char>& multiple_ends,  std::map<char, node>& dict, char& start, int& mode) {

    std::ofstream output;

    if(mode == 1) {
        output.open("C:/Users/Anton/CLionProjects/PAA_LAB2/output");
    }

    node tmp;

    std::priority_queue<node, std::vector<node>, std::less<>>  priorityQueue;

    priorityQueue.push(dict[start]);
    while(!priorityQueue.empty()) {

        if(multiple_ends.empty())
            break;

        tmp = priorityQueue.top();
        priorityQueue.pop();

        if(mode == 1) {
            output << "Current vertex with the lowest priority : " << tmp.vertex <<
                   "\nPath to the vertex(length) : " << tmp.length <<
                   "\nPath to the vertex(path) : " << tmp.path << std::endl;
        }
        else {
            std::cout << "Current vertex with the lowest priority : " << tmp.vertex <<
                      "\nPath to the vertex(length) : " << tmp.length <<
                      "\nPath to the vertex(path) : " << tmp.path << std::endl;
        }

        for(int i = 0; i < multiple_ends.size(); ++i) {

            if (tmp.vertex == multiple_ends[i]) {
                if(mode == 1) {
                    output << "!!!END!!! Full path to the vertex " << tmp.vertex << " : "<< tmp.path + multiple_ends[i] + '\n';
                }
                else {
                    std::cout << "!!!END!!! Full path to the vertex " << tmp.vertex << " : "<< tmp.path + multiple_ends[i] + '\n';
                }
                multiple_ends.erase(multiple_ends.begin() + i);
                continue;
            }
        }
        if(mode == 2)
            std::cout << "______________________________________________________\n";
        else
            output << "______________________________________________________\n";

        node elem_for_push;
        for(auto i : tmp.destinations) {
            if(dict.find(i.first) == dict.end())
                continue;

            elem_for_push = dict[i.first];
            elem_for_push.length = i.second + tmp.length;
            elem_for_push.path =  tmp.path + tmp.vertex;
            priorityQueue.push(elem_for_push);
        }
    }
}


int main() {

    int mode = 0;
    std::cout << "Choose how you want to input the data:\n1 - From the file\n2 - From console\n";
    std::cin >> mode;
    while (mode < 1 || mode > 2) {
        std::cout << "You need to choose 1 or 2\n";
        std::cin >> mode;
    }

    char start;
    int quantity = 0;
    std::vector<char> multiple_ends;
    std::map<char, node> dict;
    node tmp;

    if (mode == 2) {
        console_input(start, quantity, multiple_ends, dict);
    }
    else {
        file_input(start, quantity, multiple_ends, dict);
    }

    A_star_processing(multiple_ends, dict, start, mode);

    return 0;
}

