#include "Windows.h"

import std;
import bf;

//void print() {
//    char f[10]{};
//    f[0] = 67;
//    std::printf("%c", f[0]);
//}

int main(int argc, char* argv[])
{
    std::string_view content = bf::Examples::hello_world;
    if (argc > 1) {
        std::ifstream in(argv[1]);
        content = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    }
    // std::cout << "bf:\n" << content << "\n" << std::endl;

    auto lexer = bf::Lexer(content);
    lexer.lex(false);

    auto interpreter = bf::Interpreter(lexer);
    interpreter.interpret();

    auto jit = bf::Jit(lexer);
    jit.compile();

    auto exec = bf::Exec(jit);
    exec.run();
}