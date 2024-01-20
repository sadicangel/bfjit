export module bf:Interpreter;

import :Token;

import std;

namespace bf
{
    /// <summary>
    /// Brainfuck interpreter.
    /// </summary>
    export class Interpreter {
        std::vector<size_t> _memory;
        size_t _mp;
        size_t _ip;
    
    public:

        Interpreter()
            : _memory(), _mp(0), _ip(0)
        {
        }

        /// <summary>
        /// Interpret a collection of tokens.
        /// </summary>
        /// <param name="tokens">A collection of parsed tokens to interpret.</param>
        void interpret(const std::vector<Token>& tokens);
    };
}
