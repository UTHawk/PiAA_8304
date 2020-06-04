#include "Bor.h"


void Bor::add_pattern(const std::string& inserting_string) noexcept {
    int another_vertex = 0;
    for (char symbol : inserting_string) {
        char ch = alphabet[symbol];
        if (repository[another_vertex].neighbors[ch] == -1) {
            repository.emplace_back(Vertex(another_vertex, ch));
            repository[another_vertex].neighbors[ch] = repository.size() - 1;
        }
        another_vertex = repository[another_vertex].neighbors[ch];
    }
    repository[another_vertex].is_leaf = true;
    patterns.emplace_back(inserting_string);
    repository[another_vertex].patterns_num.emplace_back(patterns.size() - 1);
}

[[nodiscard]] int Bor::get_suffix_link(const int vertex) noexcept {
    if (repository[vertex].suff_link == -1) {
        if (vertex == 0 || repository[vertex].parent == 0) {
            repository[vertex].suff_link = 0;
        } else {
            repository[vertex].suff_link = transition(get_suffix_link(repository[vertex].parent), repository[vertex].symbol);
        }
    }
    return repository[vertex].suff_link;
}

[[nodiscard]] int Bor::transition(int vertex, char symbol) noexcept {
    if (repository[vertex].auto_move[symbol] == -1) {
        if (repository[vertex].neighbors[symbol] != -1) {
            repository[vertex].auto_move[symbol] = repository[vertex].neighbors[symbol];
        } else {
            if (vertex == 0) {
                repository[vertex].auto_move[symbol] = 0;
            } else {
                repository[vertex].auto_move[symbol] = transition(get_suffix_link(vertex), symbol);
            }
        }
    }
    return repository[vertex].auto_move[symbol];
}

[[nodiscard]] int Bor::get_fast_suffix_link(int vertex) noexcept {
    if (repository[vertex].suff_flink == -1) {
        int tmp = get_suffix_link(vertex);
        if (tmp == 0) {
            repository[vertex].suff_flink = 0;
        } else {
            repository[vertex].suff_flink = repository[vertex].is_leaf ? tmp : get_fast_suffix_link(tmp);
        }
    }
    return repository[vertex].suff_flink;
}

void Bor::check(int vertex, int position_in_text) noexcept {
    for (int u = vertex; u != 0; u = get_fast_suffix_link(u)) {
        if (repository[u].is_leaf) {
            for (int k : repository[u].patterns_num) {
                size_t j = position_in_text - patterns[k].size() + 1;
                if (j - start_positions[k] + 1 > 0 && j - start_positions[k] + 1 < length_vector.size()) {
                    ++length_vector[j - start_positions[k] + 1];
                }
            }
        }
    }
}

Bor::Bor(std::string& text, std::string& pattern, char& joker, char& unacceptable_symbol) : text(text), pattern(pattern), joker(joker), unacceptable_symbol(unacceptable_symbol) {
    repository.emplace_back(Vertex(0, '#'));
    alphabet['A'] = 0;
    alphabet['C'] = 1;
    alphabet['G'] = 2;
    alphabet['T'] = 3;
    alphabet['N'] = 4;
    length_vector = std::vector<int>(text.size());
}