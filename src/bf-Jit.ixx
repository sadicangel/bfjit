export module bf:Jit;

import std;
import :Token;

namespace bf
{
    export class Jit
    {

    public:
        std::vector<std::uint8_t> compile(const std::vector<Token>& tokens);
    };
}