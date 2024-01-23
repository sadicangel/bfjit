export module bf:Jit;

import std;
import :Lexer;
import :Token;

namespace bf
{
    export class Jit
    {
        Lexer _lexer;
        std::vector<std::size_t> _print_indices;
    public:
        Jit(const Lexer& lexer) :
            _lexer(lexer), code(), _print_indices()
        {

        }

        std::vector<std::uint8_t> code;

        void compile();

        void backpatch_calls(const std::intptr_t start);
    };
}