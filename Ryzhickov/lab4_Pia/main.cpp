#include <iostream>
#include <vector>

void createPiArray(std::vector<int> *vector, std::string *string, int length) {

    int j = 0;
    int i = 1;

    vector->emplace_back(0);

    while (length > i) {
        if (string->at(i) == string->at(j)) {
            vector->emplace_back(j + 1);
            i++;
            j++;
        } else {
            if (j == 0) {
                vector->emplace_back(0);
                i++;
            } else {
                j = vector->at(j - 1);
            }
        }
    }
}

int main() {
    std::string string;

    std::cin >> string;

    std::vector<int> vector;
    vector.reserve(0);

    std::vector<int> answer;
    vector.reserve(0);

    createPiArray(&vector, &string, string.length());

    char c;
    std::cin.get(c);

    int l = 0;
    int n = string.size();

    int count = 0;
    std::cin.get(c);
    while (true) {
        bool isCinActive = true;

        if (c == string[l]) {
            l++;
            count++;
            if (l == n) {
                answer.emplace_back(count - n);
            }
        } else {
            if (l == 0) {
                count++;
            } else {
                l = vector.at(l - 1);
                isCinActive = false;
            }
        }

        if (isCinActive) {
            std::cin.get(c);
        }

        if (c == '\n') {
            break;
        }
    }

    if (!answer.empty()) {
        for (size_t m = 0; m < answer.size(); ++m) {
            std::cout << answer[m];
            if (m != answer.size() - 1) std::cout << ",";
        }
    } else {
        std::cout << -1;
    }


    return 0;
}