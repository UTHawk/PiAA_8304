#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <fstream>

using namespace std;


struct Rib//ребро графа
{
    char begin;//начальная вершина
    char end;//конечная вершина
    double weight;//вес ребра
};

struct Step//возможные пути
{
    string path;//путь
    double length;//длина пути
    char estuary;//конец пути
};

class A_star_graph {
private:
    vector<Rib> graph;//список смежности
    vector<Step> res;//преоброзовываемый (открытый) список путей
    vector<char> curr;//закрытый список вершин, содержит текущий путь
    char source;
    char estuary;

public:
    A_star_graph() {
    };

    void input_graph() {
        char tmp;
        Rib elem;
        cin >> source >> estuary;
        while (cin >> tmp) {
            elem.begin = tmp;
            cin >> elem.end;
            cin >> elem.weight;
            graph.push_back(elem);
        }
        string buf = "";
        buf += source;
        for (auto &i : graph) {
            if (i.begin == source) {
                buf += i.end;
                res.push_back({buf, i.weight});
                res.back().estuary = estuary;
                buf.resize(1);//запись всех ребер, которые исходят из начальной позиции
            }
        }
        curr.push_back(source);
    }

    void finput_graph(char *file) {
        ifstream input(file);
        char tmp;
        Rib elem;
        input >> source >> estuary;
        while (input >> tmp) {
            elem.begin = tmp;
            input >> elem.end;
            input >> elem.weight;
            graph.push_back(elem);
        }
        string buf = "";
        buf += source;
        for (auto &i : graph) {
            if (i.begin == source) {
                buf += i.end;
                res.push_back({buf, i.weight});
                res.back().estuary = estuary;
                buf.resize(1);//запись всех ребер, которые исходят из начальной позиции
            }
        }
        curr.push_back(source);
    }

    size_t min_elem() //возвращает индекс минимального элемента из непросмотренных
    {
        double min;
        min = DBL_MAX;
        size_t temp = -1;
        for (size_t i(0); i < res.size(); i++) {
            if (res.at(i).length + abs(estuary - res.at(i).path.back()) < min) {
                if (is_visible(res.at(i).path.back())) {
                    res.erase(res.begin() + i);
                } else {
                    min = res.at(i).length + abs(estuary - res.at(i).path.back());
                    temp = i;
                }
            }
        }
        return temp;
    }

    bool is_visible(char value)//проверка доступа к вершине
    {
        for (char i : curr) {
            if (i == value) {
                return true;
            }
        }
        return false;
    }

    void Search() {
        sort(res.begin(), res.end(), [](const Step &a, const Step &b) -> bool {
            return a.length + a.estuary - a.path.back() > b.length + b.estuary - b.path.back();
        });
        while (true) {
            size_t min = min_elem();
            if (min == -1) {
                cout << "Wrong graph";
                break;
            }
            if (res.at(min).path.back() == estuary) {
                cout << res.at(min).path;
                return;
            }
            for (auto &i : graph) {
                if (i.begin == res.at(min).path.back()) {
                    string buf = res.at(min).path;
                    buf += i.end;
                    cout << buf << endl;//промежуточный вывод
                    res.push_back({buf, i.weight + res.at(min).length});
                }
            }
            curr.push_back(res.at(min).path.back());
            res.erase(res.begin() + min);
        }
    }
};



int main(int argc, char* argv[])
{
    A_star_graph element;
    if (argc == 2)
    {
        element.finput_graph(argv[1]);
    }
    else{
        element.input_graph();
    }
    element.Search();
    return 0;
}