export module bf:Lexer;

import :Token;
import std;

namespace bf
{
    export class Lexer
    {
        std::string_view _content;
        size_t _position;

    public:
        Lexer(const std::string_view& content)
            : _content(content), _position(0)
        {
        }

        /// <summary>
        /// Gets the next valid token.
        /// Discard all but the 8 allowed commands.
        /// </summary>
        /// <returns>
        /// The next valid token, or the EOF token
        /// if the is nothing left to parse.
        /// </returns>
        Token::Kind next();

        /// <summary>
        /// Parses the current lexer content.
        /// </summary>
        /// <param name="log">true to log parsed tokens.</param>
        /// <returns>A collection of parsed tokens.</returns>
        std::vector<Token> lex(bool log = false);
    };
}