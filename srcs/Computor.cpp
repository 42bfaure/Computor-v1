#include "../headers/Computor.hpp"
#include <cmath>

std::string Computor::_formatCoefficient(double value) const
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

Computor::Computor(std::string equation) : _equation(equation)
{
	std::cout << "Computor initialized with equation: " << _equation << std::endl;
	this->parseEquation();
	// this->reducedForm();
	// this->degree();
	// this->solve();
}

Computor::~Computor()
{
}

std::map<int, std::vector<std::string> > Computor::_split(const std::string &str, int index)
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

void Computor::_splitTerms(std::string equation, int index)
{
	std::cout << "Splitting terms for equation " << index << ": " << equation << std::endl;
	std::map<int, std::vector<std::string> > splitResult = _split(equation, index);
	this->_terms[index] = splitResult[index];
	for (size_t i = 0; i < _terms.at(index).size(); i++)
	{
		std::cout << "Term " << index << " " << i << ": " << _terms[index].at(i) << std::endl;
	}
	return ;
}

void Computor::parseEquation()
{
	std::string leftSide;
	std::string rightSide;
	std::cout << "Parsing equation: " << _equation << std::endl;
	std::string cleaned;
	for (size_t i = 0; i < _equation.size(); ++i)
	{
		if (_equation[i] != ' ' && _equation[i] != '*')
		cleaned += _equation[i];
	}
	std::cout << "Cleaned equation: " << cleaned << std::endl;
	_equation = cleaned;
	for (size_t i = 0; i < _equation.length(); i++)
	{
		if (_equation[i] == '=')
		{
			leftSide = _equation.substr(0, i);
			rightSide = _equation.substr(i + 1);
			std::cout << "Left side of equation: " << leftSide << std::endl;
			std::cout << "Right side of equation: " << rightSide << std::endl;
			break;
		}
	}
	this->_splitTerms(leftSide, LEFT_SIDE);
	this->_splitTerms(rightSide, RIGHT_SIDE);
	this->_parseCoefficients();
	this->reducedForm();
	this->discriminant();
	this->solve();
	return ;
}



void Computor::_parseCoefficients()
{
	std::string exp = "";
	_coefficientOrder.clear();
	
	for (size_t i = 0; i < _terms.at(LEFT_SIDE).size(); i++)
	{
		const std::string &term = _terms.at(LEFT_SIDE).at(i);
		size_t pos = term.find("X^");
		if (pos == std::string::npos)
		{
			_terms.at(RIGHT_SIDE).at(i) = _terms.at(RIGHT_SIDE).at(i) + "X^0";
			std::cout << "Term normalized: " << term << std::endl;
			i--;
			continue;
		}
		exp = term.substr(pos, pos + 2);
		std::cout << "exp: " << exp << std::endl;
		if (std::find(_coefficientOrder.begin(), _coefficientOrder.end(), exp) == _coefficientOrder.end())
			_coefficientOrder.push_back(exp);
		_coefficients[exp] += std::stod(term.substr(0, pos));
	}
	std::cout << "--------------------------------" << std::endl;
	for (size_t i = 0; i < _terms.at(RIGHT_SIDE).size(); i++)
	{
		const std::string &term = _terms.at(RIGHT_SIDE).at(i);
		size_t pos = term.find("X^");
		if (pos == std::string::npos)
		{
			_terms.at(RIGHT_SIDE).at(i) = _terms.at(RIGHT_SIDE).at(i) + "X^0";
			std::cout << "Term normalized: " << term << std::endl;
			i--;
			continue;
		}
		exp = term.substr(pos, pos + 2);
		std::cout << "exp: " << exp << std::endl;
		if (std::find(_coefficientOrder.begin(), _coefficientOrder.end(), exp) == _coefficientOrder.end())
			_coefficientOrder.push_back(exp);
		_coefficients[exp] -= std::stod(term.substr(0, pos));
	}
	_a = _coefficients["X^2"];
	_b = _coefficients["X^1"];
	_c = _coefficients["X^0"];
	std::cout << "a = " << _a << ", b = " << _b << ", c = " << _c << std::endl;
	std::cout << "--------------------------------" << std::endl;
	return ;
}

void Computor::reducedForm()
{
	std::cout << "Reduced form of equation: ";
	_reducedForm.clear();
	bool hasPrintedTerm = false;
	for (size_t i = 0; i < _coefficientOrder.size(); i++)
	{
		const std::string &power = _coefficientOrder[i];
		double coefficient = _coefficients[power];
		if (coefficient == 0.0)
			continue;
		if (!hasPrintedTerm)
		{
			_reducedForm += _formatCoefficient(coefficient) + power;
			hasPrintedTerm = true;
		}
		else
		{
			if (coefficient < 0.0)
				_reducedForm += " - " + _formatCoefficient(-coefficient) + power;
			else
				_reducedForm += " + " + _formatCoefficient(coefficient) + power;
		}
	}
	if (!hasPrintedTerm)
		_reducedForm += "0";
	_reducedForm += " = 0";
	std::cout << _reducedForm << std::endl;
	return ;
}

void Computor::discriminant()
{
	std::cout << "Calculating discriminant..." << std::endl;
	std::cout << "b^2 - 4ac = " << _b << "^2 - 4 * " << _a << " * " << _c << std::endl;
	_delta = _b * _b - 4 * _a * _c;
	std::cout << "Δ = " << _delta << std::endl;
	return ;
}

void Computor::solve()
{
	if (_delta > 0)
	{
		std::cout << "So Δ > 0, the equation has two real solutions" << std::endl;
		_solutions = "x1 = " + _formatCoefficient((-_b + std::sqrt(_delta)) / (2 * _a)) + " and x2 = " + _formatCoefficient((-_b - std::sqrt(_delta)) / (2 * _a));
	}
	else if (_delta == 0)
	{
		std::cout << "So Δ = 0, the equation has one real solution" << std::endl;
		_solutions = "x = " + _formatCoefficient(-_b / (2 * _a));
	}
	else
	{
		std::cout << "So Δ < 0, the equation has two complex solutions" << std::endl;
		_solutions = "x1 = " + _formatCoefficient(-_b / (2 * _a)) + " + i * " + _formatCoefficient(std::sqrt(-_delta) / (2 * _a)) + " and x2 = " + _formatCoefficient(-_b / (2 * _a)) + " - i * " + _formatCoefficient(std::sqrt(-_delta) / (2 * _a));
	}
	std::cout << "The solutions are: " << std::endl;
	std::cout << _solutions << std::endl;
	return ;
}