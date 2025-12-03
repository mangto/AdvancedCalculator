#include <iostream>
#include <string>

#include "term.h"
#include "utils.h"

Term::Term(double coefficent, double power) {
	this->coefficent = coefficent;
	this->power = power;
}

void string_to_term(std::string TermLikeString) {
	/*
	=======================================
	Convert Term-Like-String to Term Object 
	=======================================

	Step 1. Scan left to right
		-> Seperate nums
	Step 2. Define new term
	*/

	// 12.34 * x ^ 24.56
	int length = TermLikeString.length();
	char buffer;
	std::string nums[2] = { "", "" };
	int index = 0;
	
	for (int i = 0; i < length; i++) { // Step 1.
		buffer = TermLikeString[i];
		
		if (buffer == 'x') { index++; continue; }
		else if (buffer == ' ') { continue; }
		else if (buffer == '{' || buffer == '[') { buffer = '('; }
		else if (buffer == '}' || buffer == ']') { buffer = ')'; }

		nums[index].push_back(buffer);

	}

	if (nums[0].back() == '*') { nums[0].pop_back(); }
	if (nums[1].front() == '^') { nums[1].erase(0, 1); }

	std::cout << "{ \"" << nums[0] << "\", \"" << nums[1] << "\"}" << std::endl;

	return;
}

void Term::print() {
	if (power == 0.0f) {
		std::cout << coefficent << " * x" << std::endl;
	}
	else {
		std::cout << coefficent << " * x ^ " << power << std::endl;

	}
}