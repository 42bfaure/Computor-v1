#ifndef COMPUTOR_HPP
# define COMPUTOR_HPP

/**
 * @file
 * @brief Declares the `Computor` class used to parse and solve polynomial equations.
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

#define LEFT_SIDE 0
#define RIGHT_SIDE 1

/**
 * @brief Parse, reduce, and solve a polynomial equation.
 *
 * The class stores an input equation, extracts terms from each side, computes
 * reduced coefficients, and prepares outputs such as reduced form, degree, and
 * solutions.
 */
class Computor
{
	private:
		/// Original equation string provided by the user.
		std::string _equation;
		/// Reduced polynomial form as a printable string.
		std::string _reducedForm;
		/// Discriminant classification flag.
		int _discriminant; // 0 : Δ > 0, 1 : Δ = 0, 2 : Δ < 0
		/// Polynomial degree.
		int _degree;
		/// Solutions output as a formatted string.
		std::string _solutions;
		/// Parsed terms split by equation side.
		std::map<int, std::vector<std::string> > _terms;
		/// Quadratic coefficient for `X^2`.
		int _a;
		/// Linear coefficient for `X^1`.
		int _b;
		/// Constant coefficient for `X^0`.
		int _c;
		/// Numeric discriminant value.
		int _delta;
		/// Coefficients indexed by power token (for example `X^2`).
		std::map<std::string, double> _coefficients;
		/// Ordered powers as they appear while parsing.
		std::vector<std::string> _coefficientOrder;

		/**
		 * @brief Format a floating-point coefficient for output.
		 * @param value Coefficient value.
		 * @return A compact decimal representation without trailing zeros.
		 */
		std::string _formatCoefficient(double value) const;

		/**
		 * @brief Parse split terms and aggregate coefficients by power.
		 */
		void _parseCoefficients();

		/**
		 * @brief Split one equation side into signed terms.
		 * @param equation Equation side to split.
		 * @param index Side index (`LEFT_SIDE` or `RIGHT_SIDE`).
		 */
		void _splitTerms(std::string equation, int index);

		/**
		 * @brief Tokenize a side of the equation into signed terms.
		 * @param str Equation side string.
		 * @param index Side index (`LEFT_SIDE` or `RIGHT_SIDE`).
		 * @return A map containing the extracted term list at `index`.
		 */
		std::map<int, std::vector<std::string> > _split(const std::string &str, int index);

	public:
		/**
		 * @brief Construct a solver from a polynomial equation.
		 * @param equation Input equation string.
		 */
		Computor(std::string equation);

		/// Destroy the solver instance.
		~Computor();

		/// Parse and normalize the equation.
		void parseEquation();

		/// Build and print the reduced form.
		void reducedForm();

		/// Compute discriminant information.
		void discriminant();

		/// Compute and print polynomial degree.
		void degree();

		/// Solve the equation according to its degree.
		void solve();

		/// Display final formatted results.
		void display();
};

#endif