module bf;

using namespace bf;

Interpreter::Interpreter(const Lexer& lexer)
    : _lexer(lexer), _memory(), _mp(0), _ip(0)
{
}

void Interpreter::interpret()
{
    _memory.clear();
    _memory.push_back(0);
    _mp = 0;
    _ip = 0;
    while (_ip < _lexer.tokens.size()) {
        const auto& token = _lexer.tokens[_ip];
        switch (token.kind)
        {
        
        case Token::Kind::INC: {
            _memory[_mp] += token.operand;
            ++_ip;
        } break;
        
        case Token::Kind::DEC: {
            _memory[_mp] -= token.operand;
            ++_ip;
        } break;
        
        case Token::Kind::LEFT: {
            if (_ip < token.operand) {
                throw std::underflow_error("memory underflow");
            }
            _mp -= token.operand;
            ++_ip;
        } break;
        
        case Token::Kind::RIGHT: {
            _mp += token.operand;
            while (_mp >= _memory.size()) {
                _memory.push_back(0);
            }
            ++_ip;
        } break;

        case Token::Kind::STDIN: {
            for (size_t i = 0; i < token.operand; ++i) {
                std::copy_n(std::istream_iterator<char>(std::cin), token.operand, _memory.begin());
            }
            ++_ip;
        } break;

        case Token::Kind::STDOUT: {
            for (size_t i = 0; i < token.operand; ++i) {
                std::cout << static_cast<char>(_memory[_mp]);
            }
            ++_ip;
        } break;

        case Token::Kind::JZ: {
            if (_memory[_mp] == 0) {
                _ip = token.operand;
            }
            else {
                _ip += 1;
            }
        } break;

        case Token::Kind::JNZ: {
            if (_memory[_mp] != 0) {
                _ip = token.operand;
            }
            else {
                _ip += 1;
            }
        } break;
        }
    }
}