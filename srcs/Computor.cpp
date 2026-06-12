#include "../headers/Computor.hpp"

Computor::Computor(std::string equation)
{
	try
	{
		this->setEquation(equation);
	}
	catch (const std::exception &e)
	{
		throw std::invalid_argument(e.what());
	}
}

Computor::~Computor() {}

void Computor::Calcul()
{
	try
	{
		this->parseEquation();
		this->_parseCoefficients();
		this->degree();
		this->reducedForm();
		this->discriminant();
		this->solve();
	}
	catch (const std::exception &e)
	{
		throw std::invalid_argument(e.what());
	}
}
void Computor::setTerms(std::string equation, int index)
{
	this->_terms[index] = split(equation, index)[index];
	return ;
}

void Computor::setEquation(std::string equation)
{
	try
	{
		this->_equation = equation;
		checkCoefficients(equation);
	}
	catch (const std::exception &e)
	{
		throw std::invalid_argument(e.what());
	}
	return ;
}

void Computor::parseEquation()
{
	std::string leftSide;
	std::string rightSide;
	std::string cleaned;
	for (size_t i = 0; i < _equation.size(); ++i)
	{
		if (_equation[i] != ' ' && _equation[i] != '*')
		cleaned += _equation[i];
	}
	_equation = cleaned;
	for (size_t i = 0; i < _equation.length(); i++)
	{
		if (_equation[i] == '=')
		{
			leftSide = _equation.substr(0, i);
			rightSide = _equation.substr(i + 1);
			break;
		}
	}
	this->setTerms(leftSide, LEFT_SIDE);
	this->setTerms(rightSide, RIGHT_SIDE);
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
			i--;
			continue;
		}
		exp = term.substr(pos, pos + 2);
		if (std::find(_coefficientOrder.begin(), _coefficientOrder.end(), exp) == _coefficientOrder.end())
			_coefficientOrder.push_back(exp);
		_coefficients[exp] += std::stod(term.substr(0, pos));
	}
	for (size_t i = 0; i < _terms.at(RIGHT_SIDE).size(); i++)
	{
		const std::string &term = _terms.at(RIGHT_SIDE).at(i);
		size_t pos = term.find("X^");
		if (pos == std::string::npos)
		{
			_terms.at(RIGHT_SIDE).at(i) = _terms.at(RIGHT_SIDE).at(i) + "X^0";
			i--;
			continue;
		}
		exp = term.substr(pos, pos + 2);
		if (std::find(_coefficientOrder.begin(), _coefficientOrder.end(), exp) == _coefficientOrder.end())
		_coefficientOrder.push_back(exp);
		_coefficients[exp] -= std::stod(term.substr(0, pos));
	}
	
	_a = _coefficients["X^2"];
	_b = _coefficients["X^1"];
	_c = _coefficients["X^0"];
	return ;
}

void Computor::reducedForm()
{
	_reducedForm.clear();
	bool hasPrintedTerm = false;
	for (size_t i = 0; i < _coefficientOrder.size(); i++)
	{
		const std::string &power = _coefficientOrder[i];
		double coefficient = _coefficients[power];
		if (coefficient == 0.0 && !this->_verbose)
			continue;
		if (!hasPrintedTerm)
		{
			_reducedForm += formatCoefficient(coefficient) + power;
			hasPrintedTerm = true;
		}
		else
		{
			if (coefficient < 0.0)
				_reducedForm += " - " + formatCoefficient(-coefficient) + power;
			else
				_reducedForm += " + " + formatCoefficient(coefficient) + power;
		}
	}
	if (!hasPrintedTerm)
		_reducedForm += "0";
	_reducedForm += " = 0";
	return ;
}

void Computor::degree()
{
	_degree = _coefficientOrder.size() - 1;
	if (_degree < 0)
		throw std::invalid_argument("Invalid equation, I can't solve.");
	return ;
}

void Computor::discriminant()
{
	_delta = _b * _b - 4 * _a * _c;
	return ;
}

void Computor::solve()
{
	if (_degree == 0)
	{
		if (_c == 0.0)
			_solutions = "Any real number is a solution.";
		else
			_solutions = "No solution.";
	}
	else if (_degree == 1)
		_solutions = "x = " + formatCoefficient(-_c / _b);
	else if (_degree == 2)
	{
		if (_delta > 0)
		{
			_discriminant = "Discriminant is strictly positive, the two solutions are:";
			_solutions = "x1 = " + formatCoefficient((-_b + squareRoot(_delta)) / (2 * _a)) + " and x2 = " + formatCoefficient((-_b - squareRoot(_delta)) / (2 * _a));
		}
		else if (_delta == 0)
		{
			_discriminant = "The solution is:";
			_solutions = "x = " + formatCoefficient(-_b / (2 * _a));
		}
		else
		{
			_discriminant = "Discriminant is strictly negative, the two complex solutions are:";
			_solutions = "x1 = " + formatCoefficient(-_b / (2 * _a)) + " + i * " + formatCoefficient(squareRoot(-_delta) / (2 * _a)) + " and x2 = " + formatCoefficient(-_b / (2 * _a)) + " - i * " + formatCoefficient(squareRoot(-_delta) / (2 * _a));
		}
	}
	else
		_solutions = "The polynomial degree is strictly greater than 2, I can't solve.";
	return ;
}

void Computor::setVerbose(bool verbose)
{
	this->_verbose = verbose;
	return ;
}

void Computor::_displaySolutionSteps() const
{
	if (_degree != 2)
	{
		if (_degree == 1)
		{
			std::cout << "x = -c / b" << std::endl;
			std::cout << "  = -" << formatCoefficient(_c) << " / " << formatCoefficient(_b) << std::endl;
			return ;
		}
		else
			return ;
	}

	const double twoA = 2.0 * _a;

	if (_delta > 0)
	{
		const double sqrtDelta = squareRoot(_delta);
		const double x1 = (-_b + sqrtDelta) / twoA;
		const double x2 = (-_b - sqrtDelta) / twoA;

		std::cout << "x = (-b ± √(Δ)) / (2a)" << std::endl;
		std::cout << "x₁ = (-b + √(Δ)) / (2a)" << std::endl;
		std::cout << "  = (-" << formatCoefficient(_b) << " + √(" << formatCoefficient(_delta) << ")) / (2 · " << formatCoefficient(_a) << ")" << std::endl;
		std::cout << "  = (" << formatCoefficient(-_b) << " + " << formatCoefficient(sqrtDelta) << ") / " << formatCoefficient(twoA) << std::endl;
		std::cout << "  = " << formatCoefficient(x1) << std::endl;
		std::cout << "x₂ = (-b - √(Δ)) / (2a)" << std::endl;
		std::cout << "  = (-" << formatCoefficient(_b) << " - √(" << formatCoefficient(_delta) << ")) / (2 · " << formatCoefficient(_a) << ")" << std::endl;
		std::cout << "  = (" << formatCoefficient(-_b) << " - " << formatCoefficient(sqrtDelta) << ") / " << formatCoefficient(twoA) << std::endl;
		std::cout << "  = " << formatCoefficient(x2) << std::endl;
	}
	else if (_delta == 0)
	{
		const double x = -_b / twoA;

		std::cout << "x = -b / (2a)" << std::endl;
		std::cout << "  = -" << formatCoefficient(_b) << " / (2 · " << formatCoefficient(_a) << ")" << std::endl;
		std::cout << "  = " << formatCoefficient(-_b) << " / " << formatCoefficient(twoA) << std::endl;
		std::cout << "  = " << formatCoefficient(x) << std::endl;
	}
	else
	{
		const double sqrtNegDelta = squareRoot(-_delta);
		const double real = -_b / twoA;
		const double imag = sqrtNegDelta / twoA;

		std::cout << "x = (-b) / (2a) ± i · √(-Δ) / (2a)" << std::endl;
		std::cout << "ℜ(x) = -b / (2a)" << std::endl;
		std::cout << "  = -" << formatCoefficient(_b) << " / (2 · " << formatCoefficient(_a) << ")" << std::endl;
		std::cout << "  = " << formatCoefficient(real) << std::endl;
		std::cout << "ℑ(x) = √(-Δ) / (2a)" << std::endl;
		std::cout << "  = √(" << formatCoefficient(-_delta) << ") / (2 · " << formatCoefficient(_a) << ")" << std::endl;
		std::cout << "  = " << formatCoefficient(sqrtNegDelta) << " / " << formatCoefficient(twoA) << std::endl;
		std::cout << "  = " << formatCoefficient(imag) << std::endl;
		std::cout << "x₁ = ℜ(x) + i · ℑ(x) = " << formatCoefficient(real) << " + i · " << formatCoefficient(imag) << std::endl;
		std::cout << "x₂ = ℜ(x) - i · ℑ(x) = " << formatCoefficient(real) << " - i · " << formatCoefficient(imag) << std::endl;
	}
}

void Computor::display()
{
	std::cout << "Reduced form: " << _reducedForm << std::endl;
	if (this->_degree != 0)
		std::cout << "Polynomial degree: " << _degree << std::endl;
	if (this->_verbose)
	{
		if (!(this->_degree <= 2 && this->_degree >= 0))
		{
			std::cout << "No verbose mode available for polynomial degree greater than 2" << std::endl;
			return ;
		}
		std::cout << "Standard form: " << "aX^2 + bX + c = 0" << std::endl;
		std::cout << "a = " << formatCoefficient(_a) << std::endl;
		std::cout << "b = " << formatCoefficient(_b) << std::endl;
		std::cout << "c = " << formatCoefficient(_c) << std::endl;
		std::cout << "Δ = b² - 4ac" << std::endl;
		std::cout << "  = " << formatCoefficient(_b) << "² - 4 · " << formatCoefficient(_a) << " · " << formatCoefficient(_c) << std::endl;
		std::cout << "  = " << formatCoefficient(_delta) << std::endl;
	}
	std::cout << _discriminant << std::endl;
	if (this->_verbose)
		this->_displaySolutionSteps();
	std::cout << _solutions << std::endl;
	return ;
}