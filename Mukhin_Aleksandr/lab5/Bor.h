#ifndef LAB5_BOR_H
#define LAB5_BOR_H
#include <iostream>
#include <map>
#include <vector>
#include <fstream>

struct Vertex{
    Vertex(int parent, char symbol, bool is_leaf = false) :
            parent(parent), symbol(symbol), is_leaf(is_leaf), serial_number(-1), suff_link(-1), suff_flink(-1) {}

    int neighbors[5] = {-1, -1, -1, -1, -1};
    int serial_number;
    int suff_link;
    int auto_move[5] = {-1, -1, -1, -1, -1};
    int parent;
    int suff_flink;
    bool is_leaf;
    char symbol;
    std::vector<int> patterns_num{};
};

class Bor {
private:
    std::vector<int> start_positions;
    std::map<char, int> alphabet;
    std::vector<Vertex> repository;
    std::vector<std::string> patterns;
    std::string text;
    std::string pattern;
    char joker;
    char unacceptable_symbol;
    std::vector<int> length_vector;

    void add_pattern(const std::string& inserting_string) noexcept;
    [[nodiscard]] int get_suffix_link(const int vertex) noexcept;
    [[nodiscard]] int transition(int vertex, char symbol) noexcept;
    [[nodiscard]] int get_fast_suffix_link(int vertex) noexcept;
    void check(int vertex, int position_in_text) noexcept;

public:
    Bor(std::string& text, std::string& pattern, char& joker, char& unacceptable_symbol);
    void run(std::ostream& output) noexcept {
        std::string tmp_substring;

        for (int i = 0; i < pattern.size(); ++i) {            // построение бора из слов,
            if (pattern[i] == joker) {                        // не содержащих wildcard
                if (!tmp_substring.empty()) {
                    add_pattern(tmp_substring);
                    start_positions.push_back(i - tmp_substring.size());
                    tmp_substring.clear();
                }
                continue;
            }
            tmp_substring += pattern[i];
        }

        int another_vertex = 0;                                 // проход по тексту, с фиксированием листов и
        for (int i = 0; i < text.size(); ++i) {                 // заполнением массива C
            another_vertex = transition(another_vertex, alphabet[text[i]]);
            check(another_vertex, i);
        }

        for (int k = 0; k < length_vector.size(); ++k) {
            if (length_vector[k] == start_positions.size()) {
                bool is_joker = false;
                for (size_t i = k; i < k + pattern.size() - 1; ++i) {                   // проход по всему шаблону и проверка на
                    if (pattern[i - k] == joker && text[i - 1] == unacceptable_symbol){ // то, чтобы джокер в шаблоне не стоял
                        is_joker = true;                                                // на месте запрещённого символа в тексте
                        output << "Find unacceptable symbol!" << std::endl;
                        break;
                    }
                }

                if (!is_joker && k + pattern.size() - 1 <= text.size())
                    output << "Find match at " << k << " position." << std::endl;
            }
        }
    }
};


#endif
