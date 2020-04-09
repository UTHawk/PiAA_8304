#include <iostream>
#include <string>
#include <vector>
#include <fstream>


void prefix_function (const std::string& s, std::vector<int>& pi) { // Префикс-функция
    pi[0] = 0;
    for (int i=1; i < s.length(); ++i) { //Цикл по длине строки
        int j = pi[i-1];  //j - текущее значение длины префикса для символа в позиции i
        while (j > 0 && s[i] != s[j]) //Пока символы не равны
            j = pi[j-1]; // // Берем ранее рассчитанное значение (начиная с максимально возможных)
        if (s[i] == s[j]) //Если равны, то увеличиваем j
            ++j;
        pi[i] = j;
    }
}


void kmp(std::string& P, std::string& T, int& answer){ //алгорим Кнута-Морисса-Пратта

    std::vector<int> pi(P.length()); //Вектор, содержащий значения длин префиксов
    prefix_function(P, pi);


    // k - количество совпавших символов, оно же индекс сравниваемого
    // символа в образце. В строке сравниваемый символ будет иметь индекс i
    for (int k = 0, i = 0; i < T.length(); ++i)
    {
        std::cout << "Going to be compared symbols:\nFrom substring #" << k << "(" << P[k]
        << ") and from text #" << i << "(" << T[i] << ")\n______________________________________\n";
        while ((k > 0) && (P[k] != T[i])) {// Очередной символ строки не совпал с символом в образце.
            k = pi[k - 1]; // Сдвигаем образец
        }

        if (P[k] == T[i]) //Символы совпали
            k++;          // увеличиваем длину совпавшего фрагмента на 1

        if (k == P.length()) {// образец найден
            answer = i - P.length() + 1;
            return;
        }
    }
    answer = -1;
}

int main() {

    char in, out;
    std::cout << "Enter how you want to read and write data(c - console, f - file)\n";
    std::cin >> in >> out;
    if((out != 'c' && out != 'f') || (in != 'c' && in != 'f')) {
        std::cout << "Incorrect data\n";
        return 1;
    }


    int answer = -1;
    std::string text, substring;

    if(in == 'c') {
        std::cin >> text >> substring;
    }
    else if(in == 'f') {
        std::ifstream input("C:/Users/Anton/CLionProjects/PAA_LAB4/input");
        input >> text >> substring;
    }
    text += text;

    kmp(substring, text, answer);

    if(out == 'c')
        std::cout << answer;
    else if(out == 'f') {
        std::ofstream output("C:/Users/Anton/CLionProjects/PAA_LAB4/output");
        output << answer;
    }


    return 0;
}