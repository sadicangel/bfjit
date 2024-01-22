module bf;

import std;

using namespace bf;

std::string Token::to_string() const
{
    return token_kind_to_string(kind) + " (" + std::to_string(operand) + ")";
}

std::string Token::token_kind_to_string(Token::Kind kind)
{
    switch (kind)
    {
    case Token::Kind::EOF:
        return "EOF";
    case Token::Kind::INC:
        return "INC";
    case Token::Kind::DEC:
        return "DEC";
    case Token::Kind::LEFT:
        return "LEFT";
    case Token::Kind::RIGHT:
        return "RIGHT";
    case Token::Kind::IN:
        return "IN";
    case Token::Kind::OUT:
        return "OUT";
    case Token::Kind::JZ:
        return "JZ";
    case Token::Kind::JNZ:
        return "JNZ";
    default:
        return "";
    }
}