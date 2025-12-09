#pragma once
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <stack>

namespace ep {

	/* Token */

	enum class TokenType {
		NUM, STRING, OPERATOR, BRACKET_OPEN, BRACKET_CLOSE,
		VARIABLE, FUNCTION, NONE
	};

	enum class FunctionType {
		SIN, COS, TAN, LOG, LN, NONE,
	};

	static std::map<std::string, double> CONSTANTS = {
		{"pi", 3.1415926535897932384626l},
		{"e",  2.7182818284590452353602l},
	};

	void create_constant(std::string name, double value);

	class Token {
	public:
		ep::TokenType type;
		ep::FunctionType function;
		std::string text;
		double value;
		int id;
		/*  ^~~ Sub ID of Token
		*		<OPERATOR> ( id // 2 -> operator level )
		*			{ 0: +, 1: -, 2: *, 3: /, 4: ^ }
		*/

		Token();
		Token(TokenType type, std::string text, int id); // for operators and variables
		Token(TokenType type, std::string text); // for string
		Token(TokenType type, double value); // for num

		void print();

		static void auto_convert_type(ep::Token* token);
	};

	/* Expression */

	class Expression {
		public:
			std::queue<ep::Token> TokenQueue;

			Expression(std::queue<ep::Token> TokenQueue);

			void simplize();

			double substitution();

			double substitution(std::map<char, double> VariableMap);

	};

	/* Variable */

	class Variable; // for pointer

	static std::map<char, Variable*> VARIABLES = {};

	class Variable {
		public:
			char character;
			double value = 0.0f;
			double derivative = 0.0f;

			Variable(char character);

			/* static methods */
			static Variable create_variable(char character);
			static void set_value(char character, double value);
			static double get_value(char character);
			static double get_value(char character, double DefaultValue);
	};


	/* parse */

	namespace parse {
		/* parse */
		void print_token_queue(std::queue<Token> TokenQueue);

		ep::TokenType get_type(char character);

		std::queue<ep::Token> tokenize(std::string ExpressionLikeString);
		
		ep::Expression parse_string(std::string ExpressionLikeStriing);
	}
}

#endif // !EXPRESSION_H
