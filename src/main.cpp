import std;
import bf;

int main(int argc, char* argv[])
{
    std::string_view content = bf::Examples::hello_world;
    if (argc > 1) {
        std::ifstream in(argv[1]);
        content = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    }
    // std::cout << "bf:\n" << content << "\n" << std::endl;

    auto lexer = bf::Lexer(content);
    const auto tokens = lexer.lex(false);

    auto interpreter = bf::Interpreter();
    interpreter.interpret(tokens);

    auto jit = bf::Jit();
    const auto code = jit.compile(tokens);

    auto exec = bf::Exec(code);
    exec.run();
}