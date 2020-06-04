#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>

constexpr bool f = true;
constexpr const char* PATH_IN = "D:/test.txt";
constexpr const char* PATH_OUT = "D:/result.txt";


struct elemInfo{
    int size;
    int flow;
};

struct elem {
    char prev;
    std::vector<std::pair<char, elemInfo>> ways;
    bool isUsed;
};

char select(std::vector<std::pair<char, elemInfo>> const& arr, std::map<char, elem> & dict){
    char needed = 0;
    for(auto i:arr){
        if(!dict[i.first].isUsed && i.second.size>0) {
            needed = i.first;
        }
    }
    return needed;
}

std::string find(std::map<char, elem> dict, char start, char end){
    char curr = start;
    std::string path = {start};

    while(curr != end && !(curr == start && select(dict[curr].ways, dict) == 0)) {
        dict[curr].isUsed = true;
        if(select(dict[curr].ways, dict) != 0) {
            char prev = curr;
            curr = select(dict[curr].ways, dict);
            dict[curr].prev = prev;
            path += curr;
        }
        else {
            path.erase(path.end()-1);
            curr = dict[curr].prev;
        }
    }
    return path;
}

int minSize(std::string a, std::map<char, elem> dict) {
    int min = 2147483647;
    while(a.size()>1){
        for(auto i:dict[a[0]].ways)
            if(i.first == a[1] && i.second.size < min) {
                min = i.second.size;
            }
        a.erase(a.begin());
    }
    return min;
}

void change(std::string a, std::map<char, elem>& dict){
    int min = minSize(a,dict);
    if(f) std::cout << "Max flow: " << min << std::endl;
    while(a.size()>1){
        for(auto& i:dict[a[0]].ways)
            if(i.first == a[1]) {
                i.second.flow += min;
                i.second.size -= i.second.flow;

                for(auto& j:dict[a[1]].ways) {
                    if(j.first == a[0]) {
                        j.second.flow -= min;
                        j.second.size -= j.second.flow;
                    }
                }
            }
        a.erase(a.begin());
    }

}

bool cmp(std::pair<char, elemInfo> const& a, std::pair<char, elemInfo> const& b) {
    return a.first < b.first;
}

void read(std::istream & in,char& start,char& end, std::map<char, elem>& my_map) {
    int N;
    in >> N >> start >> end;
    char a, b;
    int c = 0;
    for(int i = 0; i < N; ++i) {
        in >> a >> b >> c;
        my_map[a].ways.push_back({b,{c,0}});
    }
}

int main() {
    char start, end;
    std::map<char, elem> my_map;
    int choseIn, choseOut;
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
        read(std::cin, start, end, my_map);
    }
    else{
        std::ifstream file;
        file.open(PATH_IN);

        if (!file.is_open()) {
            std::cout << "Can't open file!\n";
            return 0;
        }
        else {
            read(file,start,end,my_map);
        }
    }

    std::string cur_way = find(my_map,start,end);
    while (cur_way != std::string(1,start)) {
        if(f) std::cout << std::endl << "Current way: " << cur_way << std::endl;
        change(cur_way, my_map);
        cur_way = find(my_map,start,end);

    }

    int sum = 0;
    for(auto i:my_map[start].ways)
        sum += i.second.flow;
    if(choseOut == 1) {
        std::cout << sum <<std::endl;
        for(auto k:my_map) {
            std::sort(k.second.ways.begin(),k.second.ways.end(),cmp);
            for(auto i:k.second.ways)
                std::cout << k.first << " " << i.first << " " << std::max(0,i.second.flow) << std::endl;
        }
    }
    else {
        std::ofstream file;
        file.open(PATH_OUT);

        if (!file.is_open()) {
            std::cout << "Can't open file!\n";
            return 0;
        }
        else {
            file << sum <<std::endl;
            for(auto k:my_map) {
                std::sort(k.second.ways.begin(),k.second.ways.end(),cmp);
                for(auto i:k.second.ways)
                    file << k.first << " " << i.first << " " << std::max(0,i.second.flow) << std::endl;
            }
        }
    }
    return 0;
}