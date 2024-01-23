export module bf:Interpreter;

import :Lexer;

import std;

namespace bf
{
    /// <summary>
    /// Brainfuck interpreter.
    /// </summary>
    export class Interpreter
    {
        Lexer _lexer;
        std::vector<size_t> _memory;
        size_t _mp;
        size_t _ip;
    
    public:

        Interpreter(const Lexer& lexer);

        /// <summary>
        /// Interpret a collection of tokens.
        /// </summary>
        void interpret();
    };
}
