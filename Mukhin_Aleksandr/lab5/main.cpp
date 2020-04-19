#include "Bor.h"


int main() {
    int input_mode = 0;
    int output_mode = 0;
    std::cout << "0 for terminal input, 1 for file input:";
    std::cin >> input_mode;

    std::cout << "0 for terminal output, 1 for file output:";
    std::cin >> output_mode;


    std::string text;
    std::string pattern;
    char joker;
    char unacceptable_symbol;

    if (input_mode == 1) {
        std::ifstream input("input.txt");
        if (input.is_open()) {
            input >> text;
            input >> pattern;
            input >> joker;
            input >> unacceptable_symbol;
        } else {
            std::cout << "Can't open input file!" << std::endl;
        }
    } else {
        std::cin >> text;
        std::cin >> pattern;
        std::cin >> joker;
        std::cin >> unacceptable_symbol;
    }

    Bor bor(text, pattern, joker, unacceptable_symbol);

    if (output_mode == 0) {
        bor.run(std::cout);
    } else if (output_mode == 1) {
        std::ofstream output("output.txt", std::ios::out | std::ios::trunc);
        bor.run(output);
    } else {
        std::cout << "Wrong prameters!" << std::endl;
    }
    return 0;
}