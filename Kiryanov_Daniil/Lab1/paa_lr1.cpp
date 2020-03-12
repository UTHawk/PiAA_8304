#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

struct coord {
	int x;
	int y;
	int len;
};

void solve(std::vector<std::vector<int>>& rectangle, std::vector<coord>& potential, std::vector<coord>& answer, int min, int x, int y){
	coord point;
	bool check = false;
	for (int i = 0; i < x; ++i){
		for (int j = 0; j < y; ++j)
			if (rectangle[i][j] == 0){
				point.x = i;
				point.y = j;
				check = true;
				break;
			}
		if (check)
			break;
	}
	if (check == false){
		answer = potential;
		return;
	}
	for (int size = 1; size < min; ++size){
		if (point.x + size > x || point.y + size > y)
			return;
		if (potential.size() + 1 >= answer.size())
			return;
		for (int i = point.x; i < point.x + size; ++i)
			for (int j = point.y; j < point.y + size; ++j)
				if (rectangle[i][j] == 1)
					return;
		for (int i = point.x; i < point.x + size; ++i)
			for (int j = point.y; j < point.y + size; ++j)
				rectangle[i][j] = 1;
		point.len = size;
		potential.push_back(point);
		solve(rectangle, potential, answer, min, x, y);
		for (int i = point.x; i < point.x + size; ++i)
			for (int j = point.y; j < point.y + size; ++j)
				rectangle[i][j] = 0;
		potential.pop_back();

	}
}

int main(){
	int x = 0;
	std::cout << "x = ";
	std::cin >> x;
	std::cout << "\ny = ";
	int y = 0;
	std::cin >> y;
	int min = 0;
	int max = 0;
	if (y > x) {
		min = x;
	}
	else {
		min = y;
	}
	std::vector<std::vector<int>> rectangle(x);
	for (int i = 0; i < x; ++i){
		std::vector<int> side(y);
		for (int j = 0; j < y; ++j)
			side[j] = 0;
		rectangle[i] = side;
	}
	std::vector<coord> potential;
	std::vector<coord> answer(x * y);
	clock_t time = clock();
	solve(rectangle, potential, answer, min, x, y);
	time = clock() - time;
	std::cout << answer.size() << "\n";
	for (size_t i = 0; i < answer.size(); ++i)
		std::cout << answer[i].x
		<< " " <<
		answer[i].y
		<< " " <<
		answer[i].len
		<< std::endl;
	std::cout << (double)(time) / CLOCKS_PER_SEC;
	return 0;
}