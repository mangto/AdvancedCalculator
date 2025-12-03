#include "term.h"
#include "expression.h"
#include "calc.h"

int main() {

	std::string input;
	double result=0.0f;

	while (true) {

		std::cout << "Type Term\n >>> ";
		std::cin >> input;

		Expression MyExpression(input);
		MyExpression.print();
		std::cout << std::endl;
	}
	return 0;
}