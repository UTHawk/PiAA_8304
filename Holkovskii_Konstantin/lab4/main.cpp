#include <iostream>
#include <vector>
#include <fstream>

constexpr const char* PATH_IN = "D:/test.txt";
constexpr const char* PATH_OUT = "D:/result.txt";

void Prefix(std::string const& S, std::vector<int>& A){
    A[0]=0;
    for(int i = 1; i<S.size();++i) {
        int k = A[i - 1];
        while (k > 0 && S[i] != S[k])
            k = A[k - 1];
        if (S[k] == S[i - k])
            A[i] = k;
        if(S[i] == S[k])
            ++k;
        A[i]=k;
    }
}

int KMP(std::string & S, std::string & T) {
    int size = S.size();
    int pos = -1;
    int  k = 0;
    std::vector<int> P(size);
    Prefix(S,P);
    for(int i = 0; i < T.size();++i) {
        while(k>0 && T[i] != S[k])
            k = P[k-1];
        if(T[i] == S[k])
            ++k;
        if(k == size) {
            pos = i - size + 1;
            return pos;
        }
    }
    return pos;
}

int main() {
    std::string S, T;

    int choseIn, choseOut;
    std::cout << "Input: 1 - console, 0 - file" << std::endl;
    std::cin >> choseIn;
    if(choseIn!=0 && choseIn!=1) {
        std::cout << "Wrong chose Input";
        return 0;
    }
    std::cout << "Output: 1 - console, 0 - file" << std::endl;
    std::cin >> choseOut;
    if(choseOut!=0 && choseOut!=1) {
        std::cout << "Wrong chose Output";
        return 0;
    }
    if(choseIn == 1)
        std::cin >> T >> S;
    else{
        std::ifstream file;
        file.open(PATH_IN);

        if (!file.is_open()) {
            std::cout << "Can't open file!\n";
            return 0;
        }
        file >> T >> S;
    }

    T+=T;
    if(choseOut ==1)
        std::cout << KMP(S,T);
    else{
        std::ofstream file;
        file.open(PATH_OUT);

        if (!file.is_open()) {
            std::cout << "Can't open file!\n";
            return 0;
        }
        file << KMP(S,T);
    }
    return 0;
}