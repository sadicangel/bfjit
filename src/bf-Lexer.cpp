module bf;

import std;

using namespace bf;

Token::Kind Lexer::next() {
    // Skip non valid tokens.
    while (_position < _content.length() && tokens.find(_content[_position]) == std::string_view::npos) {
        ++_position;
    }

    if (_position >= _content.length())
        return Token::Kind::EOF;

    return static_cast<Token::Kind>(_content[_position++]);
}

std::vector<Token> Lexer::lex() {
    std::stack<size_t> addresses{};
    std::vector<Token> tokens{};
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
                // TODO: Make this pretty.
                std::string msg = "[" + _position;
                msg += "] unbalanced loop\n";
                throw std::domain_error(msg);
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
    return tokens;
}