#ifndef COMPUTOR_HPP
# define COMPUTOR_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

#define LEFT_SIDE 0
#define RIGHT_SIDE 1

class Computor
{
	private:
		std::string _equation;
		std::string _reducedForm;
		int _discriminant; // 0 : Δ > 0, 1 : Δ = 0, 2 : Δ < 0
		int _degree;
        std::string _solutions;
        std::map<int, std::vector<std::string> > _terms;
        int _a;
        int _b;
        int _c;
        int _delta;
		std::map<int, std::vector<double> > _coefficients;
        void _parseCoefficients();
        void _splitTerms(std::string equation, int index);
        std::map<int, std::vector<std::string> > _split(const std::string &str, int index);

	public:
		Computor(std::string equation);
		~Computor();
		void parseEquation();
		void reducedForm();
		void discriminant();
		void degree();
		void solve();
		void display();
};

#endif