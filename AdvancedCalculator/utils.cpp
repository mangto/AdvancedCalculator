#include <iostream>
#include <string>
#include <vector>
#include <stack>

#include "utils.h"

/* Token Class */

Token::Token() {
	value = 0.0f;
	character = "_";
	type = 0;
}

Token::Token(double value) {
	this->value = value;
	this->character = "_";
	this->type = 1;
}

Token::Token(std::string value) {
	this->value = 0.0f;
	this->character = value;
	this->type = 2;
}

void Token::print() {
	if (type == 1) { std::cout << value; }
	else if (type == 2) { std::cout << "\"" << character << "\""; }
	else { std::cout << character; }
}

void print_token_vector(std::vector<Token> TokenVector) {
	int length = TokenVector.size();

	std::cout << "[Token List]" << std::endl;

	for (Token token: TokenVector) {

		std::cout << " [Tk] Type." << token.type << " ";
		token.print();
		std::cout << "," << std::endl;
	}
}

/* Utils Functions */

bool is_digit(std::string DigitLikeString) {
	if (DigitLikeString == "0" ||
		DigitLikeString == "1" ||
		DigitLikeString == "2" ||
		DigitLikeString == "3" ||
		DigitLikeString == "4" ||
		DigitLikeString == "5" ||
		DigitLikeString == "6" ||
		DigitLikeString == "7" ||
		DigitLikeString == "8" ||
		DigitLikeString == "9") {
		return true;
	}

	return false;
}

bool is_digit(char DigitLikeChar) {
	
	std::string str(1, DigitLikeChar);

	return is_digit(str);
}

int digit_to_int(std::string DigitLikeString) {
	if (DigitLikeString == "0") { return 0; }
	else if (DigitLikeString == "1") { return 1; }
	else if (DigitLikeString == "2") { return 2; }
	else if (DigitLikeString == "3") { return 3; }
	else if (DigitLikeString == "4") { return 4; }
	else if (DigitLikeString == "5") { return 5; }
	else if (DigitLikeString == "6") { return 6; }
	else if (DigitLikeString == "7") { return 7; }
	else if (DigitLikeString == "8") { return 8; }
	else if (DigitLikeString == "9") { return 9; }
}

int digit_to_int(char DigitLikeChar) {
	std::string str(1, DigitLikeChar);

	return digit_to_int(str);
}

double string_to_num(std::string NumLikeString) {

	int length = NumLikeString.size();
	char target;
	int type = 0; // 1: num, 2: string, 3: bracket_open, 4: bracket_close, 5: operator, Negative => changed
	int PrvType = 0;
	std::string buffer = "";
	std::vector<Token> TokenVector = {};

	for (int i = 0; i < length; i++) {

		target = NumLikeString[i];

		//std::cout << target << std::endl;

		if (target == ' ') { continue; } // Ignore Space

		// Digit (including floating point): Type.1
		else if (is_digit(target) || target == '.') {

			if (type != 0 && abs(type) != 1) { PrvType = abs(type);  type = -1; }
			else { type = 1; }

			buffer.push_back(target);
		}

		// Open-Bracket: Type.3
		else if (target == '(' || target == '{' || target == '[') {
			if (type != 0 && abs(type) != 3) { PrvType = abs(type); type = -3; }
			else { type = 3; }

			target = '(';

		}

		// Close-Bracket: Type.4
		else if (target == ')' || target == '}' || target == ']') {
			if (type != 0 && abs(type) != 4) { PrvType = abs(type); type = -4; }
			else { type = 4; }

			target = ')';
		}

		// Operator: Type.5
		else if (target == '+' || target == '-' ||
				 target == '*' || target == '/' ||
				 target == '^') {
			if (type != 0 && abs(type) != 5) { PrvType = abs(type); type = -5; }
			else { type = 5; }
		}

		// String: Type.2
		else {
			if (type != 0 && abs(type) != 2) { PrvType = abs(type); type = -2; }
			else { type = 2; }

			buffer.push_back(target);
		}

		//std::cout << "bf: " << buffer << std::endl;

		// push
		if (type <= 0 && buffer != "" && (PrvType == 1 || PrvType == 2)) {

			Token NewToken(buffer);
			NewToken.type = abs(PrvType);

			if (NewToken.type == 1) {
				NewToken.value = std::stod(buffer);
			}

			TokenVector.push_back(NewToken);

			buffer = "";

		}

		if (abs(type) == 3 || abs(type) == 4 || abs(type) == 5) {
			std::string buffer(1, target);
			Token NewToken(buffer);
			NewToken.type = abs(type);
			TokenVector.push_back(NewToken);
			buffer = "";

		}

		if (i == length - 1 && buffer != "") { // last
			Token NewToken(buffer);
			NewToken.type = abs(type);
			if (NewToken.type == 1) {
				NewToken.value = std::stod(buffer);
			}

			TokenVector.push_back(NewToken);

			buffer = "";
		}
	}

	print_token_vector(TokenVector);

	parse(TokenVector);

	return 0.0f;
}


/* Shunting Yard Algorithm */
/*
Rule 1

If the current input token is a symbol (= number):
   Add it directly to the output token list.

Rule 2

If the current input token is an opening (left) parenthesis:
   Add it to the operator stack.

Rule 3

If the current input token is a closing (right) parenthesis:
   While the token at top of the operator stack is not a left parenthesis:
	  Pop the token from the stack and add it to the output token.
   Remove the final left parenthesis from the stack and discard it (do not add it to the output list).

Rule 4

If the current input token is an operator:
   While the token at top of the operator stack is
	 not a left parenthesis AND
	 has a higher precedence OR (the same precedence and current token is left associative):
	   Pop the token from the stack and add it to the output token list
   Add current input token operator to the operator stack.
*/
/*
	level
		4: ( )
		3: ^
		2: * /
		1: + -
	*/

void print_token_stack(std::stack<Token> TokenStack) {
	

	std::cout << "{ ";
	
	while (!TokenStack.empty()) {
		TokenStack.top().print();
		std::cout << ", ";
		TokenStack.pop();
	}

	std::cout << " }" << std::endl;
}

int get_operator_level(Token tk) {
	if (tk.character == "(" || tk.character == ")") { return 4; }
	else if (tk.character == "^") { return 3; }
	else if (tk.character == "*" || tk.character == "/") { return 2; }
	else if (tk.character == "+" || tk.character == "-") { return 1; }
	else { return -1; }
}

void parse(std::vector<Token> TokenVector) {
	std::stack<Token> output = {};
	std::stack<Token> op = {}; // operator
	
	for (Token tk : TokenVector) {

		if (tk.type == 1) { output.push(tk); } // Rule 1
		else if (tk.type == 2) { output.push(tk); }
		else if (tk.type == 3) { op.push(tk); } // Rule 2
		else if (tk.type == 4) { // Rule 3
			while (op.top().type != 3) {
				output.push(op.top());
				op.pop();
			}
			op.pop();
		}
		else if (tk.type == 5) { // Rule 4.
			while (
				!op.empty() && // error handling
				op.top().type != 3 && // not a left parenthesis
				(get_operator_level(tk) < get_operator_level(op.top()) || // has a higher precedence
					(get_operator_level(tk) == get_operator_level(op.top()) && tk.character != "^"))) {
					// ^^^^^^: the same precedence and current token is left associative

				output.push(op.top());
				op.pop();
			}

			op.push(tk);
		}

		// debugging
		std::cout << "========================" << std::endl;

		std::cout << "read: ";
		tk.print();
		std::cout << "" << std::endl;

		print_token_stack(output);
		print_token_stack(op);
	}

	while (!op.empty()) {
		output.push(op.top());
		op.pop();
	}

	std::cout << "========================" << std::endl;
	print_token_stack(output);
	print_token_stack(op);

}