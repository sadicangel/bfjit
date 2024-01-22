module bf;

import std;

using namespace bf;

static constexpr std::string_view tokens = "+-<>.,[]";

Token::Kind Lexer::next()
{
    // Skip non valid tokens.
    while (_position < _content.length() && tokens.find(_content[_position]) == std::string_view::npos) {
        ++_position;
    }

    if (_position >= _content.length())
        return Token::Kind::EOF;

    return static_cast<Token::Kind>(_content[_position++]);
}

std::vector<Token> Lexer::lex(bool log)
{
    std::stack<size_t> addresses{};
    std::vector<Token> tokens{};
    _position = 0;
    auto k = next();
    while (k != Token::Kind::EOF) {
        switch (k)
        {
        case Token::Kind::INC:
        case Token::Kind::DEC:
        case Token::Kind::RIGHT:
        case Token::Kind::LEFT:
        case Token::Kind::IN:
        case Token::Kind::OUT: {
            size_t count = 1;
            auto s = next();
            while (s == k) {
                count++;
                s = next();
            }
            auto token = Token{
                .kind = k,
                .operand = count
            };
            tokens.push_back(token);
            k = s;
        } break;

        case Token::Kind::JZ: {
            auto token = Token{
                .kind = k,
                .operand = 0
            };
            addresses.push(tokens.size());
            tokens.push_back(token);
            k = next();
        } break;
        case Token::Kind::JNZ: {
            if (addresses.size() == 0) {
                throw std::domain_error("[" + std::to_string(_position) + "] unbalanced loop\n");
            }

            auto addr = addresses.top();
            auto token = Token{
                .kind = k,
                .operand = addr + 1
            };
            addresses.pop();
            tokens.push_back(token);
            tokens[addr].operand = tokens.size();

            k = next();
        } break;
        }
    }
    if (log) {
        for (size_t i = 0; i < tokens.size(); ++i) {
            std::cout << i << ": " << tokens[i].to_string() << '\n';
        }
    }
    return tokens;
}
