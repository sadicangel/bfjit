import std;
import bf;

static constexpr std::string_view content = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";

int main()
{
    auto lexer = bf::Lexer(content);
    const auto tokens = lexer.lex();

    for(size_t i = 0; i < tokens.size(); ++i) {
        std::cout << i << ": " << tokens[i].to_string() << std::endl;
    }
}