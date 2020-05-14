#include <iostream>
#include <string>
#include <vector>
#include <map>


struct Vertex       //Структура вершины
{
    std::map<char, int> next;    // сыновья вершины в боре
    std::map<char, int> go;      // пути автомата
    int prev = 0;                // индекс родительской вершины
    char prevChar = 0;           // символ родительской вершины
    int suffix = -1;             // индекс вершины суффиксного перехода
    int number = 0;              // номер терминальной вершины
    int deep = 0;                // глубина вершины
    bool isLeaf = false;         // является ли вершина листом (терминалом)
};


class AhoCorasick                // класс реализующий алгоритм Ахо-Корасик
{
public:

    AhoCorasick(){
        Vertex root;              // создание корневой вершины
        root.prev = -1;
        vertexs.push_back(root);  // добавление вершины в вектор вершин
        countVertex = 0;
    }

    void addPattern(const std::string& str)  // добавление символов паттерна в бор
    {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Add symbols of new pattern in prefix tree" << std::endl;
        currentVertex = 0;
        for (char i : str) {
            std::cout << "\tReceived character " << i << std::endl;
            if (vertexs[currentVertex].next.find(i) == vertexs[currentVertex].next.end()) {   // если для текущей вершины нет перехода по символу
                Vertex vertex;                                                                // вершина создается и добавляется в бор
                vertex.prev = currentVertex;
                vertex.prevChar = i;
                vertexs.push_back(vertex);
                vertexs[currentVertex].next[i] = vertexs.size() - 1;
                std::cout << "\tAdd new vertex in prefix tree with number " << vertexs.size() - 1 << std::endl;
                std::cout << "\tSymbol path to the vertex " << i << std::endl;
            }
            currentVertex = vertexs[currentVertex].next[i];
            std::cout << "\tTransition in vertex by symbol " << i << std::endl << std::endl;
        }

        countVertex++;
        patternsArr.push_back(str);
        vertexs[currentVertex].number = countVertex;                            // номер терминальной вершины
        vertexs[currentVertex].isLeaf = true;                                   // вершина объявляется терминальной
        vertexs[currentVertex].deep = str.size();
        std::cout << "New terminal vertex is " << vertexs[currentVertex].prevChar << std::endl;
        std::cout << "Count of terminal vertex " << vertexs[currentVertex].number << std::endl;
        std::cout << "Deep of terminal vertex " << vertexs[currentVertex].deep << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
    }

    void search(const std::string& str)                                         // поиск паттернов в строке
    {
        std::cout << std::endl << "\nSearch for patterns in the line is started" << std::endl << std::endl << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        int curr = 0;
        bool terminalVertexFound;
        for (size_t i = 0; i < str.size(); i++) {
            std::cout << "Search for symbols " << str[i] << std::endl;
            curr = getGo(curr, str[i]);                                         // по каждому символу переходим в новую вершину бора
            std::cout << "\nStart check terminal vertex and check terminal vertex by suffix link" << std::endl;
            terminalVertexFound = false;
            for (int tmp = curr; tmp != 0; tmp = getSuffix(tmp)) {              // также осуществляем переходы по суффиксным ссылкам
                if (vertexs[tmp].isLeaf) {                                      // если символ терминальный, то добавляем индекс в массив результата
                    result.push_back(i - vertexs[tmp].deep + 2);
                    result.push_back(vertexs[tmp].number);
                    terminalVertexFound = true;
                    std::cout << "Terminal vertex found, found pattern with index " << i - vertexs[tmp].deep + 2 << " and number " << vertexs[tmp].number << std::endl;
                    break;
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
        for (size_t i = 0; i < result.size(); i += 2) {
            std::cout << result[i] << " " << result[i + 1] << std::endl;
            for (int j = 0; j < patternsArr[result[i+1] - 1].size(); j++)
                cutStr[result[i] - 1 + j] = true;
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

    int getSuffix(int index)                                        // получение вершины перехода по суффиксной ссылке
    {
        if (vertexs[index].suffix == -1) {                          // если суффиксная ссылка еще не определена
            if (index == 0 || vertexs[index].prev == 0) {
                vertexs[index].suffix = 0;                          // если корень или родитель корень - то суффиксная ссылка ведет в корень
                std::cout << "Suffix link follow to the root" << std::endl;
            }
            else {
                std::cout << "Start find path by suffix " << std::endl;
                vertexs[index].suffix = getGo(getSuffix(vertexs[index].prev), vertexs[index].prevChar); // иначе переходим ищем суффикс через суффикс родителя
            }
        }


        return vertexs[index].suffix;                           // возвращаем индекс суффиксной вершины в векторе вершин
    }

    int getGo(int index, char ch)                               // получить путь автомата из текущей вершины
    {
        if (vertexs[index].go.find(ch) == vertexs[index].go.end()) {                // если пути по символу из текущей вершины нет
            if (vertexs[index].next.find(ch) != vertexs[index].next.end()) {
                vertexs[index].go[ch] = vertexs[index].next[ch];                    // если из вершины есть дети, то путь прокладывается через них
                std::cout << "\nGo to the vertex " << vertexs[index].go[ch] << " by symbol " << ch << std::endl;
                std::cout <<"Add the transition to the paths of machine" << std::endl;
            }
            else {
                if (index == 0) {
                    vertexs[index].go[ch] = 0;
                    std::cout << "\nGo to the root " << std::endl;
                }
                else {
                    std::cout << "\nFollow the new suffix link and add the transition to the path of machine" << std::endl;
                    vertexs[index].go[ch] = getGo(getSuffix(index),ch);        // иначе путь прокладывается через суффиксную ссылку
                    std::cout << "\nReceived the path to the vertex " << vertexs[index].go[ch] << " through the suffix link" << std::endl;
                }
            }
        }
        else{
            std::cout << "Go to the vertex " << vertexs[index].go[ch] << " by path from vectors of paths" << std::endl;
        }

        return vertexs[index].go[ch];       // возвращаем индекс вершины пути в векторе вершин
    }

    void printMachine() const {                                             //функция печати автомата, полученного во время работы алгоритма
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
    std::vector<Vertex> vertexs;     // вектор вершин
    std::vector<int> result;         // вектор результата
    int countVertex;                 // количество терминальных вершин в боре
    int currentVertex;               // текущая вершина
    std::vector<std::string> patternsArr;
};


int main() {
    std::string str;
    int count = 0;
    std::cout << "Enter text string:" << std::endl;
    std::cin >> str;
    std::cout << "Enter count patterns:" << std::endl;
    std::cin >> count;

    std::string pattern;
    std::vector<std::string> patterns;
    std::cout << "Enter all patterns:" << std::endl;
    for (int i = 0; i < count; i++){
        std::cin >> pattern;
        patterns.push_back(pattern);
    }

    auto* ahoCorasick = new AhoCorasick();
    for (int i = 0; i < count; i++) {               // заполнение бора символами паттернов
        ahoCorasick->addPattern(patterns[i]);
    }

    ahoCorasick->search(str);                       // запуск поиска
    ahoCorasick->printResult(str);                        // печать результата

    return 0;
}
