#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <map>
#include <string>


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


int main() {

    char start;
    int quantity = 0;
    std::vector <char> multiple_ends;

    std::cin >> start;

    std::cout << "Set the number of ends\n";
    std::cin >> quantity;
    for(int i = 0; i < quantity; ++i) {
        char tmp_end;
        std::cin >> tmp_end;
        multiple_ends.push_back(tmp_end);
    }


    std::map<char, node> dict;

    node tmp;
    char from, to;
    float len;
    std::pair<char, int> to_child;

    while(std::cin >> from >> to >> len) {
        if(len == -1) {
            break;
        }

        if(dict.find(from) == dict.end()) {
            tmp.vertex = from;
            to_child = {to, len};
            tmp.destinations.push_back(to_child);
            dict[from] = tmp;
        }
        else {
            to_child = {to, len};
            dict[from].destinations.push_back(to_child);
        }
        tmp.destinations.clear();
    }
    for(auto i : multiple_ends) {
        dict[i] = {};
        dict[i].vertex = i;
    }

    std::priority_queue<node, std::vector<node>, std::less<>>  priorityQueue;

    priorityQueue.push(dict[start]);
    while(!priorityQueue.empty()) {

        if(multiple_ends.empty())
            break;

        tmp = priorityQueue.top();
        priorityQueue.pop();

        std::cout << "Current vertex with the lowest priority : " << tmp.vertex <<
        "\nPath to the vertex(length) : " << tmp.length <<
        "\nPath to the vertex(path) : " << tmp.path << std::endl;


        for(int i = 0; i < multiple_ends.size(); ++i) {

            if (tmp.vertex == multiple_ends[i]) {
                std::cout << "!!!END!!! Full path to the vertex " << tmp.vertex << " : " << tmp.path + multiple_ends[i] + '\n';
                multiple_ends.erase(multiple_ends.begin() + i);
                continue;
            }
        }

        std::cout << "______________________________________________________\n";

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
    return 0;
}
/*
a
3
e
f
h
a b 3.0
b c 1.0
c d 1.0
a d 5.0
d e 1.0
c f 2.0
b h 4.0
t t -1

a
2
l
h
a b 1.000000
a f 3.000000
b c 5.000000
b g 3.000000
f g 4.000000
c d 6.000000
d m 1.000000
g e 4.000000
e h 1.000000
e n 1.000000
n m 2.000000
g i 5.000000
i j 6.000000
i k 1.000000
j l 5.000000
m j 3.000000
t t -1


a
2
k
p
a b 1.000000
a f 3.000000
b c 5.000000
b g 3.000000
f g 4.000000
c d 6.000000
d m 1.000000
g e 4.000000
e h 1.000000
e n 1.000000
n m 2.000000
g i 5.000000
i j 6.000000
i k 1.000000
j l 5.000000
m j 3.000000
j p 4.000000
t t -1

a
2
f
i
a b 0
a c 0
b d 0
c e 0
c s 0
d f 0
d s 0
s i 0
e f 0
t t -1

 */

