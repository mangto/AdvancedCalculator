#pragma once
#ifndef TERM_H
#define TERM_H

#include <iostream>

class Term {
	public:
		double coefficent;
		double power;

		Term(double coefficent = 1.0f, double power = 0.0f);

		void print();

};

void string_to_term(std::string TermLikeString);

#endif // !TERM_H
