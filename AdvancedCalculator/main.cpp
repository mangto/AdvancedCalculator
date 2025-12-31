#include <iostream>

#include "expression.h"

int main() {

	ep::Variable::create_variable('x');
	ep::create_constant("dou", 175.5l);

	ep::Expression expression = ep::parse::parse_string("3*x*(1+16*sin(2/3*pi))");

	ep::Variable::set_value('x', 1.0l);

	std::cout << expression.substitution() << std::endl;

	return 0;
}