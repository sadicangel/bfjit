module bf;

import std;

using namespace bf;

static constexpr std::string_view Allowed_Tokens = "+-<>.,[]";

bf::Lexer::Lexer(const std::string_view& content)
    : _content(content), _position(0), tokens(0)
{
}

Token::Kind Lexer::next()
{
    // Skip non valid tokens.
    while (_position < _content.length() && Allowed_Tokens.find(_content[_position]) == std::string_view::npos) {
        ++_position;
    }

    if (_position >= _content.length())
        return Token::Kind::EOF;

    return static_cast<Token::Kind>(_content[_position++]);
}

void Lexer::lex(bool log)
{
    tokens.clear();
    _position = 0;
    std::stack<size_t> addresses{};
    auto k = next();
    while (k != Token::Kind::EOF) {
        switch (k)
        {
        case Token::Kind::INC:
        case Token::Kind::DEC:
        case Token::Kind::RIGHT:
        case Token::Kind::LEFT:
        case Token::Kind::STDIN:
        case Token::Kind::STDOUT: {
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
}
