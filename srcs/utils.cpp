#include "../headers/utils.hpp"

std::map<int, std::vector<std::string> > split(const std::string &str, int index)
{
	std::map<int, std::vector<std::string> > result;
	std::string token;

	for (size_t i = 0; i < str.size(); ++i)
	{
		if ((str[i] == '+' || str[i] == '-') && !token.empty())
		{
			result[index].push_back(token);
			token.clear();
		}
		token += str[i];
	}
	if (!token.empty())
		result[index].push_back(token);
	return result;
}

std::string formatCoefficient(double value)
{
	std::string result = std::to_string(value);
	size_t dotPos = result.find('.');

	if (dotPos == std::string::npos)
		return result;
	while (!result.empty() && result[result.size() - 1] == '0')
		result.erase(result.size() - 1);
	if (!result.empty() && result[result.size() - 1] == '.')
		result.erase(result.size() - 1);
	if (result == "-0")
		return "0";
	return result;
}

bool checkCoefficients(const std::string &term)
{
	for (size_t i = 0; i < term.size(); i++)
	{
		if (i > 0 && i + 1 < term.size())
		{
			if (term[i - 1] == '^' && (term[i] == '+' || term[i] == '-') && (term[i + 1] != ' ' && term[i - 1] != ' '))
				throw std::invalid_argument("Not a polynomial equation, I can't solve.");
			if (term[i - 1] == '^' && !isdigit(term[i]))
				throw std::invalid_argument("Not a polynomial equation, I can't solve.");
		}
	}
	return true;
}

double absoluteValue(double value)
{
	return value < 0 ? -value : value;
}

double squareRoot(double value)
{
	double x = 1;
	double eps = 1e-10;
	while (absoluteValue(x * x - value) > eps)
	{
		x = (x + value / x) / 2;
	}
	return x;
}
