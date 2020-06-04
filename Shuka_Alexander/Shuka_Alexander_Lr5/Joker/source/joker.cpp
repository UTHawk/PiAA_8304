#include <iostream>
#include <string>
#include <vector>
#include <map>

struct Vertex                       //Структура вершины
{
    std::map<char, int> next;       //сыновья вершины в боре
    std::map<char, int> go;         // пути автомата
    std::vector<int> number;        // массив номеров паттернов
    int prev = 0;                   // индекс родительской вершины
    int deep = 0;                   // глубина вершины
    int suffix = -1;                // индекс вершины суффиксного перехода
    bool isLeaf = false;            // является ли вершина листом (терминалом)
    char prevChar = 0;              // символ родительской вершины
};


class AhoCorasick               // класс реализующий алгоритм Ахо-Корасик
{
public:
    explicit AhoCorasick(char joker): matchPatterns(1100000){
        Vertex root;                    // создание корневой вершины
        root.prev = -1;
        vertexs.push_back(root);        // добавление вершины в вектор вершин
        this->joker = joker;            // определяется джокер
        countTerminalVertex = 0;        // определяется число терминальных вершин
    }

    void readPattern(std::string& str){      //функция считывание паттерна
        std::cout << "--------------------------------------------" << std::endl;
        std::cout << "Read pattern processing" << std::endl;
        std:: cout << "Read pattern: " << str << std::endl;
        patternLen = str.size();
        split(str);                         //разбиение паттерна по джокерам

        std::cout << "Subpattern and his index in patterns: " << std::endl;
        for (int i = 0; i < patternArr.size(); i++){
            std::cout << patternArr[i] << " with index " << patternsLength[i] + 1 << std::endl;
        }
        std::cout  << "--------------------------------------------";
        std::cout << std::endl << std::endl <<"Add subpatterns in prefix tree " << std::endl << std::endl << std::endl;

        for (const auto& pattern : patternArr) {
            addPattern(pattern);                // добавление каждого подпатерна в бор
        }


    }

    void search(const std::string& str)                    // поиск паттернов в строке
    {
        std::cout << std::endl << "\nSearch for patterns in the line is started" << std::endl << std::endl << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        int curr = 0;
        bool terminalVertexFound;
        for (int i = 0; i < str.size(); i++) {
            std::cout << "Search for symbols " << str[i] << std::endl;
            curr = getGo(curr, str[i]);                 // по каждому символу переходим в новую вершину бора
            std::cout << "\nStart check terminal vertex and check terminal vertex by suffix link" << std::endl;
            terminalVertexFound = false;
            for (int tmp = curr; tmp != 0; tmp = getSuffix(tmp)) {     // также осуществляем переходы по суффиксным ссылкам
                if (vertexs[tmp].isLeaf) {
                    for (int j = 0; j < vertexs[tmp].number.size(); j++) {
                        if (i + 1 - patternsLength[vertexs[tmp].number[j] - 1] - vertexs[tmp].deep >= 0 &&
                            i + 1 - patternsLength[vertexs[tmp].number[j] - 1] - vertexs[tmp].deep <= str.size() - patternLen){   //если паттерн не выходит за границы (слева и справа)
                            matchPatterns[i + 1 - patternsLength[vertexs[tmp].number[j] - 1] - vertexs[tmp].deep]++;                // добавляем индекс совпадения в вектор совпадений подпаттернов
                            terminalVertexFound = true;
                            std::cout << "Terminal vertex found, found pattern  " << patternArr[vertexs[tmp].number[j] - 1]  << "  with index in text " << i - vertexs[tmp].deep + 2  << std::endl;
                            std::cout << "Count subpatterns in this index " << matchPatterns[i + 1 - patternsLength[vertexs[tmp].number[j] - 1] - vertexs[tmp].deep] << " of " << patternsLength.size() << std::endl;
                            break;
                        }
                    }
                }
            }
            if (!terminalVertexFound){
                std::cout << "Terminal vertex was not found, go to the next symbol" << std::endl;
            }
            std::cout << "-----------------------------------------" << std::endl;
        }

    }

    void printResult(const std::string& text) const{                      // функция печати результата
        printMachine();
        printMaxArcs();
        std::vector<bool> cutStr(text.size());                            // вектор попадания символов, вошедших в паттерн
        std::string textRest;                                             // входной текст без паттернов
        std::cout << "The result of algorithm work:" <<std::endl;
        for (int i = 0; i < matchPatterns.size(); i++) {
            if (matchPatterns[i] == patternsLength.size()) {              // если число вошедших подпаттернов в индексе совпадет с числом всех подпаттнов, то это индекс вхождения паттерна
                std::cout << i + 1 << "\n";
                for (int j = 0; j < patternLen; j++)
                    cutStr[i + j] = true;                                  // помечаем, что символ вошел в паттерн
            }
        }
        for (int i = 0; i < cutStr.size(); i++){
            if (!cutStr[i])
                textRest.push_back(text[i]);                                // заполняем строку символов, которые не вошли в паттерн
        }

        std::cout << std::endl << "String without found patterns:" << std::endl;
        std::cout << textRest << std::endl;
    }


private:

    void printMaxArcs() const{      //функция, вычисляющая максимальное количество дуг, исходящих из одной вершины в боре
        auto current = vertexs.begin();
        int maxArcs = 0;
        while (current != vertexs.end()){
            if (current->next.size() > maxArcs)
                maxArcs = current->next.size();
            current++;
        }
        std::cout << std::endl << "Max arcs of one vertex prefix tree: " << maxArcs << std::endl << std::endl;
    }

    void split(std::string str){        // функция разделения паттернов на подпаттерны
        std::string buf = "";
        for (int i=0; i<str.size(); i++){
            if (str[i] == joker){
                if (!buf.empty()) {
                    patternArr.push_back(buf);                      //заполняет массив подпаттернов
                    patternsLength.push_back(i - buf.size());       //и массив их вхождения в паттерне
                    buf = "";
                }
            }
            else {
                buf.push_back(str[i]);
                if (i == str.size() - 1){
                    patternArr.push_back(buf);
                    patternsLength.push_back(i - buf.size() + 1);
                }
            }
        }
    }

    void addPattern(const std::string& str)     // добавление символов паттерна в бор
    {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Add symbols of new pattern in prefix tree" << std::endl;
        int current = 0;
        for (char i : str) {
            std::cout << "\tReceived character " << i << std::endl;
            if (vertexs[current].next.find(i) == vertexs[current].next.end()) {  // если для текущей вершины нет перехода по символу
                Vertex ver;                                                           // вершина создается и добавляется в бор
                ver.suffix = -1;
                ver.prev = current;
                ver.prevChar = i;
                vertexs.push_back(ver);
                vertexs[current].next[i] = vertexs.size() - 1;
                std::cout << "\tAdd new vertex in prefix tree with number " << vertexs.size() - 1 << std::endl;
                std::cout << "\tSymbol path to the vertex " << i << std::endl;
            }
            current = vertexs[current].next[i];
            std::cout << "\tTransition in vertex by symbol " << i << std::endl << std::endl;
        }
        countTerminalVertex++;
        vertexs[current].number.push_back(countTerminalVertex);   //номера подпаттернов
        vertexs[current].isLeaf = true;                           // вершина объявляется терминальной
        vertexs[current].deep = str.size();
        std::cout << "New terminal vertex is " << vertexs[current].prevChar << std::endl;
        std::cout << "Deep of terminal vertex " << vertexs[current].deep << std::endl;
        std::cout << "-----------------------------------------" << std::endl;

    }

    int getSuffix(int index)                                    // получение вершины перехода по суффиксной ссылке
    {
        if (vertexs[index].suffix == -1) {                      // если суффиксная ссылка еще не определена
            if (index == 0 || vertexs[index].prev == 0) {
                vertexs[index].suffix = 0;                      // если корень или родитель корень - то суффиксная ссылка ведет в корень
                std::cout << "Suffix link follow to the root" << std::endl;
            }
            else {
                std::cout << "Start find path by suffix " << std::endl;
                vertexs[index].suffix = getGo(getSuffix(vertexs[index].prev), vertexs[index].prevChar);    // иначе переходим ищем суффикс через суффикс родителя
            }
        }
        return vertexs[index].suffix;                 // возвращаем индекс суффиксной вершины в векторе вершин
    }

    int getGo(int index, char ch)                     // получить путь автомата из текущей вершины
    {
        if (vertexs[index].go.find(ch) == vertexs[index].go.end()) {           // если пути по символу из текущей вершины нет
            if (vertexs[index].next.find(ch) != vertexs[index].next.end()) {
                vertexs[index].go[ch] = vertexs[index].next[ch];              // если из вершины есть дети, то путь прокладывается через них
                std::cout << "\nGo to the vertex " << vertexs[index].go[ch] << " by symbol " << ch << std::endl;
                std::cout <<"Add the transition to the paths of machine" << std::endl;
            }
            else {
               if (index == 0){
                   vertexs[index].go[ch] = 0;
                   std::cout << "\nGo to the root " << std::endl;
               }
               else{
                   std::cout << "\nFollow the new suffix link and add the transition to the path of machine" << std::endl;
                   vertexs[index].go[ch] = getGo(getSuffix(index), ch);    // иначе путь прокладывается через суффиксную ссылку
                   std::cout << "\nReceived the path to the vertex " << vertexs[index].go[ch] << " through the suffix link" << std::endl;
               }
            }
        }
        else{
            std::cout << "Go to the vertex " << vertexs[index].go[ch] << " by path from vectors of paths" << std::endl;
        }
        return vertexs[index].go[ch];      // возвращаем индекс вершины пути в векторе вершин
    }

    void printMachine() const {             //функция печати автомата, полученного во время работы алгоритма
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Machine built during the operation of the algorithm" << std::endl << std::endl;
        for (int i=0; i<vertexs.size(); i++){
            std::cout << "Vertex " << i << " with possible path: " << std::endl;
            auto cur = vertexs[i].go.begin();
            while (cur != vertexs[i].go.end()){
                std::cout << "\tVertex " << cur->second << " with path " << cur->first << std::endl;
                cur++;
            }
            std::cout << std::endl;
        }
        std::cout << "-----------------------------------------" << std::endl;
    }

private:
    std::vector<Vertex> vertexs;           // вектор вершин
    char joker;                            // символ джокера
    int countTerminalVertex;                // количество терминальных вершин
    std::vector<std::string> patternArr;      // вектор подпаттернов
    int patternLen{};                         // длина паттерна
    std::vector<int> matchPatterns;         // вектор попаданий подпаттернов
    std::vector<int> patternsLength;        // вектор длин подпаттернов
};



int main() {
    std::string str;
    std::string pattern;
    char joker;
    std::cout << "Enter text string:" << std::endl;
    std::cin >> str;
    std::cout << "Enter pattern:" << std::endl;
    std::cin >> pattern;
    std::cout << "Enter joker:" << std::endl;
    std::cin >> joker;

    auto* ahoCorasick = new AhoCorasick(joker);
    ahoCorasick->readPattern(pattern);           // обработка паттерна
    ahoCorasick->search(str);                   // запуск поиска
    ahoCorasick->printResult(str);                 // печать результата

    return 0;
}