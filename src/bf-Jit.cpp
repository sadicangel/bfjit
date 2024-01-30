module;
#if !_WIN32
#error platform not supported
#endif
module bf;

using namespace bf;

import std;

struct Jump
{
    std::size_t src_index;
    std::size_t operand_index;
    std::size_t operand_value;
};

Jit::Jit(const Lexer& lexer)
    : _lexer(lexer), code(), memory_size_required(0)
{
}

void Jit::compile()
{
    code.clear();
    memory_size_required = 0;

    // token index -> code index
    std::vector<std::size_t> addresses{};
    std::vector<Jump> jumps{};

    for (const auto& token : _lexer.tokens) {
        addresses.push_back(code.size());
        switch (token.kind)
        {
        case Token::Kind::INC: {
            // add byte[rcx], <operand>
            code.insert(code.end(), { 0x80, 0x01, static_cast<std::uint8_t>(token.operand & 0xFF) });
        } break;
        case Token::Kind::DEC: {
            // sub byte[rcx], <operand>
            code.insert(code.end(), { 0x80, 0x29, static_cast<std::uint8_t>(token.operand & 0xFF) });
        } break;
        case Token::Kind::RIGHT: {
            // add rcx, <operand>
            code.insert(code.end(), { 0x48, 0x81, 0xC1, 0x00, 0x00, 0x00, 0x00 });
            auto ptr = reinterpret_cast<std::int32_t*>(&code[code.size() - sizeof(std::int32_t)]);
            *ptr = static_cast<std::int32_t>(token.operand);
            memory_size_required += token.operand;
        } break;
        case Token::Kind::LEFT: {
            // sub rcx, <operand>
            code.insert(code.end(), { 0x48, 0x81, 0xE9, 0x00, 0x00, 0x00, 0x00 });
            auto ptr = reinterpret_cast<std::int32_t*>(&code[code.size() - sizeof(std::int32_t)]);
            *ptr = static_cast<std::int32_t>(token.operand);
            memory_size_required -= token.operand;
        } break;
        case Token::Kind::OUT: {
            for (std::size_t i = 0; i < token.operand; ++i) {
                // push rcx
                code.push_back(0x51);
                // sub rsp, 56
                code.insert(code.end(), { 0x48, 0x83, 0xEC, 0x38 });
                // mov rax, qword &Win32Interop::write
                code.insert(code.end(), { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
                auto ptr = reinterpret_cast<std::intptr_t*>(&code[code.size() - sizeof(std::intptr_t)]);
                *ptr = (std::intptr_t)&Win32Interop::write;
                // call rax
                code.insert(code.end(), { 0xFF, 0xD0 });
                // add rsp, 56
                code.insert(code.end(), { 0x48, 0x83, 0xC4, 0x38 });
                // pop rcx 
                code.push_back(0x59);
            }
        } break;
        case Token::Kind::IN: {
            for (std::size_t i = 0; i < token.operand; ++i) {
                // push rcx
                code.push_back(0x51);
                // sub rsp, 56
                code.insert(code.end(), { 0x48, 0x83, 0xEC, 0x38 });
                // mov rax, qword &Win32Interop::read
                code.insert(code.end(), { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
                auto ptr = reinterpret_cast<std::intptr_t*>(&code[code.size() - sizeof(std::intptr_t)]);
                *ptr = (std::intptr_t)&Win32Interop::read;
                // call rax
                code.insert(code.end(), { 0xFF, 0xD0 });
                // add rsp, 56
                code.insert(code.end(), { 0x48, 0x83, 0xC4, 0x38 });
                // pop rcx 
                code.push_back(0x59);
            }
        } break;
        case Token::Kind::JZ: {
            // mov al, byte[rcx]
            code.insert(code.end(), { 0x8A, 0x01 });
            // test al, al
            code.insert(code.end(), { 0x84, 0xC0 });
            // jz qword <dst_addr - src_addr>
            code.insert(code.end(), { 0x0F, 0x84, 0x00, 0x00, 0x00, 0x00 });

            jumps.push_back({
                .src_index = code.size(),
                .operand_index = code.size() - sizeof(std::int32_t),
                .operand_value = token.operand,
                });
        } break;
        case Token::Kind::JNZ: {
            // mov al, byte[rcx]
            code.insert(code.end(), { 0x8A, 0x01 });
            // test al, al
            code.insert(code.end(), { 0x84, 0xC0 });
            // jnz qword <dst_addr - src_addr>
            code.insert(code.end(), { 0x0F, 0x85, 0x00, 0x00, 0x00, 0x00 });

            jumps.push_back({
                .src_index = code.size(),
                .operand_index = code.size() - sizeof(std::int32_t),
                .operand_value = token.operand,
                });
        } break;
        }
    }
    // ret
    addresses.push_back(_lexer.tokens.size());
    code.push_back(0xC3);

    // Backpatch jumps.
    for (auto& jump : jumps) {
        auto src = static_cast<std::int32_t>(jump.src_index);
        auto dst = static_cast<std::int32_t>(addresses[jump.operand_value]);
        auto ptr = reinterpret_cast<std::int32_t*>(&code[jump.operand_index]);
        *ptr = dst - src;
    }
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