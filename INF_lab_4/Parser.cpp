#include <stdexcept>

#include "Parser.h"


const Token& Parser::shift() 
{
	const Token& tok = tokens[pos];
	if (tok.type != TokenType::End) pos++;
	return tok;
}

const Token& Parser::expect(TokenType type, const std::string& msg) 
{
	if (tokens[pos].type != type) {
		throw std::runtime_error(msg + " at line " + std::to_string(tokens[pos].line));
	}
	return shift();
}

Value Parser::parse() 
{
	Value root = parseValue();
	expect(TokenType::End, "Unexpected tokens after json root");
	return root;
}

Value Parser::parseValue()
{
	switch (tokens[pos].type) {
	case TokenType::LBrace: return parseObject();
	case TokenType::LBracket: return parseArray();
	case TokenType::String: return Value(shift().value);
	case TokenType::True: { shift(); return Value(true); }
	case TokenType::False: { shift(); return Value(false); }
	case TokenType::Null: { shift(); return Value(); }
	case TokenType::Number: {
		std::string num = shift().value;
		if (num.find('.') != std::string::npos) return Value(std::stod(num));
		return Value(static_cast<int64_t>(std::stoll(num)));
	}
	default: throw std::runtime_error("Unexpected token '" + tokens[pos].value + "' at line " + std::to_string(tokens[pos].line));
	}
}

Value Parser::parseArray() 
{
	expect(TokenType::LBracket, "Expected '['");
	Array arr;

	if (tokens[pos].type == TokenType::RBracket) {
		shift();
		return Value(std::move(arr));
	}

	while (true) {
		arr.push_back(parseValue());

		if (tokens[pos].type == TokenType::Comma) shift();
		else break;
	}

	expect(TokenType::RBracket, "Expected ']'");
	return Value(std::move(arr));
}

Value Parser::parseObject() 
{
	expect(TokenType::LBrace, "Expected '{'");
	Object obj;

	if (tokens[pos].type == TokenType::RBrace) {
		shift();
		return Value(std::move(obj));
	}

	while (true) {
		std::string key = expect(TokenType::String, "Expected string key").value;
		expect(TokenType::Colon, "Expected ':' after key");
		Value val = parseValue();

		obj.emplace_back(std::move(key), std::move(val));

		if (tokens[pos].type == TokenType::Comma) shift();
		else break;
	}

	expect(TokenType::RBrace, "Expected '}'");
	return Value(std::move(obj));
}