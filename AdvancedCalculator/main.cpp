#include <iostream>
#include <list>

#include "term.h"
#include "expression.h"
#include "utils.h"

int main() {

	string_to_num("(1234.34 + sin(1234) + {3 * 2^3})");

	//string_to_term("(1234.34 + {3 * 2^3}) * x ^ 24.56");

	return 0;
}