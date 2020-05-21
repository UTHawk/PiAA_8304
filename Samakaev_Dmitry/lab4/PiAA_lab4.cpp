#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

std::vector<size_t> get_prefix(std::string sample) {
	std::vector<size_t> prefix(sample.length(), 0);
	for (size_t i = 1; i < sample.length(); i++) {
		size_t j = 0;
		while (i + j < sample.length() && sample[j] == sample[i + j]) {
			prefix[i + j] = std::max(prefix[i + j], j + 1);
			j++;
		}
	}
	return prefix;
}

int KMPSearch(std::string text, std::string sample) {
	std::vector<size_t> found;
	std::vector<size_t> prefix = get_prefix(sample);
	
	for (size_t i = 0; i < prefix.size(); i++)
		std::cout << prefix[i];
	std::cout<<std::endl;

	size_t i = 0;
	size_t j = 0;

	text += text;

	while (i < text.length()) {
		
		if (sample[j] == text[i]) {
			i++;
			j++;
			if (j == sample.length())
				return i - j;
		}

		if (j == sample.length())
			j = prefix[j - 1];


		if (i < text.length() && sample[j] != text[i]) {
			if (j != 0) {
				j = prefix[j - 1];
			}
			else {
				i++; 
			}
		}
	}
	return -1;
}

void console_input() {

	std::cout << "Please, Enter the text\n";

	std::string text;
	std::string sample;

	std::cin >> text;

	std::cout << "Please, Enter the sample\n";

	std::cin >> sample;

	std::cout << KMPSearch(text, sample);
}

void file_input(char*& argv) {
	std::ifstream file;
	std::string testfile = argv;
	file.open(testfile);

	if (!file.is_open()) {
		std::cout << "Error! File isn't open" << std::endl;
		return;
	}

	std::string text;
	std::string sample;

	while (!file.eof()) {
		file >> text;
		file >> sample;
	}

	std::cout << KMPSearch(text, sample);
}

int main(size_t argc, char** argv)
{
	if (argc == 1)
		console_input();
	else if (argc == 2)
		file_input(argv[1]);
}
