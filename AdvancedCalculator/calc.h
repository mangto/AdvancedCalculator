#pragma once
#ifndef CALC_H
#define CALC_H

#include <iostream>
#include <vector>
#include <stack>
#include <string>

#define PI 3.1415926535897932384626l
#define E  2.7182818284590452353602l

/* Token Class */


class Token {
	public:
		int type; // 1: num, 2: string, 3: bracket_open, 4: bracket_close, 5: operator
		std::string character;
		double value;

		Token();
		Token(double value);
		Token(std::string value);

		void print();
};

namespace Calc {

	void print_token_vector(std::vector<Token> TokenVector);

	/* Utils Functions */

	bool is_digit(std::string DigitLikeString);

	bool is_digit(char DigitLikeChar);

	int digit_to_int(std::string DigitLikeString);

	int digit_to_int(char DigitLikeChar);

	double string_to_num(std::string NumLikeString);

	std::vector<Token> tokenize(std::string NumLikeString);

	void print_token_stack(std::stack<Token> TokenStack);

	int get_operator_level(Token tk);

	std::vector<Token> parse(std::vector<Token> TokenVector);

	double calculate_parsed(std::vector<Token> ParsedTokenVector);
}
#endif // !CALC_H
