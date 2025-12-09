#include <iostream>

#include "expression.h"

int main() {

	ep::Variable::create_variable('x');
	ep::create_constant("dou", 175.5f);

	ep::parse::parse_string("3*x*(1+16*sin(2/3*pi))+dou");

	return 0;
}