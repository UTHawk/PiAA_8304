#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

struct Square
{
	int x;
	int y;
	int lenght;
};

void search(std::vector<int>& tabletop, std::vector<Square>& current_result, std::vector<Square>& min_result, int n, int step, std::vector<int>& iter)
{
	Square check_square;
	bool fl = false;

	iter[0]++;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if (tabletop[i * n + j] == 0)
			{
				check_square.x = i;
				check_square.y = j;
				fl = true;
				break;
			}
		}

		if (fl)
			break;
	}

	if (fl == false)
	{
		min_result = current_result;
		return;
	}

	for (int current_lenght = step; current_lenght < n; current_lenght += step)
	{
		iter[0]++;
		if (check_square.x + current_lenght > n || check_square.y + current_lenght > n)
			return;

		iter[0]++;
		if (current_result.size() + 1 >= min_result.size())
			return;

		iter[0]++;
		for (int i = check_square.x; i < check_square.x + current_lenght; ++i)
			for (int j = check_square.y; j < check_square.y + current_lenght; ++j)
				if (tabletop[i * n + j] == 1)
					return;

		iter[0]++;
		for (int i = check_square.x; i < check_square.x + current_lenght; ++i)
			for (int j = check_square.y; j < check_square.y + current_lenght; ++j)
				tabletop[i * n + j] = 1;


		check_square.lenght = current_lenght;
		current_result.push_back(check_square);

		search(tabletop, current_result, min_result, n, step, iter);

		iter[0]++;
		for (int i = check_square.x; i < check_square.x + current_lenght; ++i)
			for (int j = check_square.y; j < check_square.y + current_lenght; ++j)
				tabletop[i * n + j] = 0;

		current_result.pop_back();

	}
}


int check(int n)
{
	int a = 1;
	for (int i = 2; i < n; ++i)
	{
		if (n % i == 0)
			a = i;
	}

	return a;
}


int main()
{
	int n = 0;
	std::cin >> n;

	std::vector<int> iter(1);

	iter[0]++;
	int min_side = check(n);

	std::vector<Square> current_result;
	std::vector<Square> min_result(n * 2 + 1);
	std::vector<int> tabletop(n * n, 0);
	


	if (min_side == 1 && n != 2)
	{

		for (int i = 0; i < n / 2; ++i)
			for (int j = 0; j < n / 2; ++j)
				tabletop[i * n + j] = 1;

		for (int i = n / 2; i < n; ++i)
			for (int j = 0; j < n / 2 + 1; j++)
				tabletop[i * n + j] = 1;

		current_result.push_back({ 0, 0, n / 2 });
		current_result.push_back({ n / 2, 0, n / 2 + 1 });
		iter[0] += 2;
	}
	
	search(tabletop, current_result, min_result, n, min_side, iter);

	std::cout << min_result.size() << "\n";
	for (auto i : min_result)
		std::cout << i.x << " " << i.y << " " << i.lenght << "\n";

	std::cout << iter[0];
	return 0;
}
