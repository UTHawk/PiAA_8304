#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>

struct Node{
    std::vector<int> ways;
    bool isLeaf = false;
    int str_num = -1;
    int link = -1;
    int flink = -1;
    int from = -1;
    char how = 0;
    std::vector<int> go;
};

Node createNode(int from, char how){
    Node vert;
    vert.ways = { -1, -1, -1, -1, -1 };
    vert.go = { -1, -1, -1, -1, -1 };
    vert.from = from;
    vert.how = how;
    return vert;
}

void addString(std::string& str, std::vector<Node>& bor, std::map<char, int>& alphabet, int str_num){
    int borInd = 0;
    for (auto c : str){
        char cInd = alphabet[c];
        if (bor[borInd].ways[cInd] == -1){ bor.push_back(createNode(borInd, cInd)); bor[borInd].ways[cInd] = bor.size() - 1;}
        borInd = bor[borInd].ways[cInd];
    }
    bor[borInd].isLeaf = true;
    bor[borInd].str_num = str_num;
}

int go(int v, char c, std::vector<Node>& bor);

int getSuffLink(int v, std::vector<Node>& bor){
    if (bor[v].link == -1){
        if (v == 0 || bor[v].from == 0) bor[v].link = 0;
        else bor[v].link = go(getSuffLink(bor[v].from, bor), bor[v].how, bor);
    }return bor[v].link;
}

int go(int v, char c, std::vector<Node>& bor){
    if (bor[v].go[c] == -1){
        if (bor[v].ways[c] != -1) bor[v].go[c] = bor[v].ways[c];
        else{ if (v == 0) bor[v].go[c] = 0; else bor[v].go[c] = go(getSuffLink(v, bor), c, bor); }
    } return bor[v].go[c];
}

int main(){
    std::map<char, int> alphabet;
    alphabet['A'] = 0;
    alphabet['C'] = 1;
    alphabet['G'] = 2;
    alphabet['T'] = 3;
    alphabet['N'] = 4;
    std::string text;
    size_t n;
    char in, out;
    std::cout << "Для считывания/вывода через консоль введите - 'c', через файл - 'f' \n";
    std::cin >> in >> out;
    if((out != 'c' && out != 'f') || (in != 'c' && in != 'f')){ std::cout << "Неверный ввод\n"; return 1; }
    std::vector<Node> bor;
    bor.push_back(createNode(0, 0));
    std::ofstream fout("output.txt");
    if(in == 'c'){
        std::cin >> text >> n;
        std::vector<std::string> a(n);
        for (size_t i = 0; i < n; ++i) std::cin >> a[i];
        for (size_t i = 0; i < a.size(); ++i) addString(a[i], bor, alphabet, i);
        int u = 0;
        for (int i = 0; i < text.length(); ++i){
            if(out=='c') std::cout << "Элемент текста: " << text[i] << " Cостояние: " << u << std::endl;
            else fout << "Элемент текста: " << text[i] << " Cостояние: " << u << std::endl;
            u = go(u, alphabet[text[i]], bor);
            if(out=='c') std::cout << "Новое состояние: " << u << std::endl;
            else fout << "Новое состояние: " << u << std::endl;
            for (int v = u; v != 0; v = getSuffLink(v, bor)){
                if(out=='c') std::cout << "\tВозврат к корню. Текущее состояние: " << v << std::endl;
                else fout << "\tВозврат к корню. Текущее состояние: " << v << std::endl;
                if (bor[v].isLeaf){
                    if(out=='c') std::cout << "\t\tЛист!!! Позиция в тексте: " << i - a[bor[v].str_num].length() + 2 << " Номер образца: " << bor[v].str_num + 1 << std::endl;
                    else fout << "\t\tЛист!!! Позиция в тексте: " << i - a[bor[v].str_num].length() + 2 << " Номер образца: " << bor[v].str_num + 1 << std::endl;
                }
            }
        }
    }else if(in == 'f'){
        std::ifstream input("input.txt");
        input >> text >> n;
        std::vector<std::string> a(n);
        for (size_t i = 0; i < n; ++i) input >> a[i];
        for (size_t i = 0; i < a.size(); ++i) addString(a[i], bor, alphabet, i);
        int u = 0;
        for (int i = 0; i < text.length(); ++i){
            if(out=='c') std::cout << "Элемент текста: " << text[i] << " Cостояние: " << u << std::endl;
            else fout << "Элемент текста: " << text[i] << " Cостояние: " << u << std::endl;
            u = go(u, alphabet[text[i]], bor);
            if(out=='c') std::cout << "Новое состояние: " << u << std::endl;
            else fout << "Новое состояние: " << u << std::endl;
            for (int v = u; v != 0; v = getSuffLink(v, bor)){
                if(out=='c') std::cout << "\tВозврат к корню. Текущее состояние: " << v << std::endl;
                else fout << "\tВозврат к корню. Текущее состояние: " << v << std::endl;
                if (bor[v].isLeaf){
                    if(out=='c') std::cout << "\t\tЛист!!! Позиция в тексте: " << i - a[bor[v].str_num].length() + 2 << " Номер образца: " << bor[v].str_num + 1 << std::endl;
                    else fout << "\t\tЛист!!! Позиция в тексте: " << i - a[bor[v].str_num].length() + 2 << " Номер образца: " << bor[v].str_num + 1 << std::endl;
                }
            }
        }
    }


}
