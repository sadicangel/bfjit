module bf;

using namespace bf;

import std;

void Jit::compile()
{
    _print_indices.clear();
    code.clear();

    for (const auto& token : _lexer.tokens) {
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
            // call print
            code.push_back(0xE8);
            _print_indices.push_back(code.size());
            code.push_back(0);
            code.push_back(0);
            code.push_back(0);
            code.push_back(0);
        } break;
        case Token::Kind::JZ: {
        } break;
        case Token::Kind::JNZ: {
        } break;
        }
    }

    // ret
    code.push_back(0xC3);
}

static void asm_print(const char* memory)
{
    std::printf("%c", memory[0]);
}

void Jit::backpatch_calls(const std::intptr_t start)
{
    asm_print("C");
    const void* fptr = &std::printf;
    const std::intptr_t iptr = (std::intptr_t)fptr;
    for (const auto& addr : _print_indices)
    {
        const std::intptr_t offset = start + static_cast<std::intptr_t>(addr);
        const std::intptr_t rel32 = iptr - offset + 1;

        code[addr + 0] = static_cast<std::uint8_t>((rel32 & 0x000000ff));
        code[addr + 1] = static_cast<std::uint8_t>((rel32 & 0x0000ff00) >> 8);
        code[addr + 2] = static_cast<std::uint8_t>((rel32 & 0x00ff0000) >> 16);
        code[addr + 3] = static_cast<std::uint8_t>((rel32 & 0xff000000) >> 24);
    }
}