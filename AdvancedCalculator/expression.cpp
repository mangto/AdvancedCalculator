#include "expression.h"

/* namespace: ep */
/* class: Expression*/
ep::Expression::Expression(std::string ExpressionLikeString) {

};

/* class: Variable */
ep::Variable::Variable(char character) {
	this->character = character;
	ep::VARIABLES.insert({ character, this });
}

ep::Variable ep::Variable::create_variable(char character) {
	// same as ep::Variable(char character)
	ep::Variable var(character);

	return var;
}

void ep::Variable::set_value(char character, double value) {

	if (ep::VARIABLES.find(character) != ep::VARIABLES.end()) {

		ep::VARIABLES[character]->value = value;

	}

}

double ep::Variable::get_value(char character) {
	
	if (ep::VARIABLES.find(character) != ep::VARIABLES.end()) {

		return ep::VARIABLES[character]->value;

	}

	return 0.0f;
}

double ep::Variable::get_value(char character, double DefaultValue) {
	
	if (ep::VARIABLES.find(character) != ep::VARIABLES.end()) {

		return ep::VARIABLES[character]->value;

	}

	return DefaultValue;
}

/* Token */

void ep::add_constant(std::string name, double value) {
	if (ep::CONSTANTS.find(name) != ep::CONSTANTS.end()) {
		ep::CONSTANTS[name] = value;
	}
	else {
		ep::CONSTANTS.insert({ name, value });
	}
}

ep::Token::Token() {
	this->type = ep::TokenType::NONE;
	this->function = ep::FunctionType::NONE;
	this->text = "";
	this->value = 0.0f;
	this->id = 0;
}

ep::Token::Token(TokenType type, std::string text, int id) {
	this->type = type;
	this->function = ep::FunctionType::NONE;
	this->text = text;
	this->value = 0.0f;
	this->id = id;
}

ep::Token::Token(TokenType type, std::string text) {
	this->type = type;
	this->function = ep::FunctionType::NONE;
	this->text = text;
	this->value = 0.0f;
	this->id = 0;
}

ep::Token::Token(TokenType type, double value) {
	this->type = type;
	this->function = ep::FunctionType::NONE;
	this->text = text;
	this->value = value;
	this->id = 0;
}

void ep::Token::print() {
	if (this->type == ep::TokenType::NUM) {
		std::cout << this->value; 
	}
	
	else if (this->type == ep::TokenType::VARIABLE || this->type == ep::TokenType::FUNCTION) { 
		std::cout << "<" << this->text<<">";
	}

	else if (this->type == ep::TokenType::BRACKET_OPEN || this->type == ep::TokenType::BRACKET_CLOSE) {
		std::cout << this->text;
	}
	
	else { std::cout << "\"" << this->text << "\""; }
}

void ep::Token::auto_convert_type(ep::Token* token) {

	if (token->type == ep::TokenType::NUM) {
		token->value = std::stod(token->text);
	}

	else if (token->type == ep::TokenType::OPERATOR) {
		if (token->text == "+") { token->id = 0; }
		else if (token->text == "-") { token->id = 1; }
		else if (token->text == "*") { token->id = 2; }
		else if (token->text == "/") { token->id = 3; }
		else if (token->text == "^") { token->id = 4; }

	}

	else if (token->type == ep::TokenType::BRACKET_OPEN) { token->id = 0; }
	else if (token->type == ep::TokenType::BRACKET_CLOSE) { token->id = 1; }
	else {
		if (ep::CONSTANTS.find(token->text) != ep::CONSTANTS.end()) {
			token->type = ep::TokenType::NUM;
			token->value = ep::CONSTANTS[token->text];
		}

		else if (token->text.size() == 1 && ep::VARIABLES.find(token->text[0]) != ep::VARIABLES.end()) {
			token->type = ep::TokenType::VARIABLE;
		}
		else if (token->text == "sin") {
			token->function = ep::FunctionType::SIN;
			token->type = ep::TokenType::FUNCTION;
		}
		else if (token->text == "cos") {
			token->function = ep::FunctionType::COS;
			token->type = ep::TokenType::FUNCTION;
		}
		else if (token->text == "tan") {
			token->function = ep::FunctionType::TAN;
			token->type = ep::TokenType::FUNCTION;
		}
		else if (token->text == "log") {
			token->function = ep::FunctionType::LOG;
			token->type = ep::TokenType::FUNCTION;
		}
		else if (token->text == "ln") {
			token->function = ep::FunctionType::LN;
			token->type = ep::TokenType::FUNCTION;
		}
	}
}

/* namespace: parse */

/* class: binary tree */

ep::parse::Node::Node() {

}

ep::parse::Node::Node(ep::Token token) {
	this->token = token;
}

void ep::parse::Node::set_left(ep::parse::Node* left) {
	this->left = left;
}

void ep::parse::Node::set_right(ep::parse::Node* right) {
	this->right = right;
}

/* parse */

void ep::parse::print_token_queue(std::queue<ep::Token> TokenQueue) {
	ep::Token token;
	int length = TokenQueue.size();

	std::cout << "<Queue> {";

	for (int i = 0; i < length; i++) {
		token = TokenQueue.front();
		token.print();
		std::cout << ", ";
		TokenQueue.pop();
	}
	std::cout << "}" << std::endl;
}

ep::TokenType ep::parse::get_type(char character) {

	// num
	if (character == '0' || character == '1' ||
		character == '2' || character == '3' ||
		character == '4' || character == '5' ||
		character == '6' || character == '7' ||
		character == '8' || character == '9') {
		return ep::TokenType::NUM;
	}

	// operator
	else if ( character == '+' || character == '-' ||
			  character == '*' || character == '/' ||
			  character == '^') {
		return ep::TokenType::OPERATOR;
	}
	
	// bracket
	else if (character == '(' || character == '{' || character == '[') {
		return ep::TokenType::BRACKET_OPEN;
	}

	else if (character == ')' || character == '}' || character == ']') {
		return ep::TokenType::BRACKET_CLOSE;
	}
	
	else { return ep::TokenType::STRING; }


}


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

std::queue<ep::Token> ep::parse::tokenize(std::string ExpressionLikeString) {
	
	std::queue<ep::Token> output = {};

	std::string temp = "";
	ep::TokenType type = ep::TokenType::NONE;
	ep::TokenType PrvType = ep::TokenType::NONE;
	bool PushBuffer;

	for (char buffer : ExpressionLikeString) {
		PushBuffer = true;
		type = ep::parse::get_type(buffer);

		if (type == ep::TokenType::NUM || type == ep::TokenType::STRING) {
			temp.push_back(buffer);
			PushBuffer = false;
		}
		else if (type == ep::TokenType::BRACKET_OPEN) { buffer = '('; }
		else if (type == ep::TokenType::BRACKET_CLOSE) { buffer = ')'; }

		if (PrvType != ep::TokenType::NONE && type != PrvType && temp != "" &&
			(PrvType == ep::TokenType::NUM || PrvType == ep::TokenType::STRING)) {

			ep::Token NewToken(PrvType, temp);
			ep::Token::auto_convert_type(&NewToken);

			output.push(NewToken);
			temp = "";
			temp.push_back(buffer);
		}

		if (PushBuffer) {
			temp = "";
			temp.push_back(buffer); 
			ep::Token NewToken(type, temp);
			output.push(NewToken);
			temp = "";
		}
		
		PrvType = type;
	}

	ep::Token NewToken(PrvType, temp);
	ep::Token::auto_convert_type(&NewToken);
	output.push(NewToken);

	return output;

}

ep::parse::Node ep::parse::parse_string(std::string ExpressionLikeString) {
	
	std::queue<ep::Token> output = {};
	std::stack<ep::Token> op = {}; // operator

	ep::parse::Node head;

	std::queue<ep::Token> tokenized = ep::parse::tokenize(ExpressionLikeString);

	int length = tokenized.size();
	Token buffer;

	// shunting yard algoritm + binary tree

	for (int i = 0; i < length; i++) {
		buffer = tokenized.front();

		// Rule.1
		if (buffer.type == ep::TokenType::NUM || buffer.type == ep::TokenType::VARIABLE) {
			output.push(buffer);
		}

		// Rule.2
		else if (buffer.type == ep::TokenType::BRACKET_OPEN) {
			op.push(buffer);
		}

		// Rule.3
		else if (buffer.type == ep::TokenType::BRACKET_CLOSE) {
			while (op.top().type != ep::TokenType::BRACKET_OPEN) {
				output.push(op.top());
				op.pop();
			}

			op.pop();
		}

		else if (buffer.type == ep::TokenType::OPERATOR || buffer.type == ep::TokenType::FUNCTION) {
			while (
				!op.empty() &&
				op.top().type != ep::TokenType::BRACKET_OPEN &&
				((op.top().id / 2 < buffer.id / 2) ||
				(op.top().id / 2 == buffer.id / 2 && buffer.id != 4))) {
				output.push(op.top());
				op.pop();
			}
			op.push(buffer);
		}

		tokenized.pop();
	}

	ep::parse::print_token_queue(output);

	return head;
}