//#include <iostream>
//#include <ctime>
//
//#include "backtrack.hpp"
//
//
//int main() {
//    std::cout << "Enter square size between 2-40:  ";
//    int n = 0;
//    std::cin >> n;
//
//    if (std::cin.bad()) {
//        std::cout << "\nInvalid input\n";
//        exit(1);
//    }
//    if (n < 2 || n > 50) {
//        std::cout << "\nInvalid input\n";
//        exit(1);
//    }
//
//    Track track(n);
//
//    if (n % 2 == 0 || n % 3 == 0 || n % 5 == 0) {
//        track.optimalSolution();
//    }
//    else {
//         auto startTime = clock();
//         track.startBacktracking();
//         auto endTime = clock();
////        std::cout << endTime - startTime << std::endl;
//         std::cout << "\nTime: " << (double)(endTime - startTime) / CLOCKS_PER_SEC;
//    }
//    std::cout << std::endl;
//    return 0;
//}
//


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <iostream>

double delta, c, d;
double BISECT(double, double, double, int&);
double F(double);
double Round(double, double);
int  main() {
while (true) {
int k;
long int s;
float a1, b1, c1, d1, eps1, delta1;
double a, b, eps, x;
printf("vvedite eps:");
scanf("%f", &eps1);
eps = eps1;
printf("vvedite a:");
scanf("%f", &a1);
a = a1;
printf("vvedite b:");
scanf("%f", &b1);
b = b1;
printf("vvedite delta:");
scanf("%f", &delta1);
delta = delta1;
x = BISECT(a, b, eps, k);
printf("x=%f k=%d\n", x, k);
std::cout << "V delta: " << (1 / fabs(2*(((pow(x,3)-x)*sin(pow(x,2))-1))/(pow(x,2)-1)))

<< "\n";

std::cout << "V delta max: " << eps / delta << "\n";
if ((eps / delta) >= (1 / fabs(2*(((pow(x,3)-x)*sin(pow(x,2))-1))/(pow(x,2)-1)))) {
printf("zadacha horosho obyslovlena \n\n");
}
else {
printf("zadacha ploho obyslovlena\n\n");
}
}
}
    
double F(double x) {
extern double c, d, delta;
double s;
s = log((1+x)/(1-x)) - cos(pow(x,2));
s = Round(s, delta);
return(s);
}
double BISECT(double Left, double Right, double Eps, int &N) {
double E = fabs(Eps)*2.0;
double FLeft = F(Left);
double FRight = F(Right);
double X = (Left + Right) / 2.0;
double Y;
if (FLeft*FRight>0.0) {
puts("neverno zadan interval\n"); exit(1);

}
if (Eps <= 0.0) {
puts("neverno zadana tochnost\n");
exit(1);
}
N = 0;
if (FLeft == 0.0)
return Left;
if (FRight == 0.0)
return Right;
while ((Right - Left) >= E) {
X = 0.5*(Right + Left);
Y = F(X);
if (Y == 0.0)
return (X);
if (Y*FLeft < 0.0)
Right = X;
else
Left = X; FLeft = Y;
N++;
};
return(X);
}
    
double Round(double X, double Delta) { if
(Delta <= 1E-9) {
puts("Неверно задана точность округления\n");
exit(1);
}
if (X>0.0)
return (Delta*(long((X / Delta) + 0.5)));
else
return (Delta*(long((X / Delta) + 0.5)));

}
