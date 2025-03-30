#pragma once
#include <vector>
#include <string>
#include <functional>
#include <tuple>
void start_text();
std::string getInput(const std::string& prompt,const bool& funcornums);
std::tuple<std::vector<long double>, std::vector<long double>> get_coef(std::string& func);
std::function<long double(long double)> get_func(const std::string& func,const std::function<long double(long double)>& poly,const std::function<long double(long double)>& poly2,const std::vector<long double>& polycoef2);
long double get_point(const std::string& prompt,const std::string& func,const std::function<long double(long double)>& poly);
int get_degree(const std::string& prompt);
std::vector<long double> coeffs(std::function<long double(long double)>& toBeDoneOn, std::vector<long double>& fact, long double& a);
std::vector<long double> factor(const int& degree, const std::function<long double(long double)>& toBeDoneOn, const long double& a);
void printFuncs(std::vector<long double> polycoef,std::vector<long double> polycoef2, std::string func, std::vector<long double> coefficients);
