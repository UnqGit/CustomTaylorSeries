#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <sstream>
#include <functional>
#include <cmath>

void start_text(){
    std::cout << "In the program you have to enter\nthe coefficients from the highest coefficient\nfor eg, 2 0 0 would be 2x^2 and 1 0 would be x\n";
    std::cout << "The available functions will be\nsqrt, exp, log, tan, sin, cos, atan, acos, asin\n\n";
}

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

bool cond(const std::string& part,const std::string& str){
    return str.find(part)!=std::string::npos;
}
bool function_checker(std::string& func){
    if(cond("ex", func)){func = "exp"; return 0;}
    if(cond("lo", func)){func = "log"; return 0;}
    if(cond("at", func)){func = "atan"; return 0;}
    if(cond("as", func)){func = "asin"; return 0;}
    if(cond("ac", func)){func = "acos"; return 0;}
    if(cond("sq", func)){func = "sqrt"; return 0;}
    if(cond("nh", func)){func = "tanh"; return 0;}
    if(cond("inh", func)){func = "sinh"; return 0;}
    if(cond("sh", func)){func = "cosh"; return 0;}
    if(cond("ta", func)){func = "tan"; return 0;}
    if(cond("si", func)){func = "sin"; return 0;}
    if(cond("co", func)){func = "cos"; return 0;}
    if(cond("po", func)){func = "pow"; return 0;}
    return 1;
}

long double derivatives(const std::function<long double(long double)>& toBeDoneOn, long double a, int i){
    if(i == 0) return toBeDoneOn(a);
    long double h = (i<=4)?1.0/(1<<10):1.0/(1<<8);
    auto f = [&](long double d){
        return (derivatives(toBeDoneOn, a+h*d, i-1)-derivatives(toBeDoneOn, a-h*d, i-1))/(2*h*d);
    };
    return (4*f(0.5)-f(1))/3;
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
    if(b>a/2) b = a - b;
    long double result = 1.0;
    for(int i = 1; i <= b; i++){
        result*=(1.0 + a - i)/(0.0 + i);
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

void print(std::vector<long double>& arr, std::string func=""){
    std::string result;
    if(arr[0]==0){}
    else{
        result = std::to_string(arr[0]);
        std::cout << arr[0];
    }
    for(int i = 1; i < arr.size(); i++){
        if(arr[i]==0) continue;
        else{
            std::cout << (result.empty()?(arr[i]>0?"":"-"):(arr[i]>0?" + ":" - "));
            if(std::abs(std::abs(arr[i])-1)>1e-6){
                std::cout << std::abs(arr[i]);
            }
            std::cout << "x" << superscript(i);
            result = std::to_string(arr[i]);
        }
    }
    if(result.empty()){
        if(func=="log") std::cout << "e";
        else std::cout << 0;
    }
}

void hornerPrint(std::vector<long double>& arr){
    while(!arr.empty()&&arr.back()==0) arr.pop_back();
    if(arr.empty()) arr={0};
    bool f = false;
    if(arr[0]!=0)std::cout << arr[0] << ((arr.size()==1)?"":" + ");
    else if(arr.size()==1)std::cout << 0;
    for(int i = 1; i < arr.size(); i++){
        if(i==arr.size()-1&&arr[i]==1){std::cout << "x"; f = true;}
        else std::cout << "x*( " << arr[i] << (i!=arr.size()-1?" + ":"");
    }
    std::cout << (f?std::string(arr.size()-2, ')'):std::string(arr.size()-1, ')'));
}

std::string getInput(const std::string& prompt,const bool& funcornums){
    std::string input;
    bool invalid = false;
    std::cout << prompt;
    do{
        std::cin.clear();
        std::getline(std::cin, input);
        input_corrector(input);
        invalid = funcornums?function_checker(input):invalid_input(input);
        if(!invalid) break;
        std::cout << "Invalid " << (funcornums?"function":"numbers") << ".\nEnter again:\n";
    }while(true);
    std::cin.clear();
    return input;
}

std::tuple<std::vector<long double>, std::vector<long double>> get_coef(std::string& func){
    std::vector<long double> polycoef, polycoef2;
    std::string prompt, input;
    long double coef, coef2;
    prompt = "Enter the coefficients of";
    prompt+=(func=="pow"?" base":func=="log"?" function":"");
    prompt+=" polynomial:\n";
    input = getInput(prompt, false);
    std::stringstream ss(input);      
    while(ss >> coef) polycoef.push_back(coef);
    if(func=="pow"||func=="log"){
        prompt = "Enter the coefficients of ";
        prompt+=(func=="pow"?"exponent":"base(leave empty or enter 0 to get natural log)");
        prompt+=" polynomial:\n";
        input = getInput(prompt, false);
        std::stringstream sd(input);
        while(sd >> coef2) polycoef2.push_back(coef2);
    }
    return std::tuple<std::vector<long double>, std::vector<long double>>(polycoef, polycoef2);
}

std::function<long double(long double)> get_func(const std::string& func,const std::function<long double(long double)>& poly,const std::function<long double(long double)>& poly2,const std::vector<long double>& polycoef2){
    std::function<long double(long double)> toBeDoneOn;
    if(func=="sqrt") toBeDoneOn = [f = static_cast<long double(*)(long double)>(std::sqrt), g = poly](long double x){return f(g(x));};
    else if(func=="exp") toBeDoneOn = [f = static_cast<long double(*)(long double)>(std::exp), g = poly](long double x){return f(g(x));};
    else if(func=="sin") toBeDoneOn = [f = static_cast<long double(*)(long double)>(std::sin), g = poly](long double x){return f(g(x));};
    else if(func=="cos") toBeDoneOn = [f = static_cast<long double(*)(long double)>(std::cos), g = poly](long double x){return f(g(x));};
    else if(func=="tan") toBeDoneOn = [f = static_cast<long double(*)(long double)>(std::tan), g = poly](long double x){return f(g(x));};
    else if(func=="asin") toBeDoneOn = [f = static_cast<long double(*)(long double)>(std::asin), g = poly](long double x){return f(g(x));};
    else if(func=="acos") toBeDoneOn = [f = static_cast<long double(*)(long double)>(std::acos), g = poly](long double x){return f(g(x));};
    else if(func=="atan") toBeDoneOn = [f = static_cast<long double(*)(long double)>(std::atan), g = poly](long double x){return f(g(x));};
    else if(func=="sinh") toBeDoneOn = [f = static_cast<long double(*)(long double)>(std::sinh), g = poly](long double x){return f(g(x));};
    else if(func=="cosh") toBeDoneOn = [f = static_cast<long double(*)(long double)>(std::cosh), g = poly](long double x){return f(g(x));};
    else if(func=="tanh") toBeDoneOn = [f = static_cast<long double(*)(long double)>(std::tanh), g = poly](long double x){return f(g(x));};
    else if(func=="pow") toBeDoneOn = [f = static_cast<long double(*)(long double, long double)>(std::pow), g = poly, h = poly2](long double x){return f(g(x),h(x));};
    else if(func=="log") toBeDoneOn = [f = static_cast<long double(*)(long double)>(std::log), g = poly, h = poly2, o = polycoef2](long double x){return (o.size()==1&&o[0]==0)?f(g(x)):f(g(x))/f(h(x));};
    return toBeDoneOn;
}

long double get_point(const std::string& prompt,const std::string& func,const std::function<long double(long double)>& poly){
    std::cout << prompt;
    long double a;
    while(!(std::cin >> a) || ((func=="sqrt"||func=="pow")&&(poly(a)<0)) || ((func=="log")&&(poly(a)<=0)) || ((func=="acos"||func=="asin")&&(std::abs(poly(a))>1))){
        if(std::cin.fail()) std::cout << "Invalid number";
        else std::cout << "Number is out of domain of function";
        std::cout << " please enter again:\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return a;
}

int get_degree(const std::string& prompt){
    int degree;
    std::cout << prompt;
    while(!(std::cin >> degree) || degree < 0 ){
        std::cout << "Invalid number please enter again:\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return degree;
}

void printFuncs(std::vector<long double> polycoef,std::vector<long double> polycoef2, std::string func, std::vector<long double> coefficients){
    for(int i = 0; i < polycoef.size()/2; i++)
        std::swap(polycoef[i], polycoef[polycoef.size()-i-1]);
    for(int i = 0; i < polycoef2.size()/2; i++)
        std::swap(polycoef2[i], polycoef2[polycoef2.size()-i-1]);
    std::cout << "y = \033[92m" << func << "(";
    if(func=="log"){
        print(polycoef2, func);
        std::cout << ", ";
        print(polycoef);
    }
    else{
        print(polycoef);
        if(func=="pow"){std::cout << ", "; print(polycoef2);}
    }
    std::cout << ")\033[0m ≈ f(x) = \033[93m";
    print(coefficients);
    std::cout << "\033[0m\nHorner's method: \033[93m";
    hornerPrint(coefficients);
}
