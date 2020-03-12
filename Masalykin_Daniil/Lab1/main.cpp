#include <cmath>
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

class Point
{
public:
    int x;
    int y;
    int length;
    Point()
    {
        x = 0;
        y = 0;
        length = 0;
    }
};

class Matrix
{
private:
    int length;
    int** matrix;
    int** best_matrix;
    int64_t count;
    int best_count;
    vector <Point> current_matrix;
    vector <Point> best;
    unsigned int count_of_operations;
public:
    Matrix(int length, int count) :length(length), count(0), best_count(length*length),count_of_operations(0)
    {
        matrix = new int* [length];
        best_matrix = new int* [length];
        for (int i = 0; i < length; i++)
        {
            matrix[i] = new int[length];
            best_matrix[i] = new int[length];
            for (int j = 0; j < length; j++)
            {
                matrix[i][j] = 0;
                best_matrix[i][j] = 0;
            }
        }
    }

    void insert_square(Point point)
    {
        for (int i = point.y; i < point.y + point.length; i++)
        {
            for (int j = point.x; j < point.x + point.length; j++)
            {
                matrix[i][j] = count + 1;
            }
        }
        //std::cout<<count+1<<"\n";
        count++;
    }

    void remove_square(Point point)
    {
        for (int i = point.y; i < point.y + point.length; i++)
        {
            for (int j = point.x; j < point.x + point.length; j++)
            {
                matrix[i][j] = 0;
            }
        }
        count--;
    }

    void print_table()
    {
        std::cout << "current table" << "\n";
        for (int i = 0; i < length; i++)
        {
            for (int j = 0; j < length; j++)
            {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    void print_best_table()
    {
        std::cout << "current best table!" << "\n";
        for (int i = 0; i < length; i++)
        {
            for (int j = 0; j < length; j++)
            {
                std::cout << best_matrix[i][j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    void copy_square()
    {
        for (int i = 0; i < length; i++)
        {
            for (int j = 0; j < length; j++)
            {
                best_matrix[i][j] = matrix[i][j];
            }
        }
    }

    bool is_filled()
    {
        for (int i = length - 1; i >= 0; --i)
            for (int j = length - 1; j >= 0; --j)
                if (matrix[i][j] == 0)
                    return false;
        return true;
    }

    bool is_possible(int x, int y, int m)
    {
        if (x >= length || y >= length)
            return false;
        if (x + m > length || y + m > length)
        {
            return false;
        }
        for (int i = y; i < y + m; i++)
        {
            for (int j = x; j < x + m; j++)
            {
                if (matrix[i][j] != 0)
                {
                    return false;
                }
            }
        }
        return true;
    }

    void backtracking()
    {
        Point point;
        point.x = 0;
        point.y = 0;
        point.length = ceil(length / 2);
        current_matrix.push_back(point);
        insert_square(point);

        point.length = length / 2;
        point.x = ceil(length / 2);
        current_matrix.push_back(point);
        insert_square(point);
        point.x = 0;
        point.y = ceil(length / 2);
        current_matrix.push_back(point);
        insert_square(point);
        print_table();
        do
        {
            while (count < best_count && !is_filled() )
            {
                for (int y = 0; y < length; y++)
                {
                    for (int x = 0; x < length; x++)
                    {
                        if (matrix[y][x] == 0)
                        {
                            for (int m = length - 1; m > 0; m--)
                            {
                                count_of_operations++;
                                if (is_possible(x, y, m))
                                {
                                    point.x = x;
                                    point.y = y;
                                    point.length = m;
                                    break;
                                }

                            }
                            insert_square(point);
                            current_matrix.push_back(point);
                            //print_table();
                        }
                    }
                }
            }

            if (best_count > count || best_count == 4)
            {
                best_count = count;
                copy_square();
                print_best_table();//вывод промежуточного лучшего результата
                best = current_matrix;
            }
            while (!current_matrix.empty() && current_matrix[current_matrix.size() - 1].length == 1)//удаление квадратов со стороной 1
            {
                remove_square(current_matrix[current_matrix.size() - 1]);
                current_matrix.pop_back();
            }
            if (!(current_matrix.empty()))//уменьшение стороны квадрата на 1
            {
                point = current_matrix[current_matrix.size() - 1];
                current_matrix.pop_back();
                remove_square(point);
                point.length -= 1;
                insert_square(point);
                current_matrix.push_back(point);
            }


        } while (count < best_count * 3 && !(current_matrix.empty()));
    }

    void print_result(int multiply)//вывод результата работы программы
    {
        Point point;
        std::cout<< "Minimum number of squares: " << best_count << "\n";
        std::cout<< "Count of operations: " << count_of_operations << "\n";
        while (!(best.empty()))
        {
            point = best[best.size() - 1];
            best.pop_back();
            std::cout << point.x * multiply + 1<< " " << point.y * multiply + 1 << " " << point.length*multiply;
            if(!(best.empty()))
                std::cout << std::endl;
        }
    }
};


int min_primal_size(int size)
{
    int primal_size = size;
    for (int i = 2; i <= sqrt(size); i++)
    {
        if (size % i == 0)
            return i;
    }
    return  primal_size;
}

int main()
{
    int square_side;
    std::cout << "Please, input length of sub_matrix's side\n";
    std::cin >> square_side;
    int primal_size = min_primal_size(square_side);
    Matrix sub_matrix(primal_size, 0);
    std::cout<<primal_size<<std::endl;
    int multiply = square_side/primal_size;
    srand(time(0));
    sub_matrix.backtracking();
    sub_matrix.print_result(multiply);
    cout << std::endl <<"runtime = " << clock() / 1000.0 << endl; // время работы программы
}
