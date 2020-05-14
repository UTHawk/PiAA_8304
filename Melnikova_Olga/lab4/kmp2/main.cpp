#include <iostream>
#include <string>
#include <vector>
#include <fstream>
//Префикс-функция от строки s равна массиву π,
//где π[i] обозначает длину максимального префикса строки, совпадающего с её суффиксом.
void prefixFunction (std::vector<int>& vectorPi, const std::string& str){
    vectorPi[0] = 0;
    //Пока не равны, берем ранее рассчитаное значение (начиная с максимально возможных),
    //если оказываются равны, то прибавляем к текущему и заполняем в массив.
    for (int i=1; i<str.length(); i++) {
        int curPrefixLenght = vectorPi[i-1];
        while ((str[i] != str[curPrefixLenght]) && (curPrefixLenght > 0)) curPrefixLenght = vectorPi[curPrefixLenght-1];
        if (str[i] == str[curPrefixLenght]) curPrefixLenght++; // равны
        vectorPi[i] = curPrefixLenght;
    }
}

void KMP(std::string& B, std::string& A, std::ostream& fout){
    fout << "Начало работы алгоритма Кнута-Морриса-Пратта" << std::endl;
    std::vector<int> vectorPi(B.length());
    prefixFunction(vectorPi, B);
    //кол-во совпавших символов, оно же индекс сравниваемого символа в образце.
    //В строке сравниваемый символ будет иметь индекс i.
    int matching = 0;
    //сравниваем очередные символы текста и подстроки, если они не совпали,
    //сдвигаем образец на несколько символов и делаем еще одно сравнение
    for (int i = 0; i<A.length(); ++i){
        fout << "Элемент образца " << B[matching]
        << "; Элемент цепочки " << A[i] << std::endl;
        while ((B[matching] != A[i]) && (matching > 0)) matching = vectorPi[matching - 1]; //опять сдвиг образца, пока символы не совпадут или не дойдем до конца образца
        if (A[i] == B[matching]) matching++; //Если есть совпадение очередного символа, увеличиваем длину совпавшего фрагмента на 1
        //Если совпали длины — нашли вхождение.
        if (matching == B.length()){ fout<<"Длины совпали - индекс начала: "<< i - B.length() + 1; return; }
    }
    fout<<"Не циклический сдвиг";
}

int main(){
    char in, out;
    std::cout << "Для считывания/вывода через консоль введите - 'c', через файл - 'f' \n";
    std::cin >> in >> out;
    if((out != 'c' && out != 'f') || (in != 'c' && in != 'f')) { std::cout << "Неверный ввод\n"; return 1; }
    std::string A, B;
    if(in == 'c') std::cin >> A >> B;
    else{ std::ifstream input("input.txt"); input >> A >> B; }
    A += A;
    if(out == 'c') KMP(B, A, std::cout);
    else{ std::ofstream output("output.txt"); KMP(B, A, output); }
    return 0;
}
