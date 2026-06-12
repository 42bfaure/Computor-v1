#ifndef COMPUTOR_HPP
# define COMPUTOR_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>
#include "utils.hpp"

#define LEFT_SIDE 0
#define RIGHT_SIDE 1

class Computor
{
	private:
		std::string _equation;
		std::string _reducedForm;
		int _degree; 
        std::string _solutions;
		std::string _discriminant;
        std::map<int, std::vector<std::string> > _terms;
        double _a = 0.0;
        double _b = 0.0;
        double _c = 0.0;
        double _delta; // 0 : Δ < 0, 1 : Δ = 0, 2 : Δ > 0
		std::map<std::string, double> _coefficients;
		std::vector<std::string> _coefficientOrder;
        void _parseCoefficients();
		void _displaySolutionSteps() const;
        bool _verbose = false;

	public:
		Computor(std::string equation = "");
		~Computor();
		void Calcul();
		void parseEquation();
		void reducedForm();
		void discriminant();
		void degree();
		void solve();
		void display();
		void setTerms(std::string equation, int index);
		void setEquation(std::string equation);
		void setVerbose(bool verbose);
};

#endif