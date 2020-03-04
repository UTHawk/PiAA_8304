#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

struct ElemInfo
{
    char prev;
    std::vector<std::pair<char, int>> ways;
    int lenToStart = std::numeric_limits<int>::max();
};

size_t h(char c1, char end)
{
    return std::abs(c1 - end);
}

int main()
{
    std::map<char, ElemInfo> d;

    char start = 0;
    char end = 0;
    std::cin >> start >> end;

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


    while (!q.empty())
    {
        char cur;
        size_t eraseInd;
        int min_priority = -1;
        for (size_t i = 0; i < q.size(); ++i)
        {
            if (d[q[i]].lenToStart == std::numeric_limits<int>::max())
                continue;

            size_t cur_priority = d[q[i]].lenToStart + h(q[i], end);
            if (cur_priority < min_priority || min_priority == -1)
            {
                min_priority = d[q[i]].lenToStart + h(q[i], end);
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

    std::string s(1, end);
    while (true)
    {
        if (s.back() == start)
            break;
        s += d[s.back()].prev;
    }

    std::reverse(s.begin(), s.end());
    std::cout << s;

    return 0;
}