#include "expression.h"

/* namespace: ep */


/* Token */

void ep::create_constant(std::string name, long double value) {
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

ep::Token::Token(TokenType type, long double value) {
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
		std::cout << "<" << this->text << ">";
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
			token->id = 4;
		}
		else if (token->text == "cos") {
			token->function = ep::FunctionType::COS;
			token->type = ep::TokenType::FUNCTION;
			token->id = 4;
		}
		else if (token->text == "tan") {
			token->function = ep::FunctionType::TAN;
			token->type = ep::TokenType::FUNCTION;
			token->id = 4;
		}
		else if (token->text == "log") {
			token->function = ep::FunctionType::LOG;
			token->type = ep::TokenType::FUNCTION;
			token->id = 4;
		}
		else if (token->text == "ln") {
			token->function = ep::FunctionType::LN;
			token->type = ep::TokenType::FUNCTION;
			token->id = 4;
		}
		else {
			token->type = ep::TokenType::VARIABLE;
		}
	}
}


/* class: Expression*/
ep::Expression::Expression(std::queue<ep::Token> TokenQueue) {
	this->TokenQueue = TokenQueue;
};

void ep::Expression::simplize() {

}

long double ep::Expression::substitution() {

	std::stack<long double> output = {};
	std::queue<ep::Token> TokenQueue = this->TokenQueue;
	int length = TokenQueue.size();
	
	ep::Token* TkBuffer;
	ep::TokenType TypeBuffer;
	long double a = 0.0l, b = 0.0l; // Use when calculate with OPERATOR or FUNCTION

	for (int i = 0; i < length; i++) {

		TkBuffer = &TokenQueue.front();
		TypeBuffer = TkBuffer->type;

		if (TypeBuffer == ep::TokenType::NUM) {
			output.push(TkBuffer->value);
		}

		else if (TypeBuffer == ep::TokenType::VARIABLE) {
			output.push(ep::Variable::get_value(TkBuffer->text[0])); // Get value of variable using text and Push
		}

		else if (TypeBuffer == ep::TokenType::OPERATOR) {

			b = output.top(); // Behind
			output.pop();
			a = output.top(); // Front
			output.pop();


			if (TkBuffer->id == 0) { // +
				output.push((long double) a + b);
			}

			else if (TkBuffer->id == 1) { // -
				output.push((long double) a - b);
			}

			else if (TkBuffer->id == 2) { // *
				output.push((long double) a * b);
			}

			else if (TkBuffer->id == 3) { // /
				output.push((long double)a / b);
			}

			else if (TkBuffer->id == 4) { // ^
				output.push((long double) pow(a, b));
			}
		}

		else if (TypeBuffer == ep::TokenType::FUNCTION) {
			a = output.top();
			output.pop();


			if (TkBuffer->function == ep::FunctionType::SIN) {
				output.push(sinl(a));
			}

			else if (TkBuffer->function == ep::FunctionType::COS) {
				output.push(cosl(a));
			}

			else if (TkBuffer->function == ep::FunctionType::TAN) {
				output.push(tanl(a));
			}

			else if (TkBuffer->function == ep::FunctionType::LOG) {
				output.push(log10l(a));
			}

			else if (TkBuffer->function == ep::FunctionType::LN) {
				output.push(logl(a));
			}
		}

		TokenQueue.pop();

		//std::stack<long double> copied = output;
		//int size = copied.size();
		//for (int j = 0; j < size; j++) {
		//	std::cout << copied.top() << ", ";
		//	copied.pop();
		//}
		//std::cout << ";" << std::endl;

	}

	return output.top();
}

long double ep::Expression::substitution(std::map<char, long double> VariableMap) {
	return 0.0l;
}


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

void ep::Variable::set_value(char character, long double value) {

	if (ep::VARIABLES.find(character) != ep::VARIABLES.end()) {

		ep::VARIABLES[character]->value = value;

	}

}

long double ep::Variable::get_value(char character) {
	
	if (ep::VARIABLES.find(character) != ep::VARIABLES.end()) {

		return ep::VARIABLES[character]->value;

	}

	return 0.0f;
}

long double ep::Variable::get_value(char character, long double DefaultValue) {
	
	if (ep::VARIABLES.find(character) != ep::VARIABLES.end()) {

		return ep::VARIABLES[character]->value;

	}

	return DefaultValue;
}

/* namespace: parse */

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
		character == '8' || character == '9' ||
		character == '.') {
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
		if (buffer == ' ') { continue; }

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
			ep::Token::auto_convert_type(&NewToken);
			output.push(NewToken);
			temp = "";
		}
		
		PrvType = type;
	}

	if (!temp.empty()) {

		ep::Token NewToken(PrvType, temp);
		ep::Token::auto_convert_type(&NewToken);
		output.push(NewToken);

	}
	return output;
}

ep::Expression ep::parse::parse_string(std::string ExpressionLikeString) {
	
	std::queue<ep::Token> output = {};
	std::stack<ep::Token> op = {}; // operator

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

			if (!op.empty() && op.top().type == ep::TokenType::FUNCTION) {
				output.push(op.top());
				op.pop();
			}
		}

		else if (buffer.type == ep::TokenType::OPERATOR || 
				 buffer.type == ep::TokenType::STRING) {
			while (
				!op.empty() &&
				op.top().type != ep::TokenType::BRACKET_OPEN &&
				((op.top().id / 2 > buffer.id / 2) ||
				(op.top().id / 2 == buffer.id / 2 && buffer.id != 4))) {
				output.push(op.top());
				op.pop();
			}
			op.push(buffer);
		}

		else if (buffer.type == ep::TokenType::FUNCTION) {
			op.push(buffer);
		}

		tokenized.pop();
	}

	while (!op.empty()) {
		output.push(op.top());
		op.pop();
	}

	//ep::parse::print_token_queue(output);

	return Expression(output);
}