#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>
#include <ctime>

struct ElemInfo
{
    char prev;
    std::vector<std::pair<char, int>> ways;
    int lenToStart = std::numeric_limits<int>::max();
};

size_t h(char c1, char end1)
{
    return std::abs(c1 - end1);
}

void write(char end, char start, std::map<char, ElemInfo>& d)
{
    for (auto& i : d)
    {
        if (i.first == end && i.second.lenToStart == std::numeric_limits<int>::max())
        {
            std::cout << "no way\n";
            return;
        }
    }

    std::string s(1, end);
    while (true)
    {
        if (s.back() == start)
            break;
        s += d[s.back()].prev;
    }

    std::reverse(s.begin(), s.end());
    std::cout << s;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    std::map<char, ElemInfo> d;

    char start = 0;
    char end1 = 0;
    char end2 = 0;

    std::cout << "start end1 end2\n";
    std::cin >> start >> end1 >> end2;

    char p1 = 0;
    char p2 = 0;
    float len = 0;
    while (std::cin >> p1 >> p2 >> len)
    {
        if (len == -1)
            break;

        d[p1].ways.push_back(std::make_pair(p2, len));
        if (p1 == start)
            d[p1].lenToStart = 0;
    }

    std::vector<char> q;

    for (auto& i : d)
        q.push_back(i.first);

    
    auto t1 = clock();
    while (!q.empty())
    {
        char cur;
        size_t eraseInd;
        int min_priority = -1;
        for (size_t i = 0; i < q.size(); ++i)
        {
            if (d[q[i]].lenToStart == std::numeric_limits<int>::max())
                continue;

            size_t cur_priority = d[q[i]].lenToStart + h(q[i], end1);
            if (cur_priority < min_priority || min_priority == -1)
            {
                min_priority = d[q[i]].lenToStart + h(q[i], end1);
                eraseInd = i;
                cur = q[i];
            }
        }
        if (min_priority == -1)
            break;

        q.erase(q.begin() + eraseInd);

        for (auto& next : d[cur].ways)
        {
            int old_value = d[next.first].lenToStart;
            int new_value = d[cur].lenToStart + next.second;
            if (old_value > new_value)
            {
                d[next.first].lenToStart = new_value;
                d[next.first].prev = cur;
            }
        }
    }
    auto t2 = clock();

    std::cout << "\nВремя работы: ";
    std::cout << (double)(t2 - t1) / CLOCKS_PER_SEC << "\n";
    std::cout << "\nДля end1: ";
    write(end1, start, d);
    std::cout << "\nДля end2: ";
    write(end2, start, d);
    std::cout << "\n\nСложность алгоритма: O(|V|*|V| + |E|) V - мн-во вершин, E - мн-во ребер\n";

    return 0;
}
