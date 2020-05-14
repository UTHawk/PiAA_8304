#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <fstream>

struct elem {
	std::vector<std::pair<char, int>> ways;
	int length = 0;
};

bool cmp(const std::pair<char, int>& a, const std::pair<char, int>& b) {
	if (a.second == b.second) return a.first > b.first;
	return a.second < b.second;
}

int get_length(char a, std::map<char, elem>& my_map) { return my_map[a].length; }

int evristic(char a, char b) { return abs(a - b); }

int total(char a, char where, std::map<char, elem>& my_map) { return get_length(a, my_map) + evristic(a, where); }

char MIN_F(std::set<char>& open, char where, std::map<char, elem>& my_map) {
	int min = std::numeric_limits<int>::max();
	char curr = 0;
	for (char i : open)
		if (total(i, where, my_map) < min) {
			min = total(i, where, my_map);
			curr = i;
		}
	return curr;
}

std::string make_path(std::map<char, char>& from, char start, char where) {
	std::string path = { where };
	char curr = where;
	while (curr != start) {
		curr = from[curr];
		path += curr;
	}
	std::reverse(path.begin(), path.end());
	return path;
}

std::string findWay(char start, char end, std::map<char, elem>& my_map) {
	char curr = start;
	std::string result;
	std::set<char> closed_set;
	std::set<char> open_set = { start };
	std::map<char, char> path_syms;
	while (!open_set.empty()) {
		curr = MIN_F(open_set, end, my_map);

		std::cout << curr;

		if (curr == end) {
			result = make_path(path_syms, start, end);
			std::cout << std::endl << result;
			return result;
		}
		open_set.erase(curr);
		closed_set.insert(curr);

		for (auto neighbour : my_map[curr].ways) {

			bool tentative_is_better;
			if (closed_set.find(neighbour.first) != closed_set.end()) {
				std::cout << "\nwe've already been to " << neighbour.first << std::endl;
				continue;
			}
			int tentative_g_score = get_length(curr, my_map) + neighbour.second;
			if (open_set.find(neighbour.first) == open_set.end()) {
				open_set.insert(neighbour.first);
				tentative_is_better = true;
			}
			else {
				tentative_is_better = tentative_g_score < get_length(neighbour.first, my_map);
			}

			if (tentative_is_better) {
				path_syms[neighbour.first] = curr;
				my_map[neighbour.first].length = tentative_g_score;
			}
		}
	}
	return result;
}

size_t shortest_way(std::map<char, elem>& my_map,char start_ch, char end_ch, size_t min_length, size_t current_length, size_t buff) {
	if (start_ch == end_ch) {
		return current_length;
	}
	for (size_t i = 0; i < my_map[start_ch].ways.size(); i++) {
		buff = shortest_way(my_map, my_map[start_ch].ways[i].first, end_ch, min_length, current_length + my_map[start_ch].ways[i].second, buff);
			if (min_length > buff)
				min_length = buff;
	}
	return min_length;
}

bool check_monotony(std::map<char, elem>& my_map, char end_ch) {
	for (auto it = my_map.begin(); it != my_map.end(); ++it) {
		for (size_t i = 0; i < it->second.ways.size(); i++) {
			if (evristic(it->first, end_ch) - evristic(it->second.ways[i].first, end_ch) > it->second.ways[i].second)
				return false;
		}
	}
	return true;
}

bool check_ambissibility(std::map<char, elem>& my_map, char end_ch) {
	if (check_monotony(my_map, end_ch))
		return true;

	for (auto it = my_map.begin(); it != my_map.end(); ++it) {
		if (evristic(it->first, end_ch) > shortest_way(my_map, it->first, end_ch, std::numeric_limits<size_t>::max(), 0, 0))
			return false;
	}

	return true;
}

void file_input(char*& argv) {

	std::ifstream file;
	std::string testfile = argv;

	std::string out_file_name = "output.txt";

	char start, end;
	char a, b;
	int c = 0;

	std::map<char, elem> my_map;

	file.open(testfile);

	if (!file.is_open()) {
		std::cout << "Error! File isn't open" << std::endl;
		return;
	}

	file >> start >> end;

	while (!file.eof()) {
		file >> a >> b >> c;
		my_map[a].ways.push_back({ b,c });
		std::sort(my_map[a].ways.begin(), my_map[a].ways.end(), cmp);
	}

	for (auto it = my_map.begin(); it != my_map.end(); ++it)
		for (size_t i = 0; i < it->second.ways.size(); i++)
			std::cout << it->first << " : " << it->second.ways[i].first << " " << it->second.ways[i].second << std::endl;


	if (check_ambissibility(my_map, end))
		std::cout << "allowable\n";
	else std::cout << "not allowable\n";
	if (check_monotony(my_map, end))
		std::cout << "monotone\n";
	else std::cout << "not monotone\n";

	std::ofstream out_file;
	out_file.open(out_file_name);

	if (!out_file.is_open()) {
		std::cout << "Error! Output file isn't open" << std::endl;
	}

	std::string result = findWay(start, end, my_map);

	out_file << result;

}



int main(int argc, char** argv) {

	

	size_t input_num;

	std::cout << "Please, choose input type:\n Enter \"1\" to use console input\n Enter \"2\" to use file input\n";

	std::cin >> input_num;

	if (input_num == 2) {
		if (argc == 1) {
			std::cout << "Please, check arguments are correct\n";
			return 0;
		}
		file_input(argv[1]);
	}

	else if(input_num == 1){

		std::string out_file_name = "output.txt";

		char start, end;
		std::cin >> start >> end;
		char a, b;
		int c = 0;
		std::map<char, elem> my_map;
		while (std::cin >> a >> b >> c) {
			if (c == -1) break;
			my_map[a].ways.push_back({ b,c });
			std::sort(my_map[a].ways.begin(), my_map[a].ways.end(), cmp);
		}

		for (auto it = my_map.begin(); it != my_map.end(); ++it)
			for (size_t i = 0; i < it->second.ways.size(); i++)
				std::cout << it->first << " : " << it->second.ways[i].first << " " << it->second.ways[i].second << std::endl;

		if (check_ambissibility(my_map, end))
			std::cout << "allowable\n";
		else std::cout << "not allowable\n";
		if (check_monotony(my_map, end))
			std::cout << "monotone\n";
		else std::cout << "not monotone\n";

		

		std::ofstream out_file;
		out_file.open(out_file_name);

		if (!out_file.is_open()) {
			std::cout << "Error! Output file isn't open" << std::endl;
		}

		std::string result = findWay(start, end, my_map);

		out_file << result;
	}
	return 0;
}