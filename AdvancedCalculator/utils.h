#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <stack>


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

std::stack<Token> parse(std::vector<Token> TokenVector); 

#endif // !UTILS_H
