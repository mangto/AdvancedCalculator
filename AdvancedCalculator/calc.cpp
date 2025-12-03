#include "calc.h"

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

void Calc::print_token_vector(std::vector<Token> TokenVector) {
	int length = TokenVector.size();

	std::cout << "{ ";

	for (Token token: TokenVector) {
		token.print();
		std::cout << ", ";
	}

	std::cout << "}" << std::endl;
}

/* Utils Functions */

bool Calc::is_digit(std::string DigitLikeString) {
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

bool Calc::is_digit(char DigitLikeChar) {
	
	std::string str(1, DigitLikeChar);

	return Calc::is_digit(str);
}

int Calc::digit_to_int(std::string DigitLikeString) {
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

int Calc::digit_to_int(char DigitLikeChar) {
	std::string str(1, DigitLikeChar);

	return Calc::digit_to_int(str);
}

double Calc::string_to_num(std::string NumLikeString) {

	std::vector<Token> TokenVector = Calc::tokenize(NumLikeString);

	//print_token_vector(TokenVector); // debugging

	std::vector<Token> parsed = Calc::parse(TokenVector);
	
	//print_token_vector(parsed); // debugging

	double result = Calc::calculate_parsed(parsed);

	return result;
}

std::vector<Token> Calc::tokenize(std::string NumLikeString) {
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

			if (type != 1 && abs(type) != 1) { PrvType = abs(type);  type = -1; }
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
			else {
				if (NewToken.character == "pi") { NewToken.type = 1; NewToken.value = PI; }
				else if (NewToken.character == "e") { NewToken.type = 1; NewToken.value = E; }
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


	return TokenVector;
}

/* Shunting Yard Algorithm */
/* Shunting Yard Algorithm Rules
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

void Calc::print_token_stack(std::stack<Token> TokenStack) {
	
	std::cout << "{ ";
	
	while (!TokenStack.empty()) {
		TokenStack.top().print();
		std::cout << ", ";
		TokenStack.pop();
	}

	std::cout << "}" << std::endl;
}

int Calc::get_operator_level(Token tk) {
	if (tk.character == "(" || tk.character == ")") { return 4; }
	else if (tk.character == "^") { return 3; }
	else if (tk.character == "*" || tk.character == "/") { return 2; }
	else if (tk.character == "+" || tk.character == "-") { return 1; }
	else if (tk.type == 2) {
		return 3;
	}
}

std::vector<Token> Calc::parse(std::vector<Token> TokenVector) {
	std::vector<Token> output = {};
	std::stack<Token> op = {}; // operator
	
	for (Token tk : TokenVector) {

		if (tk.type == 1) { output.push_back(tk); } // Rule 1
		else if (tk.type == 3) { op.push(tk); } // Rule 2
		else if (tk.type == 4) { // Rule 3
			while (op.top().type != 3) {
				output.push_back(op.top());
				op.pop();
			}
			op.pop();
		}
		else if (tk.type == 5 || tk.type == 2) { // Rule 4.
			while (
				!op.empty() && // error handling
				op.top().type != 3 && // not a left parenthesis
				(Calc::get_operator_level(tk) < Calc::get_operator_level(op.top()) || // has a higher precedence
					(Calc::get_operator_level(tk) == Calc::get_operator_level(op.top()) && tk.character != "^"))) {
					// ^^^^^^: the same precedence and current token is left associative

				output.push_back(op.top());
				op.pop();
			}

			op.push(tk);
		}

		// debugging
		//std::cout << "========================" << std::endl;

		//std::cout << "read: ";
		//tk.print();
		//std::cout << "" << std::endl;

		//print_token_stack(output);
		//print_token_stack(op);
	}

	while (!op.empty()) { // remainigs
		output.push_back(op.top());
		op.pop();
	}

	//std::cout << "========================" << std::endl;
	//print_token_stack(output);
	//print_token_stack(op);

	return output;

}

double Calc::calculate_parsed(std::vector<Token> ParsedTokenVector) {
	std::vector<double> temp;

	for (Token tk : ParsedTokenVector) {

		//std::cout << "====================" << std::endl;
		//for (double val : temp) { std::cout << val << ", "; }
		//std::cout << std::endl;

		if (tk.type == 1) { temp.push_back(tk.value); } // num
		else if (tk.type == 5) { // operator
			double a = temp[temp.size() - 2];
			double b = temp[temp.size() - 1];

			temp.pop_back();
			temp.pop_back();

			if (tk.character == "+") { temp.push_back(a + b); }
			else if (tk.character == "-") { temp.push_back(a - b); }
			else if (tk.character == "*") { temp.push_back(a * b); }
			else if (tk.character == "^") { temp.push_back(pow(a, b)); }
			else if (tk.character == "/") {
				if (b == 0.0f) { return 0.0f; } // divided by 0

				temp.push_back(a / b);
			}
		}

		else if (tk.type == 2) { // string

			double a = temp[temp.size() - 1];
			double result = 0.0f;

			temp.pop_back();

			if (tk.character == "sin") { result = sin(a); }
			else if (tk.character == "cos") { result = cos(a); }
			else if (tk.character == "tan") { result = tan(a); }
			else if (tk.character == "log") { result = log10(a); }
			else if (tk.character == "ln") { result = log(a); }

			temp.push_back(result);
		}
	}

	return temp[0];
}