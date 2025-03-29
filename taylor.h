#pragma once
#include <vector>
#include <string>
#include <functional>
#include <tuple>
void start_text();
void input_corrector(std::string& input);
bool invalid_input(std::string input);
bool cond(const std::string& part,const std::string& str);
bool function_checker(std::string& func);
long double derivatives(const std::function<long double(long double)>& toBeDoneOn, long double a, int i);
std::vector<long double> factor(const int& degree, const std::function<long double(long double)>& toBeDoneOn, const long double& a);
long double nCr(int a, int b);
std::vector<long double> coeffs(std::function<long double(long double)>& toBeDoneOn, std::vector<long double>& fact, long double& a);
std::string superscript(int i);
void print(std::vector<long double>& arr, std::string func="");
void hornerPrint(std::vector<long double>& arr);
std::string getInput(const std::string& prompt,const bool& funcornums);
std::tuple<std::vector<long double>, std::vector<long double>> get_coef(std::string& func);
std::function<long double(long double)> get_func(const std::string& func,const std::function<long double(long double)>& poly,const std::function<long double(long double)>& poly2,const std::vector<long double>& polycoef2);
long double get_point(const std::string& prompt,const std::string& func,const std::function<long double(long double)>& poly);
int get_degree(const std::string& prompt);
void printFuncs(std::vector<long double> polycoef,std::vector<long double> polycoef2, std::string func, std::vector<long double> coefficients);
