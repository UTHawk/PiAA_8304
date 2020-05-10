#include <iostream>
#include <vector>
#include <string>
#include <fstream>
void prefixFunction (std::vector<int>& vectorPi, const std::string& str){
    vectorPi[0] = 0;
    for (int i=1; i<str.length(); i++) {
        int curPrefixLenght = vectorPi[i-1];
        while ((str[i] != str[curPrefixLenght]) && (curPrefixLenght > 0)) curPrefixLenght = vectorPi[curPrefixLenght-1];
        if (str[i] == str[curPrefixLenght]) curPrefixLenght++;
        vectorPi[i] = curPrefixLenght;
    }
}
void KMP(std::istream& input, std::ostream& output){
    output << "Начало работы алгоритма Кнута-Морриса-Пратта" << std::endl;
    bool is_result = false;
    std::string str;
    input >> str;
    char tmp = '1';
    std::vector<int> vectorPi(str.length());
    prefixFunction(vectorPi, str);
    size_t j = 0;
    size_t counter = 0;
    input.get();
    input.get(tmp);
    while(input.peek() != EOF && input.peek() != '\n'){
        while (str[j] == tmp) { output<<"Совпадение!!! Элемент шаблона: "<<str[j]<<"; Элемент текста: " << tmp<< std::endl; input.get(tmp); j++; counter++; }
        if (j == str.length()){
            output <<"\tВхождение найдено, индекс начала: " << counter - j << std::endl;
            j = vectorPi[j-1];
            is_result = true;
        }else{
            if(j != 0) j = vectorPi[j-1];
            else input.get(tmp);
        }
    }
    if (!is_result) output << "Вхождений не найдено" << std::endl;
    output << "Конец работы алгоритма Кнута-Морриса-Пратта" << std::endl;
}
int main() {
    char in, out;
    std::cout << "Для считывания/вывода через консоль введите - 'c', через файл - 'f' \n";
    std::cin >> in >> out;
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");
    if((out != 'c' && out != 'f') || (in != 'c' && in != 'f')) { std::cout << "Неверный ввод\n"; return 1; }
    if((in == 'c') && (out == 'c')) KMP(std::cin, std::cout);
    if((in == 'f') && (out == 'f')) KMP(input, output);
    if((in == 'f') && (out == 'c')) KMP(input, std::cout);
    if((in == 'c') && (out == 'f')) KMP(std::cin, output);
    return 0;

}
