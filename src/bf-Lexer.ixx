export module bf:Lexer;

import :Token;
import std;

namespace bf
{
    export class Lexer {
        static constexpr std::string_view tokens = "+-<>.,[]";
        std::string_view _content;
        size_t _position;

    public:
        Lexer(const std::string_view& content)
            : _content(content), _position(0)
        {
        }

        Token::Kind next();
        std::vector<Token> lex();
    };
}