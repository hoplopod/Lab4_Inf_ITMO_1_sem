#pragma once

#include <vector>
#include <stdexcept>

#include "Token.h"

class Lexer {
public:

	explicit Lexer(const std::string& text) : src(text) {};

	std::vector<Token> tokenize();

private:
	const std::string& src;

	size_t pos = 0;
	size_t line = 1;

	Token readString();
	Token readNumber();
	Token readKeyword(const std::string& kw, TokenType type);

	static bool isDigit(char c) {
		return std::isdigit(static_cast<unsigned char>(c)) != 0;
	}
};