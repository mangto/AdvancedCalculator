#pragma once
#ifndef TERM_H
#define TERM_H

#include <iostream>

class Term {
	public:
		double coefficent;
		double power;

		Term(double Coefficent = 1, double Power = 0);

		void print();

};

void string_to_term(std::string TermLikeString);

#endif // !TERM_H
