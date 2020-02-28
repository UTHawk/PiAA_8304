#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <algorithm>

struct sq
{
    size_t x;
    size_t y;
    size_t len;
};


void get(std::vector<int>& a, std::vector<sq>& tmp_res, std::vector<sq>& min_res, size_t n, size_t buf, long long& iter_cnt)
{
    sq elem;
    bool f = false;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            ++iter_cnt;
            if (a[i * n + j] == 0)
            {
                elem.x = i;
                elem.y = j;
                f = true;
                break;
            }
        }
        if (f)
            break;
    }


    if (f == false)
    {
        min_res = tmp_res;
        iter_cnt += tmp_res.size();
        return;
    }

    for (size_t tmp_len = buf; tmp_len < n; tmp_len += buf) {
        ++iter_cnt;
        if (elem.x + tmp_len > n || elem.y + tmp_len > n)
            return;

        if (tmp_res.size() + 1 >= min_res.size())
            return;


        for (int i = elem.x; i < elem.x + tmp_len; ++i)
            for (int j = elem.y; j < elem.y + tmp_len; ++j)
            {
                ++iter_cnt;
                if (a[i * n + j] == 1)
                    return;
            }

        for (int i = elem.x; i < elem.x + tmp_len; ++i)
            for (int j = elem.y; j < elem.y + tmp_len; ++j)
            {
                ++iter_cnt;
                a[i * n + j] = 1;
            }

        elem.len = tmp_len;
        tmp_res.push_back(elem);

        get(a, tmp_res, min_res, n, buf, iter_cnt);

        for (int i = elem.x; i < elem.x + tmp_len; ++i)
            for (int j = elem.y; j < elem.y + tmp_len; ++j)
            {
                ++iter_cnt;
                a[i * n + j] = 0;
            }

        tmp_res.pop_back();

    }
}


int check(int n)
{
    size_t ans = 1;
    for (int i = 2; i < n; ++i)
    {
        if (n % i == 0)
            ans = i;
    }

    return ans;
}

int main()
{
    size_t n = 0;
    std::cin >> n;

    int res = check(n);
    long long iter_cnt = 0;

    std::vector<sq> tmp_res;
    std::vector<sq> min_res(n * 2 + 1);
    std::vector<int> a(n*n, 0);

    if (res == 1 && n != 2) {

        for (size_t i = 0; i < n / 2; ++i)
            for (size_t j = 0; j < n / 2; ++j)
            {
                ++iter_cnt;
                a[i * n + j] = 1;
            }

        for (size_t i = n / 2; i < n; ++i)
            for (size_t j = 0; j < n / 2 + 1; j++)
            {
                ++iter_cnt;
                a[i * n + j] = 1;
            }

        for (size_t i = n / 2 + n % 2; i < n; ++i)
            for (size_t j = n / 2 + 1; j < n; ++j)
            {
                ++iter_cnt;
                a[i * n + j] = 1;
            }

        tmp_res.push_back({n / 2 + 1, n / 2 + 1, n / 2});
        tmp_res.push_back({0, 0, n / 2});
        tmp_res.push_back({n / 2, 0, n / 2 + 1});
    }


    auto time1 = clock();
    get(a, tmp_res, min_res, n, res, iter_cnt);
    auto time2 = clock();

    std::cout << min_res.size() << "\n";
    for (auto i : min_res)
        std::cout << i.x << " " << i.y << " " << i.len << "\n";

    std::cout << "Iter cnt: " << iter_cnt << "\n";
    std::cout << "Time: " << (double)(time2 - time1) / CLOCKS_PER_SEC;
    return 0;
}