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

void KMP(std::istream& input, std::ostream& output) {
	std::string P;
	input >> P;//считываем шаблон
	std::vector<int> n(P.size());//вектор для префикс функции
	prefix(P, n);//вычисляем префикс функцию
	output << "Prefix: ";
	for (int j : n)
		output << j << " ";
	output << std::endl;
	int k = 0;
	int result = -1;//если нет совпадений, то резльтат останется -1
	char vau = '0';
	input.get(vau);
	input.get(vau);
	std::vector<int> ans;//ответ
	int i = 0;
	while (vau != '\n' && !input.eof()) {//пока не конец строки/файла
		output << "Changes when i = " << i << " Start value k = " << k << std::endl;
		while (k > 0 && vau != P[k]) {//пока не совпадут символы
			k = n[k - 1];//сдвигаем
			output << " k = " << k << std::endl;
		}
		if (vau == P[k]) {//если совпали
			k += 1;//увеличиваем значение
			output << " k = " << k << std::endl;
		}
		if (k == P.size()) {//если длина совпадений=длине строки
			result = i - P.size() + 1;//значит ответ получен
			ans.push_back(result);
			output << "-----------------------------------------------------" << std::endl;
			output << "Result found with i = " << i << " k = " << k 
				<< " Index = " << result << std::endl;
			output << "-----------------------------------------------------" << std::endl;
		}
		i += 1;
		input.get(vau);
	}

	output << std::endl << "Result: ";
	if (!ans.empty())
		for (unsigned long int i = 0; i < ans.size() - 1; ++i) {
			output << ans[i] << ",";
		}
	output << result;
}


int main() {
	std::cout << "How do you want to input?" << std::endl << std::endl
		<< "Press 1 to input from console." << std::endl//выбор как считать
		<< "Press 2 to input from file." << std::endl;
	int menu = 0;
	while (menu != 1 && menu != 2) {//пока не введется нужная цифра
		std::cin >> menu;
		if (menu == 1) {//считывание с консоли
			int menu = 0;
			std::cout << std::endl << "How do you want to output?" << std::endl << std::endl
				<< "Press 1 to output by console." << std::endl//выбор как вывести
				<< "Press 2 to output into file." << std::endl;
			while (menu != 1 && menu != 2) {//пока не введется нужная цифра
				std::cin >> menu;
				if (menu == 1) {//вывод на консоль
					KMP(std::cin, std::cout);
				}
				else if (menu == 2) {//вывод в файл
					std::ofstream fileout;
					fileout.open(output_way);
					if (!fileout.is_open()) {
						std::cout << "Can't open file!\n";
					}
					KMP(std::cin, fileout);
				}
				else {//если неверно введена цифра, выводится сообщение
					std::cout << std::endl << "Wrong choice! Try again!" << std::endl;
				}//а цикл продолжается
			}
		}
		else if (menu == 2) {//считывание из файла
			std::ifstream filein;
			filein.open(input_way);
			if (!filein.is_open()) {
				std::cout << "Can't open file!" << std::endl;
				return 0;
			}
			int menu = 0;
			std::cout << std::endl << "How do you want to output?" << std::endl << std::endl
				<< "Press 1 to output by console." << std::endl//выбор как вывести
				<< "Press 2 to output into file." << std::endl;
			while (menu != 1 && menu != 2) {//пока не введется нужная цифра
				std::cin >> menu;
				if (menu == 1) {//вывод на консоль
					KMP(filein, std::cout);
				}
				else if (menu == 2) {//вывод в файл
					std::ofstream fileout;
					fileout.open(output_way);
					if (!fileout.is_open()) {
						std::cout << "Can't open file!\n";
					}
					KMP(filein, fileout);
				}
				else {//если неверно введена цифра, выводится сообщение
					std::cout << std::endl << "Wrong choice! Try again!" << std::endl;
				}//а цикл продолжается
			}
		}
		else {//если неверно введена цифра, выводится сообщение
			std::cout << std::endl << "Wrong choice! Try again!" << std::endl;
		}//а цикл продолжается
	}
	return 0;
}

