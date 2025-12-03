#include "term.h"
#include "calc.h"

Term::Term(double coefficent, double power) {
	this->coefficent = coefficent;
	this->power = power;
}

Term::Term(std::string TermLikeString) {
	/*
	=======================================
	Convert Term-Like-String to Term Object 
	=======================================
	*/

	// 12.34 * x ^ 24.56
	int length = TermLikeString.length();
	char buffer;
	std::string nums[2] = { "", "" };
	int index = 0;
	
	for (int i = 0; i < length; i++) {
		buffer = TermLikeString[i];
		
		if (buffer == 'x') { index++; continue; }
		else if (buffer == ' ') { continue; }
		else if (buffer == '{' || buffer == '[') { buffer = '('; }
		else if (buffer == '}' || buffer == ']') { buffer = ')'; }

		nums[index].push_back(buffer);

	}



	// calculate expression
	if (nums[0] != "") {
		if (nums[0].back() == '*') { nums[0].pop_back(); }
		this->coefficent = Calc::string_to_num(nums[0]);
	}
	if (nums[1] != "") { // if power exists
		if (nums[1].front() == '^') { nums[1].erase(0, 1); }
		this->power = Calc::string_to_num(nums[1]);
	}
}

void Term::print() {
	if (this->coefficent != 1.0f) { std::cout << this->coefficent << " * "; }
	std::cout << "x";
	if (this->power != 1.0f) { std::cout << " ^ " << this->power; }
}

double Term::Substitution(double x) {
	return this->coefficent * pow(x, this->power);
}