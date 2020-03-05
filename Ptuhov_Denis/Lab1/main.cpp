#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <algorithm>

struct sq
{
    int x;
    int y;
    int len;
};

void get(std::vector<std::vector<int>>& data, std::vector<sq>& tmp_res, std::vector<sq>& min_res)
{
    sq point;
    bool f = false;
    for (int i = 0; i < data.size(); ++i)
    {
        for (int j = 0; j < data.size(); ++j)
            if (data[i][j] == 0)
            {
                point.x = i;
                point.y = j;
                f = true;
                break;
            }
        if (f)
            break;
    }
    

    if (f == false)
    {
        min_res = tmp_res;
        return;
    }

    for (int tmp_len = 1; tmp_len < data.size(); ++tmp_len)
    {
        if (point.x + tmp_len > data.size() || point.y + tmp_len > data.size())
            return;

        if (tmp_res.size() + 1 >= min_res.size())
            return;

        for (int i = point.x; i < point.x + tmp_len; ++i)
            for (int j = point.y; j < point.y + tmp_len; ++j)
                if (data[i][j] == 1)
                    return;

        for (int i = point.x; i < point.x + tmp_len; ++i)
            for (int j = point.y; j < point.y + tmp_len; ++j)
                data[i][j] = 1;

        point.len = tmp_len;
        tmp_res.push_back(point);

        get(data, tmp_res, min_res);

        for (int i = point.x; i < point.x + tmp_len; ++i)
            for (int j = point.y; j < point.y + tmp_len; ++j)
                data[i][j] = 0;

        tmp_res.pop_back();

    }
}


int main()
{
    int n = 0;
    std::cin >> n;

    std::vector<std::vector<int>> a(n);
    for (int i = 0; i < n; ++i)
    {
        std::vector<int> tmp(n);
        for (int j = 0; j < n; ++j)
            tmp[j] = 0;
        a[i] = tmp;
    }

    std::vector<sq> tmp_res;
    std::vector<sq> min_res(n * 2 + 1);

    auto s1 = clock();
    get(a, tmp_res, min_res);
    auto s2 = clock();

    std::cout << min_res.size() << "\n";
    for (auto i : min_res)
        std::cout << i.x << " " << i.y << " " << i.len << "\n";

    std::cout << (double)(s2 - s1) / CLOCKS_PER_SEC;
    return 0;
}
