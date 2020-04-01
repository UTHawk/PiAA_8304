#include <iostream>
#include <string>
#include <vector>

void prefixx(std::string& s, std::vector<size_t>& pi)
{
    pi[0] = 0;

    for (size_t i = 1; i < s.size(); ++i)
    {
        size_t j = pi[i - 1];
        while (j > 0 && s[i] != s[j])
            j = pi[j - 1];

        if (s[j] == s[i])
            ++j;
        pi[i] = j;
    }
}

int shift()
{
    std::string s;
    std::cin >> s;
    size_t start_size = s.size();

    std::vector<size_t> pi(s.size() * 2);
    prefixx(s, pi);

    char c;
    size_t j = 0;
    size_t text_ind = 0;
    bool is_same = true;

    while (std::cin >> c)
    {
        if (s[text_ind] != c)
            is_same = false;

        while (j > 0 && s[j] != c)
            j = pi[j - 1];

        if (s[j] == c)
            ++j;

        pi[start_size + j] = j;
        ++text_ind;
    }

    if (is_same)
        return 0;
    if (text_ind != start_size - 1)
        return -1;


    size_t ind = pi[pi.size() - 1];
    for (int i = ind; i < start_size; ++i)
    {
        if (s[i] != s[i + start_size - ind])
            return -1;
    }

    return start_size - ind;
}

int main()
{
    std::cout << shift();
    return 0;
}
