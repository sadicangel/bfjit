export module bf:Jit;

import std;
import :Lexer;
import :Token;

namespace bf
{
    export class Jit
    {
        Lexer _lexer;
    public:
        Jit(const Lexer& lexer);

        std::vector<std::uint8_t> code;
        size_t memory_size_required;

        void compile();

        void dump(const std::size_t bytes_per_row = 16);
    };
}