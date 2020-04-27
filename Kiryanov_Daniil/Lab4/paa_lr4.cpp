#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::string input_way = "C:/Users/Danielka/source/repos/paa_lr4/input";//пути до файлов
std::string output_way = "C:/Users/Danielka/source/repos/paa_lr4/output";

void prefix(const std::string& S, std::vector<int>& n) {
	n[0] = 0;
	for (unsigned long int i = 1; i < S.size(); ++i) {
		int k = n[i - 1];//изначально инициализируем предыдущим
		while (k > 0 && S[k] != S[i])//пока символы не равны
			k = n[k - 1];//возвращаемся к уже найденному значению
		if (S[k] == S[i])//если равны
			k += 1;//увеличиваем значение
		n[i] = k;
	}
}

void KMP(std::istream& input) {
	std::string P;
	input >> P;//считываем шаблон
	std::vector<int> n(P.size());//вектор для префикс функции
	prefix(P, n);//вычисляем префикс функцию
	std::cout << "Prefix: ";
	for (int j : n)
		std::cout << j << " ";
	std::cout << std::endl;
	int k = 0;
	int result = -1;//если нет совпадений, то резльтат останется -1
	char vau = '0';
	input.get(vau);
	input.get(vau);
	std::vector<int> ans;//ответ
	int i = 0;
	while (vau != '\n' && !input.eof()) {//пока не конец строки/файла
		std::cout << "Changes when i = " << i << " Start value k = " << k << std::endl;
		while (k > 0 && vau != P[k]) {//пока не совпадут символы
			k = n[k - 1];//сдвигаем
			std::cout << " k = " << k << std::endl;
		}
		if (vau == P[k]) {//если совпали
			k += 1;//увеличиваем значение
			std::cout << " k = " << k << std::endl;
		}
		if (k == P.size()) {//если длина совпадений=длине строки
			result = i - P.size() + 1;//значит ответ получен
			ans.push_back(result);
			std::cout << "-----------------------------------------------------" << std::endl;
			std::cout << "Result found with i = " << i << " k = " << k 
				<< " Index = " << result << std::endl;
			std::cout << "-----------------------------------------------------" << std::endl;
		}
		i += 1;
		input.get(vau);
	}
	int menu = 0;
	std::cout << std::endl << "How do you want to output?" << std::endl << std::endl
		<< "Press 1 to output by console." << std::endl//выбор как вывести
		<< "Press 2 to output into file." << std::endl;
	while (menu != 1 && menu != 2) {//пока не введется нужная цифра
		std::cin >> menu;
		if (menu == 1) {//вывод на консоль
			std::cout << std::endl << "Result: ";
			if (!ans.empty())
				for (unsigned long int i = 0; i < ans.size() - 1; ++i) {
					std::cout << ans[i] << ",";
				}
			std::cout << result;
		}
		else if (menu == 2) {//вывод в файл
			std::ofstream file;
			file.open(output_way);
			if (!file.is_open()) {
				std::cout << "Can't open file!\n";
			}
			file << "Result: ";
			if (!ans.empty())
				for (unsigned long int i = 0; i < ans.size() - 1; ++i) {
					file << ans[i] << ",";
				}
			file << result;
		}
		else {//если неверно введена цифра, выводится сообщение
			std::cout << std::endl << "Wrong choice! Try again!" << std::endl;
		}//а цикл продолжается
	}
}


int main() {
	std::cout << "How do you want to input?" << std::endl << std::endl
		<< "Press 1 to input from console." << std::endl//выбор как считать
		<< "Press 2 to input from file." << std::endl;
	int menu = 0;
	while (menu != 1 && menu != 2) {//пока не введется нужная цифра
		std::cin >> menu;
		if (menu == 1) {//считывание с консоли
			KMP(std::cin);
		}
		else if (menu == 2) {//считывание из файла
			std::ifstream file;
			file.open(input_way);
			if (!file.is_open()) {
				std::cout << "Can't open file!" << std::endl;
				return 0;
			}
			KMP(file);
		}
		else {//если неверно введена цифра, выводится сообщение
			std::cout << std::endl << "Wrong choice! Try again!" << std::endl;
		}//а цикл продолжается
	}
	return 0;
}

