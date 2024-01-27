#include "Windows.h"

import std;
import bf;

int main(int argc, char* argv[])
{
    std::string_view content = ",.";//bf::Examples::alphabet;
    if (argc > 1) {
        std::ifstream in(argv[1]);
        content = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    }
    // std::cout << "bf:\n" << content << "\n" << std::endl;

    auto lexer = bf::Lexer(content);
    lexer.lex(false);

    auto interpreter = bf::Interpreter(lexer);
    interpreter.interpret();

    std::cout << std::endl;

    auto jit = bf::Jit(lexer);
    jit.compile();

    auto exec = bf::Exec(jit);
    exec.run();
}