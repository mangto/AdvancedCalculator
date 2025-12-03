#pragma once
#ifndef TERM_H
#define TERM_H

#include <iostream>

#include "calc.h"
#include "term.h"

class Term {
	public:
		double coefficent;
		double power;

		Term(double coefficent = 1.0f, double power = 1.0f);
		Term(std::string TermLikeString);

		void print();
		double Substitution(double x);

};

#endif // !TERM_H
