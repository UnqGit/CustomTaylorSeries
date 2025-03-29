#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "taylor.h"
int main(){
    start_text();
    while(true){
        std::string func, exit;
        long double coef, a, coef2;
        int degree;
        func = getInput("Enter the function that you want taylor expansion of\n", true);
        std::function<long double(long double)> toBeDoneOn;
        auto [polycoef, polycoef2] = get_coef(func);
        auto poly = [&](long double x){
            long double y = 0;
            for(int i = 0; i < polycoef.size(); i++) y = y*x + polycoef[i];
            return y;
        };
        auto poly2 = [&](long double x){
            long double y = 0;
            for(int i = 0; i < polycoef2.size(); i++) y = y*x + polycoef2[i];
            return y;
        };
        toBeDoneOn = get_func(func, poly, poly2, polycoef2);
        a = get_point("Enter the point you want the taylor series around:\n", func, poly);
        degree = get_degree("Enter the degree of the equation that you want:\n");
        std::vector<long double> fact = factor(degree, toBeDoneOn, a);
        std::vector<long double> coefficients = coeffs(toBeDoneOn, fact, a);
        printFuncs(polycoef, polycoef2, func, coefficients);
        std::cout << "\033[0m\n\n————————————————————————ENTER Q TO EXIT(or press enter to continue)————————————————————————\n";
        std::getline(std::cin, exit);
        if(exit=="Q" || exit=="q") break;
        std::cout << "\n";
    }
    std::cout << "THANKS FOR USING THE PROGRAM :D";
}
