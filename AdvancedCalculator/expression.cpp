#include "expression.h"

Expression::Expression(std::vector<Term> terms) {
	this->terms = terms;
}

Expression::Expression(std::string ExpressionLikeString) {
	int length = ExpressionLikeString.length();
	int BracketCount = 0;
	std::string temp = "";

	for (int i = 0; i < length; i++) {
		std::cout << "[" << i << "]" << std::endl;
		char buffer = ExpressionLikeString[i];
		
		std::cout << "buffer: " << buffer << " temp: " << temp << std::endl;

		if (buffer == ' ') { continue; }
		else if (buffer == '(') { BracketCount++; }
		else if (buffer == ')') { BracketCount--; }

		if (i == length - 1) { temp.push_back(buffer); }

		if ((BracketCount == 0 && (buffer == '+' || buffer == '-')) || (i == length - 1)) { // split terms
			std::cout << "temp: " << temp << std::endl;
			Term NewTerm(temp); // create new term
			this->terms.push_back(NewTerm);
			temp = "";

			continue;
		}

		temp.push_back(buffer);
	}
}

void Expression::print() {
	int length = this->terms.size();

	for (int i = 0; i < length; i++) {
		
		this->terms[i].print();

		if (i != length-1) { std::cout << " + "; }
	}
}