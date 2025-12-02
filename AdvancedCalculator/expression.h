#pragma once
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <list>

#include "term.h"

class Expression {
	public:
		std::list<Term> terms;

		Expression(const std::list<Term>& Terms);
};


#endif // !EXPRESSION_H
