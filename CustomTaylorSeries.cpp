#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <map>
#include <algorithm>
#include <sstream>
#include <functional>
#include <cmath>

void input_corrector(std::string& input){
    std::replace(input.begin(), input.end(), ',', ' ');
    input.erase(0, input.find_first_not_of(" 0\n\t"));
    input.erase(input.find_last_not_of(" \n\t")+1);
    if(input.empty()) input = "0";
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

long double derivatives(const std::function<long double(long double)>& toBeDoneOn, long double a, int i){
    if(i == 0) return toBeDoneOn(a);
    long double h = (i < 4)?1.0/(1<<10):(i < 7)?1.0/(1<<8):1.0/(1 << 5);
    long double b = derivatives(toBeDoneOn, a, i - 1);
    long double c = derivatives(toBeDoneOn, a - h, i - 1);
    return (b-c)/h;
}

std::vector<long double> factor(const int& degree, const std::function<long double(long double)>& toBeDoneOn, const long double& a){
    std::vector<long double> fact(degree + 1);
    for(int i = 0; i < fact.size(); i++){
        fact[i] = derivatives(toBeDoneOn, a, i);
    }
    for(int i = 2; i < fact.size(); i++){
        std::transform(fact.begin() + i, fact.end(), fact.begin() + i, [&](long double& c){return c/i;});
    }
    return fact;
}

long double nCr(int a, int b){
    if(b == 0 || a == b) return 1;
    if(b == 1 || a - b == 1) return a;
    if(b > a/2) b = a - b;
    long double result = 1.0;
    for(int i = 1; i <= b; i++){
        result*=(a - i + 1)/i;
    }
    return result;
}

std::vector<long double> coeffs(std::function<long double(long double)>& toBeDoneOn, std::vector<long double>& fact, long double& a){
    std::vector<long double> coefficients(fact.size());
    for(int i = 0; i < coefficients.size(); i++){
        long double b = 1.0;
        for(int j = i; j >= 0; j--){
            coefficients[j] += nCr(i, j)*b*fact[i];
            b*=-a;
        }
    }
    return coefficients;
}

std::string superscript(int i){
    if(i==1) return "";
    std::string result;
    std::string super[] = {"⁰", "¹", "²", "³", "⁴", "⁵", "⁶", "⁷", "⁸", "⁹"};
    while(i>0){
        result=super[i%10]+result;
        i/=10;
    }
    return result;
}

void print(std::vector<long double>& arr){
    std::string result;
    if(arr[0]==0){}
    else{
        result = std::to_string(arr[0]);
        if(arr[0]!=std::floor(arr[0]))std::cout.precision(7);
        else std::cout.precision(0);
        std::cout << std::fixed << arr[0];
    }
    for(int i = 1; i < arr.size(); i++){
        if(arr[i]==0) continue;
        else{
            std::cout << (result.empty()?(arr[i]>0?"":"-"):(arr[i]>0?" + ":" - "));
            if(std::abs(arr[i])!=1){
                if(arr[i]!=std::floor(arr[i]))std::cout.precision(7);
                else std::cout.precision(0);
                std::cout << std::fixed << std::abs(arr[i]);
            }
            std::cout << "x" << superscript(i);
            result = std::to_string(arr[i]);
        }
    }
    if(result.empty()) std::cout << 0;
}

void hornerPrint(std::vector<long double>& arr){
    while(!arr.empty()&&arr.back()==0) arr.pop_back();
    if(arr.empty()) arr={0};
    if(arr[0]!=0)std::cout << arr[0] << " + ";
    else if(arr.size()==1) std::cout << arr[0];
    for(int i = 1; i < arr.size(); i++){
        std::cout << "x*( " << arr[i] << (i!=arr.size()-1?" + ":"");
    }
    std::cout << std::string(arr.size()-1, ')');
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
        {"cos", static_cast<long double(*)(long double)>(std::cos)},
        {"sqrt", static_cast<long double(*)(long double)>(std::sqrt)}
    };
    std::cout << "In the program you have to enter\nthe coefficients from the highest coefficient\nfor eg, 2 0 0 would be 2x^2 and 1 0 would be x\n";
    std::cout << "The available functions will be\nsqrt, exp, log, tan, sin, cos, atan, acos, asin\n\n";
    while(true){
        std::string input, func;
        std::cout << "Enter the coefficients of your polynomial:\n";
        std::getline(std::cin, input);
        input_corrector(input);
        if(invalid_input(input)){
            std::cin.clear();
            std::cout << "Invalid numbers.\n";
            continue;
        }
        std::vector<long double> polycoef;
        std::stringstream ss(input);
        long double coef, a;
        int degree;
        while(ss >> coef) polycoef.push_back(coef);
        std::cout << "Enter the function that you want taylor expansion of\n";
        std::getline(std::cin, func);
        input_corrector(func);
        while(function_checker(func)){
            std::cin.clear();
            std::cout << "Invalid function.\nEnter again:\n";
            std::getline(std::cin, func);
            input_corrector(func);
        }
        std::cin.clear();
        std::function<long double(long double)> toBeDoneOn;
        toBeDoneOn = functions[func];
        auto poly = [&](long double x){
            long double y = 0;
            for(int i = 0; i < polycoef.size(); i++) y = y*x + polycoef[i];
            return y;
        };
        toBeDoneOn = [f = toBeDoneOn, g = poly](long double x){return f(g(x));};
        std::cout << "Enter the point you want the taylor series around:\n";
        while(!(std::cin >> a) || ((func=="sqrt")&&(poly(a)<0)) || ((func=="log")&&(poly(a)<=0)) || ((func=="acos"||func=="asin")&&(std::abs(poly(a))>1))){
            if(std::cin.fail()) std::cout << "Invalid number";
            else std::cout << "Number is out of domain of function";
            std::cout << " please enter again:\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cout << "Enter the degree of the equation that you want:\n";
        while(!(std::cin >> degree) || degree < 0 ){
            std::cout << "Invalid number please enter again:\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::vector<long double> fact = factor(degree, toBeDoneOn, a);
        std::vector<long double> coefficients = coeffs(toBeDoneOn, fact, a);
        for(int i = 0; i < polycoef.size()/2; i++)
            std::swap(polycoef[i], polycoef[polycoef.size()-i-1]);
        std::cout << "y = \033[92m" << func << "(";
        print(polycoef);
        std::cout << ")\033[0m ≈ f(x) = \033[93m";
        print(coefficients);
        std::cout << "\033[0m\nHorner's method: \033[93m";
        hornerPrint(coefficients);
        std::cout << "\033[0m\n————————————————————————————————————————————————\n\n";
    }
}