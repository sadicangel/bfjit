module bf;

using namespace bf;

import std;

__declspec(noinline) static void asm_print(const char* memory)
{
    //std::printf(memory);
}

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
            // push rax
            code.push_back(0x50);
            //// push rbp
            //code.push_back(0x55);
            //// mov rbp, rsp
            //code.push_back(0x48);
            //code.push_back(0x89);
            //code.push_back(0xE5);
            //// sub rsp, 32 (shadow space)
            //code.push_back(0x48);
            //code.push_back(0x83);
            //code.push_back(0xEC);
            //code.push_back(0x30);
            // mov rax, qword &asm_print
            code.push_back(0x48);
            code.push_back(0xB8);
            const auto index = code.size();
            code.push_back(0);
            code.push_back(0);
            code.push_back(0);
            code.push_back(0);
            code.push_back(0);
            code.push_back(0);
            code.push_back(0);
            code.push_back(0);
            auto vptr = reinterpret_cast<std::intptr_t*>(&code[index]);
            *vptr = (std::intptr_t)&asm_print;
            // call rax
            code.push_back(0xFF);
            code.push_back(0xD0);
            // pop rax
            code.push_back(0x58);
            //// mov rsp, rbp
            //code.push_back(0x48);
            //code.push_back(0x89);
            //code.push_back(0xEC);
            //// pop rdp
            //code.push_back(0x5D);
#if _DEBUG
            std::ios_base::fmtflags f(std::cout.flags());
            std::cout << std::hex << *vptr << std::endl;
            std::cout.flags(f);
#endif
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

void Jit::dump(const std::size_t bytes_per_row)
{
    std::ios_base::fmtflags f(std::cout.flags());
    std::size_t i = 0;
    std::cout << std::hex << std::setfill('0');
    for (auto byte : code) {
        if (i != 0 && i % bytes_per_row == 0)
            std::cout << std::endl;
        std::cout << std::setw(2) << (int)byte << ' ';
        ++i;
    }
    std::cout.flags(f);
}