#include <iostream>
#include <string>
#include <vector>


void prefix_function (const std::string& s, std::vector<int>& pi) {
    pi[0] = 0;
    for (int i=1; i < s.length(); ++i) {
        int j = pi[i-1];
        while (j > 0 && s[i] != s[j])
            j = pi[j-1];
        if (s[i] == s[j])
            ++j;
        pi[i] = j;
    }
}


void kmp(std::string& P, std::string& T, int& answer){

    std::vector<int> pi(P.length());
    prefix_function(P, pi);

    for (int k = 0, i = 0; i < T.length(); ++i)
    {
        while ((k > 0) && (P[k] != T[i]))
            k = pi[k-1];

        if (P[k] == T[i])
            k++;

        if (k == P.length()) {
            answer = i - P.length() + 1;
            return;
        }
    }
    answer = -1;
}

int main() {

    int answer = -1;
    std::string text, substring;
    std::cin >> text >> substring;

    text += text;

    kmp(substring, text, answer);

    std::cout << answer;

    return 0;
}