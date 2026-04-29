#include "../headers/Computor.hpp"

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
	return ;
}

void Computor::_parseCoefficients()
{
	_coefficients[LEFT_SIDE].assign(3, 0.0);
	_coefficients[RIGHT_SIDE].assign(3, 0.0);

	for (size_t i = 0; i < _terms.at(LEFT_SIDE).size(); i++)
	{
		const std::string &term = _terms.at(LEFT_SIDE).at(i);
		size_t pos = term.find("X^");
		if (pos != std::string::npos && (pos + 2) < term.size())
		{
			double coef = std::stod(term.substr(0, pos));
			int exp = std::stoi(term.substr(pos + 2));
			if (exp >= 0 && exp <= 2)
				_coefficients[LEFT_SIDE][exp] += coef;
		}
	}
	for (size_t i = 0; i < _terms.at(RIGHT_SIDE).size(); i++)
	{
		const std::string &term = _terms.at(RIGHT_SIDE).at(i);
		size_t pos = term.find("X^");
		if (pos != std::string::npos && (pos + 2) < term.size())
		{
			double coef = std::stod(term.substr(0, pos));
			int exp = std::stoi(term.substr(pos + 2));
			if (exp >= 0 && exp <= 2)
				_coefficients[RIGHT_SIDE][exp] += coef;
		}
	}
	for (int exp = 0; exp <= 2; exp++)
	{
		std::cout << "Left coeff X^" << exp << ": " << _coefficients[LEFT_SIDE][exp] << std::endl;
		std::cout << "Right coeff X^" << exp << ": " << _coefficients[RIGHT_SIDE][exp] << std::endl;
	}
	return ;
}

void Computor::reducedForm()
{
	std::cout << "Reducing form of equation: " << _equation << std::endl;

}