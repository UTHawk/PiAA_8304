#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <fstream>

constexpr bool f = true;
constexpr const char* PATH_IN = "D:/test.txt";
constexpr const char* PATH_OUT = "D:/result.txt";

struct elem {
    std::vector<std::pair<char, int>> ways;
    int length;
};

bool cmp(const std::pair<char,int> &a, const std::pair<char,int> &b) {
    if(a.second == b.second) return a.first > b.first;
    return a.second < b.second;
}

int G(char a, std::map<char, elem> & my_map) {return my_map[a].length;}

int H(char a, char where) {return abs(a - where);}

int F(char a, char where, std::map<char, elem>& my_map){ return G(a, my_map) + H(a, where);}

char MIN_F(std::set<char> & open, char where, std::map<char, elem>& my_map) {
    int min = 2147483647;
    char curr = 0;
    for(char i: open)
        if(F(i, where, my_map) < min){
            min = F(i, where, my_map);
            curr = i;
        }
    return curr;
}

std::string RECONSTRUCT_PATH(std::map<char,char> & from,char start, char where) {
    std::string path = {where};
    char curr = where;
    while (curr != start) {
        curr = from[curr];
        path += curr;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

void findWay(char start, char end1, char end2, std::map<char, elem>& my_map, std::ostream & out) {
    char curr = start;
    std::set<char> closed;
    std::set<char> open = {start};
    std::map<char,char> from;
    while(!open.empty()) {
        curr = MIN_F(open,end1,my_map);

        if(f){
            std::cout << std::endl << "Current vertex: " << curr << std::endl;
            std::cout << "Open: ";
            for(auto i:open ) std::cout << i;
            std::cout << std::endl << "Closed: ";
            for(auto i:closed) std::cout << i;
            std::cout << std::endl;
        }

        if(curr == end1){
            out << RECONSTRUCT_PATH(from, start, end1) << std::endl;

            if(from.find(end2) != from.end())
                out << RECONSTRUCT_PATH(from, start, end2) << std::endl;
            return;
        }
        open.erase(curr);
        closed.insert(curr);

        for(auto neighbour : my_map[curr].ways) {
            bool tentative_is_better;
            if(closed.find(neighbour.first) != closed.end()) continue;
            int tentative_g_score = G(curr, my_map) + neighbour.second;
            if(open.find(neighbour.first) == open.end()){
                open.insert(neighbour.first);
                tentative_is_better = true;
            }
            else {
                tentative_is_better = tentative_g_score < G(neighbour.first, my_map);
            }

            if(tentative_is_better) {
                from[neighbour.first] = curr;
                my_map[neighbour.first].length = tentative_g_score;
            }
        }
    }
    std::cout << "No way" << std::endl;
}

void read(std::istream & in, char& start, char& end1, char& end2, std::map<char, elem>& my_map) {
    in >> start >> end1 >> end2;
    char a, b;
    float c = 0;
    while(in >> a >> b >> c) {
        if(c == -1) break;
        my_map[a].ways.push_back({b,c});
        std::sort(my_map[a].ways.begin(),my_map[a].ways.end(), cmp);
    }
}

int main() {
    char start, end1, end2;
    int choseIn, choseOut;
    std::map<char, elem> my_map;
    std::cout << "Input: 1 - console, 0 - file" << std::endl;
    std::cin >> choseIn;
    if(choseIn!=0 && choseIn!=1) {
        std::cout << "Wrong chose Input";
        return 0;
    }
    std::cout << "Output: 1 - console, 0 - file" << std::endl;
    std::cin >> choseOut;
    if(choseOut!=0 && choseOut!=1) {
        std::cout << "Wrong chose Output";
        return 0;
    }
    if(choseIn == 1) {
        read(std::cin, start, end1, end2, my_map);
    }
    else{
        std::ifstream file;
        file.open(PATH_IN);

        if (!file.is_open()) {
            std::cout << "Can't open file!\n";
            return 0;
        }
        else {
            read(file,start,end1,end2,my_map);
        }
    }
    if(choseOut == 1) {
        findWay(start, end1, end2, my_map, std::cout);
    }
    else {
        std::ofstream file;
        file.open(PATH_OUT);

        if (!file.is_open()) {
            std::cout << "Can't open file!\n";
            return 0;
        }
        else {
            findWay(start, end1, end2, my_map, file);
        }
    }
    return 0;
}