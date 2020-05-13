#include <iostream>
#include <vector>
#include <map>
#include <fstream>

/* Подсчитать количество вершин в автомате;
 * вывести список найденных образцов,
 * имеющих пересечения с  другими
 * найденными образцами в строке поиска.
*/

const int K = 5;
std::map<char, int> alphabet;

struct Node {
    int ways[K] = {-1, -1, -1, -1, -1};
    bool isLeaf = false;
    int numPrev = -1;
    char symbPrev = 0;
    int suffLink = -1;
    int go[K] = {-1, -1, -1, -1, -1};
    std::vector<int> leafPatternNumber;
};

void addString (const std::string& s, int num, std::vector<Node>& bor) {
    int v = 0;
    Node curr = Node();
    for (char i : s){ char c = alphabet[i];
        if (bor[v].ways[c] == -1) {
            curr.numPrev = v;
            curr.symbPrev = c;
            bor[v].ways[c] = bor.size();
            bor.push_back(curr);
        }v = bor[v].ways[c];}
    bor[v].isLeaf = true;
    bor[v].leafPatternNumber.push_back(num);
    std::cout<<"";
}

int go (int v, char c, std::vector<Node>& bor);

int getSuffLink (int v, std::vector<Node>& bor) {
    if (bor[v].suffLink == -1){
        if (v == 0 || bor[v].numPrev == 0) bor[v].suffLink = 0;
        else bor[v].suffLink = go(getSuffLink(bor[v].numPrev, bor), bor[v].symbPrev, bor);
    }return bor[v].suffLink;
}

int go (int v, char c, std::vector<Node>& bor) {
    if (bor[v].go[c] == -1){
        if (bor[v].ways[c] != -1) bor[v].go[c] = bor[v].ways[c];
        else{ if(v == 0) bor[v].go[c] = 0; else bor[v].go[c] = go(getSuffLink(v, bor), c, bor); }
    }return bor[v].go[c];
}


void multipleSearch(std::vector<std::pair<std::string, int>>& substrings, std::vector<Node>& bor, std::string& text, std::string& str, std::ostream& fout) {
    for(int i = 0; i < substrings.size(); ++i) addString(substrings[i].first, i + 1 , bor);
    std::vector<int> templates_count(text.size());
    templates_count.insert(templates_count.begin(), 0);
    int count = 0;
    Node current = bor[0];
    std::ofstream output;
    fout << "Количество вершин: " << bor.size() << std::endl;
    for(int i = 0; i<text.size(); ++i) {
        char c = alphabet[text[i]];
        count = go(count, c, bor);
        fout<<"Номер вершины в боре: "<<count<<"\n\tПроход по суффиксным ссылкам:\n";
        for (int v = count; v != 0; v = getSuffLink(v, bor)) {
            if(bor[v].isLeaf){
                for(int j = 0; j < bor[v].leafPatternNumber.size(); ++j) {
                    int pos = i - substrings[bor[v].leafPatternNumber[j] - 1].first.length() + 1;
                    fout<<"\tПозиция подстроки(A): "<< pos << "; ";
                    int t_index = pos - substrings[bor[v].leafPatternNumber[j] - 1].second + 1;
                    fout<<"Позиция в строке с маской(B): "<< substrings[bor[v].leafPatternNumber[j] - 1].second + 1<<"; A-B = "<<t_index<<std::endl;
                    if(t_index >= 0) {
                        templates_count[t_index] += 1;
                        if (templates_count[t_index] == substrings.size()) {
                            fout << "\tКоличество повторений в векторе шаблонов равно количеству подстрок без масок. Шаблон найден.\n";
                            fout << "\tИндекс вхождения: " << t_index + 1 << std::endl <<"Пересечения с другими найденными образцами:\n";
                            for(int k = t_index + 1; k < t_index + str.size() - 1; ++k) {
                                if(templates_count[k - str.size() + 1] == substrings.size()) {
                                        fout << "\t" << std::string(text.begin() + t_index,text.begin() + t_index + str.size() - 1)<< "->"<< std::string(text.begin() + k - str.size() + 1, text.begin() + k) << "\n";
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


int main() {
    alphabet['A'] = 0;
    alphabet['C'] = 1;
    alphabet['G'] = 2;
    alphabet['T'] = 3;
    alphabet['N'] = 4;
    char in, out;
    std::cout << "Для считывания/вывода через консоль введите - 'c', через файл - 'f' \n";
    std::cin >> in >> out;
    if((out != 'c' && out != 'f') || (in != 'c' && in != 'f')){ std::cout << "Неверный ввод\n"; return 1; }
    std::string text;
    std::string str;
    char joker = 0;
    if(in == 'c') std::cin >> text >> str >> joker;
    else if(in == 'f'){ std::ifstream input("input.txt"); input >> text >> str >> joker; }
    str += joker;
    std::vector<std::pair<std::string, int>> substrings;
    std::vector<Node> bor;
    Node root = Node();
    bor.push_back(root);
    std::pair<std::string,int> pairTmp;
    for(int i = 0; i < str.size(); i++){
        if(str[i] == joker){
            if(!pairTmp.first.empty()){
                pairTmp.second = i + 1 - pairTmp.first.length();
                substrings.push_back(pairTmp);
                pairTmp.first.clear();
            }continue;
        }pairTmp.first += str[i];}
    if(!pairTmp.first.empty()){ pairTmp.second = str.size() - pairTmp.first.length(); substrings.push_back(pairTmp); }
    if(out=='c') multipleSearch(substrings, bor, text, str, std::cout);
    else{ std::ofstream fout("output.txt"); multipleSearch(substrings, bor, text, str, fout); }
    return 0;
}
