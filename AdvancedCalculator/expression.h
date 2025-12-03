#pragma once
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>

#include "term.h"

class Expression {
	public:
		std::vector<Term> terms;

		Expression(std::vector<Term> terms);
		Expression(std::string ExpressionLikeString);

		void print();
};


#endif // !EXPRESSION_H
