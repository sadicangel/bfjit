module bf;

using namespace bf;

import std;

std::vector<std::uint8_t> Jit::compile(const std::vector<Token>& tokens)
{
    std::vector<std::uint8_t> code{};

    for (const auto& token : tokens) {
        switch (token.kind)
        {
        case Token::Kind::INC: {
            // add byte[rcx], <operand>
            code.push_back(0x80);
            code.push_back(0x01);
            code.push_back(static_cast<std::uint8_t>(token.operand & 0xFF));
        } break;
        case Token::Kind::DEC: {
            // sub byte[rcx], <operand>
            code.push_back(0x80);
            code.push_back(0x29);
            code.push_back(static_cast<std::uint8_t>(token.operand & 0xFF));
        } break;
        case Token::Kind::RIGHT: {
            // add rcx, <operand>
            code.push_back(0x48);
            code.push_back(0x81);
            code.push_back(0xC1);
            code.push_back(static_cast<std::uint8_t>((token.operand & 0x000000ff)));
            code.push_back(static_cast<std::uint8_t>((token.operand & 0x0000ff00) >> 8));
            code.push_back(static_cast<std::uint8_t>((token.operand & 0x00ff0000) >> 16));
            code.push_back(static_cast<std::uint8_t>((token.operand & 0xff000000) >> 24));
        } break;
        case Token::Kind::LEFT: {
            // sub rcx, <operand>
            code.push_back(0x48);
            code.push_back(0x81);
            code.push_back(0xE9);
            code.push_back(static_cast<std::uint8_t>((token.operand & 0x000000ff)));
            code.push_back(static_cast<std::uint8_t>((token.operand & 0x0000ff00) >> 8));
            code.push_back(static_cast<std::uint8_t>((token.operand & 0x00ff0000) >> 16));
            code.push_back(static_cast<std::uint8_t>((token.operand & 0xff000000) >> 24));
        } break;
        case Token::Kind::IN: {
        } break;
        case Token::Kind::OUT: {
        } break;
        case Token::Kind::JZ: {
        } break;
        case Token::Kind::JNZ: {
        } break;
        }
    }

    // ret
    code.push_back(0xC3);

    return code;
}