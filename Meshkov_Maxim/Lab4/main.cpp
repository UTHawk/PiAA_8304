#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>

std::ostream *out;
std::istream *in;

bool verbose = false;

// Вычисляет префикс функцию строки s
std::vector<int> countPrefix(const std::string &s) {
    std::vector<int> prefix(s.size(), 0);

    for (int i = 1; i < s.size(); i++) {
        int k = prefix[i - 1];
        for ( ; k > 0 && s[k] != s[i]; k = prefix[k - 1])
            ;
        if (s[k] == s[i])
            k++;
        prefix[i] = k;
    }
    return prefix;
}

// Выполняет Алгоритм Кнута — Морриса — Пратта, возвращает массив вхождений word в text
std::vector<int> kmpAlgorithm(const std::string &word, const std::string &text) {
    if (verbose) *out << "Starting KMP algorithm." << std::endl;

    // Массив для сохранения результата функции
    std::vector<int> result;

    if (verbose) *out << "Counting prefix function for word." << std::endl;
    auto wordPrefix = countPrefix(word);
    if (verbose) {
        *out << "Word prefix: ";
        for (int i = 0; i < wordPrefix.size(); i++)
            *out << "[" << i << "]" << wordPrefix[i] << " ";
        *out << std::endl;
    };

    if (verbose) *out << "Start counting prefix function for every character of the text string." << std::endl;
    if (verbose) *out << "Target prefix value is " << word.size() << " (word length)." << std::endl;
    int prevK = 0;
    for (int i = 0; i < text.size(); i++) {
        int k = prevK;
        for ( ; k > 0 && word[k] != text[i]; k = wordPrefix[k - 1])
            ;
        if (word[k] == text[i])
            k++;
        prevK = k;

        if (k == word.size())
            result.push_back(i - word.size() + 1);

        if (verbose) *out << "[" << i << "] " << k << (k == word.size() ? " - match!" : "") << std::endl;
    }

    return result;
}

int main(int argc , char *argv[]) {
    // Настройка ввода и вывода
    std::ifstream inFile;
    std::ofstream outFile;
    in = &std::cin;
    out = &std::cout;

    // Проверка параметров командной строки (нужно ли включить пошаговый вывод)
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0)
            verbose = true;
        if (strcmp(argv[i], "-fi") == 0) {
            in = &inFile;
            inFile.open("in.txt");
        }
        if (strcmp(argv[i], "-fo") == 0) {
            out = &outFile;
            outFile.open("out.txt");
        }
    }

    // word будет искаться в text
    std::string word, text;

    // Ввод
    getline(*in, word);
    getline(*in, text);

    // Обработка
    auto result = kmpAlgorithm(word, text);

    // Вывод результатов
    if (verbose) *out << "Result: ";
    if (!result.empty()) {
        for (int i = 0; i < (int)result.size() - 1; i++) {
            *out << result[i] << ",";
        }
        *out << result.back();
        *out << std::endl;
    }
    else {
        *out << -1 << std::endl;
    }

    return 0;
}

