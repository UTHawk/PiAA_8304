#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


struct Rib
{
    char begin;
    char end;
    double weight;
};


class Greedy_graph
{
private:
    vector <Rib> graph;
    vector <char> res;
    vector <char> curr;
    char source;
    char dst;

public:
    Greedy_graph()
    {

    }
    void input_graph(){
        cin >> source >> dst;
        char temp;
        while(cin >> temp)
        {
            Rib element;
            element.begin = temp;
            if(!(cin >> element.end))
                break;
            if(!(cin >> element.weight))
                break;
            graph.push_back(element);
        }
        sort(graph.begin(), graph.end(), [](Rib first, Rib second)
        {
            return first.weight < second.weight;
        });
    }

    bool is_visible(char value)
    {
        for(char i : curr)
            if(i == value)
                return true;
        return false;
    }


    void to_search()
    {
        if(source != dst)
            Search(source);
    }

    bool Search(char value)
    {
        if(value == dst)
        {
            res.push_back(value);
            return true;
        }
        curr.push_back(value);
        for(auto & i : graph)
        {
            if(value == i.begin)
            {
                if(is_visible(i.end))
                    continue;
                res.push_back(i.begin);
                bool flag = Search(i.end);
                if(flag)
                    return true;
                res.pop_back();
            }
        }
        return false;
    }

    void Print()
    {
        for(char i : res)
            cout << i;
    }
};

int main()
{
    Greedy_graph element;
    element.input_graph();
    element.to_search();
    element.Print();
    return 0;
}