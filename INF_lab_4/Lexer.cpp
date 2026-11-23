#include <stdexcept>

#include "Lexer.h"

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;
    char c;

    while (pos < src.size()) {
        c = src[pos];
        
        if (c == '\n') {
            pos++;
            line++;
            continue;
        }

        if (c == ' ' || c == '\t' || c == '\r') {
            pos++;
            continue;
        }

        switch (c) {
        case '{': tokens.push_back({ TokenType::LBrace, "{", line }); pos++; break;
        case '}': tokens.push_back({ TokenType::RBrace, "}", line }); pos++; break;
        case '[': tokens.push_back({ TokenType::LBracket, "[", line }); pos++; break;
        case ']': tokens.push_back({ TokenType::RBracket, "]", line }); pos++; break;
        case ':': tokens.push_back({ TokenType::Colon, ":", line }); pos++; break;
        case ',': tokens.push_back({ TokenType::Comma, ",", line }); pos++; break;

        case '"': tokens.push_back(readString()); break;
        case 't': tokens.push_back(readKeyword("true", TokenType::True)); break;
        case 'f': tokens.push_back(readKeyword("false", TokenType::False)); break;
        case 'n': tokens.push_back(readKeyword("null", TokenType::Null)); break;

        default:
        {
            if (c == '-' || isDigit(c)) tokens.push_back(readNumber());
            else throw std::runtime_error("Unexpected character at line " + std::to_string(line));
            break;
        }
        }
    }
    tokens.push_back({ TokenType::End, "", line });

    return tokens;
}

Token Lexer::readString()
{
    pos++;

    std::string res;
    char c;

    while (pos < src.size()) {
        c = src[pos++];

        if (c == '"') return { TokenType::String, std::move(res), line };
        if (c == '\n' || c == '\r') throw std::runtime_error("Newline inside string at line " + std::to_string(line));
        if (c == '\\') {
            if (pos < src.size()) {
                c = src[pos++];
                switch (c)
                {
                case '"': { res += '"'; break; }
                case '\\': { res += '\\'; break; }
                case '/': { res += '/'; break; }
                case 'n': { res += '\n'; break; }
                case 't': { res += '\t'; break; }
                case 'r': { res += '\r'; break; }
                case 'b': { res += '\b'; break; }
                case 'f': { res += '\f'; break; }
                default: throw std::runtime_error("Unknown escape sequence at line " + std::to_string(line));
                }
            }
            else throw std::runtime_error("Incorrect file completion");
        }
        else res.push_back(c);
    }
    throw std::runtime_error("Unterminated string at line " + std::to_string(line));
}

Token Lexer::readNumber()
{
    size_t start = pos;
    if (src[pos] == '-') {
        pos++;
        if (pos >= src.size() || !isDigit(src[pos]))  throw std::runtime_error("Single '-' in the int type at line " + std::to_string(line));
    }
    if (src[pos] == '0') {
        pos++;
        if (pos < src.size() && isDigit(src[pos])) throw std::runtime_error("Incorrect entry with a zero " + std::to_string(line));
    }

    bool have_dot = false;
    while (pos < src.size() && (src[pos] == '.' || isDigit(src[pos]))) {
        if (src[pos] == '.') {
            if(have_dot) throw std::runtime_error("Incorrect fractional number entry " + std::to_string(line));
            have_dot = true;
            pos++;
            if (pos >= src.size() || !isDigit(src[pos])) throw std::runtime_error("Incorrect fractional number entry " + std::to_string(line));
        }
        pos++;
    }

    return { TokenType::Number, src.substr(start, pos - start), line };
}

Token Lexer::readKeyword(const std::string& kw, TokenType type)
{
    if (src.compare(pos, kw.size(), kw) != 0 || (pos + kw.size() < src.size() && std::isalnum(static_cast<unsigned char>(src[pos + kw.size()])))) {
        throw std::runtime_error("Invalid keyword at line" + std::to_string(line));
    }

    pos += kw.size();

    return { type, kw, line };
}
