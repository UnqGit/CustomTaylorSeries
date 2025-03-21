#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <map>
#include <algorithm>
#include <sstream>
#include <functional>
#include <cmath>

std::vector<long double> polycoef;

long double poly(long double x){
    long double y = 0;
    for(int i = 0; i < polycoef.size(); i++) y = y*x + polycoef[i];
    return y;
}

void input_corrector(std::string& input){
    std::replace(input.begin(), input.end(), ',', ' ');
    for(char& ch : input) ch = std::tolower(ch);
}

bool invalid_input(std::string input){
    if(input.empty()) return 1;
    for(int i = 0; i < input.size(); i++){
        char j = input[i];
        if(!(std::isdigit(j))){
            if(j=='-'||j=='+'){
                if(i + 1 < input.size()){
                    if(std::isdigit(input[i + 1])) continue;
                    else if(input[i + 1] == '.'){
                        if(i + 2 < input.size()){
                            if(std::isdigit(input[i + 2])) continue;
                            return 1;
                        }
                    }
                }
                else return 1;
            }
            else if(j=='.'){
                if(i > 0){
                    char k = input[i-1];
                    if(std::isdigit(k) || k == '-' || k == '+') continue;
                }
                if(i + 1 < input.size()){
                    char k = input[i + 1];
                    if(std::isdigit(k)) continue;
                }
                return 1;
            }
            else if(j==' ' || j=='\n' || j=='\t') continue;
            return 1;
        }
    }
    return 0;
}

bool function_checker(std::string& func){
    if(func.find("ex") != std::string::npos){func = "exp"; return 0;}
    if(func.find("lo") != std::string::npos){func = "log"; return 0;}
    if(func.find("at") != std::string::npos){func = "atan"; return 0;}
    if(func.find("as") != std::string::npos){func = "asin"; return 0;}
    if(func.find("ac") != std::string::npos){func = "acos"; return 0;}
    if(func.find("sq") != std::string::npos){func = "sqrt"; return 0;}
    if(func.find("ta") != std::string::npos){func = "tan"; return 0;}
    if(func.find("si") != std::string::npos){func = "sin"; return 0;}
    if(func.find("co") != std::string::npos){func = "cos"; return 0;}
    return 1;
}

int main(){
    std::map<std::string, std::function<long double(long double)>> functions{
        {"exp", static_cast<long double(*)(long double)>(std::exp)},
        {"log", static_cast<long double(*)(long double)>(std::log)},
        {"atan", static_cast<long double(*)(long double)>(std::atan)},
        {"asin", static_cast<long double(*)(long double)>(std::asin)},
        {"acos", static_cast<long double(*)(long double)>(std::acos)},
        {"tan", static_cast<long double(*)(long double)>(std::tan)},
        {"sin", static_cast<long double(*)(long double)>(std::sin)},
        {"sqrt", static_cast<long double(*)(long double)>(std::sqrt)}
    };
    while(true){
        std::string input, func;
        std::cout << "Enter the coefficients of your polynomial:\nYou have to enter it from the highest coefficient\nfor eg, 1 0 0 would be x^2 and 1 would be 1\n";
        std::getline(std::cin, input);
        input_corrector(input);
        if(invalid_input(input)){
            std::cout << "Invalid numbers.\n";
            continue;
        }
        polycoef.clear();
        std::stringstream ss(input);
        long double coef, a;
        int degree;
        while(ss >> coef) polycoef.push_back(coef);
        std::cout << "Enter the function that you want the taylor expansion of\nfor eg,\n:exp\n:log\n:sqrt\netc.\n";
        std::getline(std::cin, func);
        input_corrector(func);
        while(function_checker(func)){
            std::cout << "Invalid function.\nEnter again:\n";
            std::getline(std::cin, func);
            input_corrector(func);
        }
        std::function<long double(long double)> toBeDoneOn;
        toBeDoneOn = functions[func];
        toBeDoneOn = [f = toBeDoneOn, g = poly](long double x){return f(g(x));};
        std::cout << "Enter the point you want the taylor series around:\n";
        while(!(std::cin >> a)){
            std::cout << "Invalid number please enter again:\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cout << "Enter the degree of the equation that you want:\n";
        while(!(std::cin >> degree) || degree < 0 ){
            std::cout << "Invalid number please enter again:\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
    }
}
