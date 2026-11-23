#pragma once

#include <vector>
#include <string>

#include "Token.h"
#include "Value.h"

class Parser {
public:

	explicit Parser(const std::vector<Token>& tokens) : tokens(tokens) {};

	Value parse();

private:
	const std::vector<Token>& tokens;
	size_t pos = 0;

	const Token& shift();
	const Token& expect(TokenType type, const std::string& msg);

	Value parseValue();
	Value parseObject();
	Value parseArray();
};