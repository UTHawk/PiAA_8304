#include <iostream>
#include <ctime>
#include "table.h"

using namespace std;

int main(){
    int tableWidth;
    cout<<"Введите N: "; cin >> tableWidth;
    table table(tableWidth);
    unsigned int start_time = clock();
    int minimumCountOfSquares = table.getMinimumCountOfSquares();
    unsigned int end_time = clock();
    unsigned int time = end_time-start_time;
    cout<< "Минимальное количество квадратов: " << minimumCountOfSquares << endl;
    for (int i = 0; i < minimumCountOfSquares; i++)
        cout << table.minSquares[i]->point->x + 1 << " " << table.minSquares[i]->point->y + 1 << " " << table.minSquares[i]->width << endl;
    cout << "Поиск занял "<< ((float)time) / CLOCKS_PER_SEC<<" cекунд"<< endl;
    return 0;
}
