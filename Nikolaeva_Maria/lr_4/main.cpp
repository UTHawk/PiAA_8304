#include <iostream>
#include <fstream>
#include <vector>
#include <string>


constexpr bool DEBUG = true;
constexpr const char* PATH_INPUT = "D:/test.txt";
constexpr const char* PATH_OUTPUT = "D:/result.txt";


std::vector<size_t> prefixFunction (const std::string& string) {
    if (DEBUG) {
        std::cout << "Calculating prefix\nText: " << string << "\n";
        std::cout << "Prefix 0 = 0\n";
    }
    // в i-м элементе (его индекс i-1) количество совпавших
    // символов в начале образца и в конце подстроки длины i.
    // p[0]=0 всегда
    std::vector<size_t> pi(string.size());
    pi[0] = 0;

    // заполняем массив длин префиксов для образца
    for (size_t i = 1; i < string.size(); ++i) {
        size_t j = pi[i-1];

        while ((j > 0) && (string[i] != string[j])) {
            j = pi[j-1];
        }

        if (string[i] == string[j]) {
            ++j;
        }

        if (DEBUG) {
            std::cout << "Prefix " << i << " = " << j << "\n";
        }

        pi[i] = j;
    }

    return pi;
}


std::vector<int> KMP(std::istream& input,
                     const std::string& pattern) {
    std::vector<int> result;
    size_t size = pattern.size();

    auto pi = prefixFunction(pattern);

    size_t k = 0;
    size_t i = 0;

    if (DEBUG) {
        std::cout << "Enter text and press enter and ctrl + z: ";
    }

    while (!input.eof()) {
        char ch = 0;
        input >> ch;

        if (DEBUG) {
            std::cout << "Char: " << ch << "\n";
        }

        while ((k > 0) && (pattern[k] != ch)) {
            // Очередной символ строки не совпал с символом в образце. Сдвигаем образец,
            // причем точно знаем, что первые k символов образца совпали с символами строки
            // и надо сравнить k+1-й символ образца (его индекс k) с i-м символом строки.
            if (DEBUG) {
                std::cout << pattern[k] << " !=  " << ch << "\n";
            }

            k = pi[k-1];
        }

        if (pattern[k] == ch) {
            if (DEBUG) {
                std::cout << pattern[k] << " ==  " << ch << "\n";
            }
            // есть совпадение очередного символа
            // увеличиваем длину совпавшего фрагмента на 1
            k++;
        }

        if (DEBUG) {
            std::cout << "Size of pattern: " << size << ". k = " << k << "\n";
        }

        if (k == size) {
            if (DEBUG) {
                std::cout << "Pattern found! Index: " << i + 1 - size << "\n";
            }
            // образец найден
            result.push_back(i + 1 - size);
        }
        ++i;
    }

    if (result.size() == 0) {
        result.push_back(-1);
    }

    return result;
}


void cyclicShift(const std::string& firstString,
                 const std::string& secondString) {
    if (DEBUG) {
        std::cout << "CyclicShift\n";
    }
    else if (firstString == secondString) {
        if (DEBUG) {
            std::cout << "Strings are equal\n";
        }
        //если строки равны
        std::cout << 0;
        return;
    }

    size_t size = firstString.size();

    char* buff = new char[3 * size];

    //контатенация одной первой строки и двух вторых
    size_t i = 0;
    for (; i < size; ++i) {
        buff[i] = secondString[i];
    }
    for (int k = 0; k < 2; ++k) {
        for (size_t j = 0; j < size; ++j) {
            buff[i++] = firstString[j];
        }
    }

    //вычисление префикс функции для итоговой строки
    auto pi = prefixFunction(buff);

    for (size_t i = 2 * size - 1; i < 3 * size; ++i) {

        if (pi[i] >= size) {
            //если в каком-то месте префикс функция больше или равна размеру первой строки,
            //то вторая строка является циклическим сдвигом
            if (DEBUG) {
                std::cout << "It's cyclicShift!\n";
            }

            std::cout << i + 1 - 2 * size;
            delete [] buff;
            return;
        }
    }

    //если не является
    if (DEBUG) {
        std::cout << "It's not cyclicShift!\n";
    }
    std::cout << -1;
    delete [] buff;
}


void writeRes(std::ostream& output, std::vector<int>& result) {
    if (DEBUG) {
        output << "Indices: ";
    }

    for (size_t i = 0; i < result.size(); ++i) {
        output << result[i];
        if (i != result.size() - 1) {
            output << ',';
        }
    }
    output << "\n";
}


int main() {
    int chooseInput = 0;
    int chooseOutput = 0;
    std::string pattern;
    std::vector<int> result;

    if (DEBUG) {
        std::cout << "Input from: 0 - console, 1 - file: ";
        std::cin >> chooseInput;
        std::cout << "Write to: 0 - console, 1 - file: ";
        std::cin >> chooseOutput;
    }

    if (chooseInput == 0) {
        if (DEBUG) {
            std::cout << "Enter pattern: ";
        }
        std::cin >> pattern;
        result = KMP(std::cin, pattern);
    }
    else if (chooseInput == 1) {
        std::ifstream file;
        file.open(PATH_INPUT);

        if (file.is_open()) {
            file >> pattern;
            result = KMP(file, pattern);
            file.close();
        }
        else {
            std::cout << "Can't open file!\n";
            return 0;
        }
    }
    else {
        std::cout << "Incorrect input!\n";
        return 0;
    }

    if (chooseOutput == 0) {
        writeRes(std::cout, result);
    }
    else if (chooseOutput == 1) {
        std::ofstream file;
        file.open(PATH_OUTPUT);
        writeRes(file, result);
        file.close();
    }
    else {
        std::cout << "Incorrect input!\n";
        return 0;
    }

    return 0;
}
