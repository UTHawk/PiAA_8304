#include <iostream>
#include <vector>
#include <string>
#include <fstream>


std::vector<size_t> pi_func(std::string form) {
    size_t len_form = form.size();
    std::vector<size_t> local_pi(len_form);

    for (size_t i = 1; i < len_form; ++i) {
        size_t j = local_pi[i-1];
        while (j > 0 && form[i] != form[j]) {
            j = local_pi[j - 1];
        }
        if (form[i] == form[j]) {
            ++j;
        }
        local_pi[i] = j;
    }

    return local_pi;
}


void KMP(std::istream& input, std::ostream& output) noexcept {
    bool is_result = false;
    std::string pattern;
    input >> pattern;
    char tmp = '1';

    std::vector<size_t> pi = pi_func(pattern);
    size_t j = 0;
    size_t counter = 0;
    input.get();
    input.get(tmp);
    while(input.peek() != EOF) {
        while (pattern[j] == tmp) {                                            // пока символы совпадают продолжаем сравнение
            input.get(tmp);
            ++j;
            ++counter;
        }
        if (j == pattern.length()) {                                           // по окончанию сравнения проверяем, равен ли счётчик, указывающий
            output << "Match at: " << counter - j << " position" << std::endl; // на шаблон длине шаблона (т. е. найдено совпадение). сдвигаем j
            j = pi[j-1];                                                       // во избежания пропуска, вложенных совпадений.
            is_result = true;
        } else {
            if (j != 0) {                                                      // если символ не первый, сдвигаем на максимально возможный размер,
                j = pi[j-1];                                                   // указанный в массиве pi.
                output << "Move right at " << j << " positions instead of " << pattern.length() << std::endl;
            } else {
                input.get(tmp);                                             // если первый, то считываем новый символ и увеличиваем позицию в строке.
                ++counter;
            }
        }
    }
    if (!is_result) {
        output << -1;
    }
    output << std::endl;
}


int main() {
    int input_mode = 0;
    int output_mode = 0;
    std::cout << "Tap 1 for console input, 0 for file input:";
    std::cin >> input_mode;
    std::cout << "Tap 1 for console output, 0 for file output:";
    std::cin >> output_mode;


    if (input_mode == 1 && output_mode == 1) {
        KMP(std::cin, std::cout);
    } else if (input_mode == 1 && output_mode == 0) {
        std::ofstream output("./output.txt", std::ios::out | std::ios::trunc);
        KMP(std::cin, output);
    } else if (input_mode == 0 && output_mode == 1) {
        std::ifstream input("./input.txt");
        KMP(input, std::cout);
    } else if (input_mode == 0 && output_mode == 0) {
        std::ifstream input("./input.txt");
        std::ofstream output("./output.txt", std::ios::out | std::ios::trunc);
        KMP(input, output);
    } else {
        std::cout << "You can tap 0 or 1" << std::endl;
    }
    return 0;

}
