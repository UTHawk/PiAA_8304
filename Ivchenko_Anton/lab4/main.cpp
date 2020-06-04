#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>

std::vector<int> prefix_function(const std::string& s) {

	std::vector<int> pi(s.length(), 0);

	for (int i = 1; i < s.length(); i++) {
		int j = pi[i - 1];

		while (j > 0 && s[i] != s[j]) {
			j = pi[j - 1];
		}

		if (s[i] == s[j]) {
			pi[i] = j + 1;
		}
		else {
			pi[i] = j;
		}
		
	}
	return pi;
}

int cyclicalKMP(std::string& str, std::string& sample, std::ostream& out) {

	if (str.size() != sample.size()) {
		std::cout << "Incorrect\n";
		return -1;
	}

	std::vector<int> solution;
	std::string search_str = sample + '.' + str + str;

	std::vector<int> pi = prefix_function(search_str);
	for (auto a : pi) out << a << ' ';
	
	for (int i = 0; i < str.length() * 2; i++) {
		if (pi[sample.length() + 1 + i] == sample.length())
			return i - sample.length() + 1;
	}
	return -1;
}

std::vector<int> simpleKMP(std::string& t, std::string& s, std::ostream& out) {

	std::string search = s + '.' + t;

	std::vector<int> pi = prefix_function(search);
	for (auto a : pi) out << a << ' ';

	std::vector<int> solution;

	for (int i = 0; i < t.length(); i++) {
		if (pi[s.length() + 1 + i] == s.length()) {
			solution.push_back(i - s.length() + 1);

		}
	}
	if (solution.size() == 0) { std::cout << "Not any entries"; }
	else return solution;
	
}

std::vector<std::string> parallel_string(std::string& str, std::string& sample) {

	std::vector<std::string> parts;

	int partSize = str.size() / sample.size();//разделение строки на sample.size() частей
	int partQ = str.size() / partSize;

	for (int i = 0; i < partQ; i++) {
		parts.push_back(str.substr(i * partSize , partSize + sample.size() - 1));//строки "перекрываются" sample.size()-1 элементами
	}
	return parts;



}
int main() {


	std::string s, t;

	int input_ch, output_ch;

	std::cout << "Input: console = 0; file = 1\n";
	std::cin >> input_ch;

	if (input_ch == 1) {
		std::ifstream infile("input.txt");
		infile >> t >> s;
	}
	else if (input_ch == 0) std::cin >> t >> s;

	std::cout << "Output:\nconsole(simpleKMP)&file(prefix-function) = 0\nconsole(cyclical)&file(prefix-function) =  1\nconsole(parallelKMP)&file(prefix-function) = 2" << std::endl;
	std::cin >> output_ch;

	std::ofstream file;
	file.open("prefix_function.txt");
	if (!file.is_open()) {
		std::cout << "Incorrect!\n";
		return 0;
	}

	if (output_ch == 0) {
		std::vector <int> result = simpleKMP(t, s, file);
		for (int i = 0; i < result.size(); i++) {
			std::cout << result[i];
			if (i != result.size() - 1)
				std::cout << ",";
		}
	}
	else if(output_ch == 1){
		std::cout << cyclicalKMP(t, s, file);
	}
	else if (output_ch == 2) {
		std::vector<std::string> split_str = parallel_string(t, s);
		std::vector<int> pos;
		int k = 0, q = 0;
		for (auto a : split_str) {

			for (auto j : a) //пошаговый вывод обрезков строки
				std::cout << j;
			std::cout << std::endl;
			
			std::vector<int> part_solution = simpleKMP(a, s, file);
			for (auto i : part_solution) {
				pos.push_back(i + k * (t.size() / s.size()));
				std::cout << i << ' ';
			}
			std::cout << std::endl;
			k++;
		}
		for (int i = 0; i < pos.size(); i++) {
			std::cout << pos[i];
			if (i != pos.size() - 1)
				std::cout << ",";

		}

	}
	file.close();
	return 0;

}
