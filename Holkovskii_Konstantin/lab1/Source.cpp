#include <iostream>
#include <vector>

int size;

struct elem {
	int x;
	int y;
	int len;
};

void func(int * a, std::vector<elem>& tmp, std::vector<elem>& min, int& count) {
	bool f1 = false;
	elem cur;
	/*
	int* arr = new int[(size * 2 - 1) * (2 * size - 1)]();
	for (size_t i = 0; i < tmp.size(); ++i)
		for (int y = tmp[i].y; y < tmp[i].y + tmp[i].len; ++y)
			for (int x = tmp[i].x; x < tmp[i].x + tmp[i].len; ++x)
				arr[y * (size * 2 - 1) + x] = 'a' + i;
	for (int i = 0; i < size * 2 - 1; ++i) {
		for (int j = 0; j < size * 2 - 1; ++j)
			std::cout << (arr[i * (size * 2 - 1) + j] != 0 ? static_cast<char>(arr[i * (size * 2 - 1) + j]) : '*') << " ";
		std::cout << std::endl;
	}
	delete[] arr;
	std::cout << std::endl;*/

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j)
			if (a[i * size + j] == 0) {
				f1 = true;
				cur = { i, j, 1 };
				break;
			}
		if (f1)
			break;
	}
	if (!f1)
	{
		min = tmp;
		return;
	}

	for (int tmp_len = 1; tmp_len < size && tmp.size() + 1 != min.size(); ++tmp_len)
	{
		if (tmp_len > size - cur.x || tmp_len > size - cur.y)
			return;
		++count;

		for (int i = cur.x; i < cur.x + tmp_len; ++i)
			for (int j = cur.y; j < cur.y + tmp_len; ++j)
				if (a[i * size + j] == 1)
					return;

		for (int i = cur.x; i < cur.x + tmp_len; ++i)
			for (int j = cur.y; j < cur.y + tmp_len; ++j)
				a[i * size + j] = 1;

		cur.len = tmp_len;                                                                                                                                                                                                 tmp.push_back(cur);
		func(a, tmp, min, count);
		for (int i = cur.x; i < cur.x + tmp_len; ++i)
			for (int j = cur.y; j < cur.y + tmp_len; ++j)
				a[i * size + j] = 0;
		tmp.pop_back();
	}
}
int main() {
	int n;
    std::cin >> n;
    for (size = 2; size < n; ++size)
		if (n % size == 0)
			break;
    std::vector<elem> tmp, min(size + 4);
    tmp.push_back({ 0, size / 2 + size % 2, size / 2});
    tmp.push_back({ size / 2 + size % 2, 0, size / 2});
	tmp.push_back({ size / 2, size / 2, size / 2 + size % 2 });
	int buf = n / size;
	int count = 0;
	if (size % 2 == 1) {
		size = size / 2 + 1;
		int* my_arr = new int[size * size]();
		my_arr[size *size - 1] = 1;
		func(my_arr, tmp, min, count);
		delete[] my_arr;
	}
	else {
		tmp.push_back({ 0, 0, 1 });
		min = tmp;
	}
	std::cout << min.size() << std::endl;
	int* arr = new int[n*n]();
	for (size_t i = 0; i < min.size(); ++i)
		for (int y = min[i].y * buf; y < min[i].y * buf + min[i].len * buf; ++y)
			for (int x = min[i].x * buf; x < min[i].x * buf + min[i].len * buf; ++x)
				arr[y * n + x] = 'a' + i;
	//	std::cout << min[i].x * buf << " " << min[i].y * buf << " " << min[i].len * buf << std::endl;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			std::cout << (char)arr[i * n + j] << " ";
		std::cout << std::endl;
	}
	//std::cout << "Кол-во итераций: " << count << std::endl;
	delete[] arr;
	return 0;
}

