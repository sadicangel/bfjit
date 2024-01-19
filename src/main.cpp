import std;
import bf;

static constexpr std::string_view content = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";

int main()
{
    auto lexer = bf::Lexer(content);
    const auto tokens = lexer.lex(false);

    auto interpreter = bf::Interpreter();
    interpreter.interpret(tokens);
}