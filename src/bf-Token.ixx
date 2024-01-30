export module bf:Token;

import std;

namespace bf
{
    export struct Token
    {
        enum class Kind
        {
            EOF = 0,
            INC = '+',
            DEC = '-',
            RIGHT = '>',
            LEFT = '<',
            STDOUT = '.',
            STDIN = ',',
            JZ = '[',
            JNZ = ']'
        };

        Kind kind;
        size_t operand;

        std::string to_string() const;

    private:
        static std::string token_kind_to_string(Token::Kind kind);
    };
}