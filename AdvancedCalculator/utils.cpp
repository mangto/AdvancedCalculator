#include <iostream>
#include <string>
#include <vector>

#include "utils.h"

/* Token Class */

Token::Token() {
	value = 0.0f;
	character = "_";
	type = 0;
}

Token::Token(double Value) {
	value = Value;
	character = "_";
	type = 1;
}

Token::Token(std::string Value) {
	value = 0.0f;
	character = Value;
	type = 2;
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

/* Node Class */

Node::Node() {
	Token value(0.0f);
}

Node::Node(Token token) {
	value = token;
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

		std::cout << target << std::endl;

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

		// Close-Bracket: Type.3
		else if (target == ')' || target == '}' || target == ']') {
			if (type != 0 && abs(type) != 3) { PrvType = abs(type); type = -3; }
			else { type = 3; }

			target = ')';
		}

		// Operator: Type.4
		else if (target == '+' || target == '-' ||
				 target == '*' || target == '/' ||
				 target == '^') {
			if (type != 0 && abs(type) != 4) { PrvType = abs(type); type = -4; }
			else { type = 4; }
		}

		// String: Type.2
		else {
			if (type != 0 && abs(type) != 2) { PrvType = abs(type); type = -2; }
			else { type = 2; }

			buffer.push_back(target);
		}

		// push
		if (type <= 0 && buffer != "") {

			if (PrvType == 1 || PrvType == 2) {

				Token NewToken(buffer);
				NewToken.type = abs(PrvType);

				if (NewToken.type == 1) {
					NewToken.value = std::stod(buffer);
				}

				TokenVector.push_back(NewToken);

				buffer = "";
			}

		}

		if (abs(type) == 3 || abs(type) == 4) {
			std::string buffer(1, target);
			Token NewToken(buffer);
			NewToken.type = abs(type);
			TokenVector.push_back(NewToken);
			buffer = "";

		}
	}

	print_token_vector(TokenVector);

	return 0.0f;
}

Node create_node(std::vector<Token> TokenVector) {
	Node head;
	int length = TokenVector.size();
	
	for (int i = 0; i < length; i++) {

		Token tk = TokenVector[i];
		Node NewNode(tk);
		
		if (tk.type == 3) {
			std::vector<Token> SlicdeTokenVector(TokenVector.begin() + i + 1, TokenVector.end());
			Node ChildNode = create_node(SlicdeTokenVector);

			NewNode.child.push_back(ChildNode);
		}
	}

	return head;
}