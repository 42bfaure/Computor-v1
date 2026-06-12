#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>

std::map<int, std::vector<std::string> > split(const std::string &str, int index);
std::string formatCoefficient(double value);
double squareRoot(double value);
bool checkCoefficients(const std::string &term);
double absoluteValue(double value);

#endif