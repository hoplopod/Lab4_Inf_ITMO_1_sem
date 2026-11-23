#pragma once

#include <string>

enum class TokenType {
	LBrace, // {
	RBrace, // }
	LBracket, // [
	RBracket, // ]
	Colon, // :
	Comma, // ,

	String,
	Number,
	True,
	False,
	Null,

	End
};

struct Token {

	TokenType type;
	std::string value;
	size_t line;
};
