module;
#if _WIN32
#include "Windows.h"
#else
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
    : _lexer(lexer), code(), memory_size_required(1)
{
}

void Jit::compile()
{
    code.clear();
    memory_size_required = 1;

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
            const auto ptr = reinterpret_cast<std::int32_t*>(&code[code.size() - sizeof(std::int32_t)]);
            *ptr = static_cast<std::int32_t>(token.operand);
            memory_size_required -= token.operand;
        } break;
        case Token::Kind::STDOUT: {
            for (std::size_t i = 0; i < token.operand; ++i) {
                // push rcx
                code.push_back(0x51);
                // push rbx
                code.push_back(0x53);
                // sub rsp, 48
                code.insert(code.end(), { 0x48, 0x83, 0xEC, 0x30 });
                // mov rbx, rcx
                code.insert(code.end(), { 0x48, 0x89, 0xCB });
                // mov ecx, STD_OUTPUT_HANDLE
                code.insert(code.end(), { 0xB9, 0x00, 0x00, 0x00, 0x00 });
                const auto std_output_handle = reinterpret_cast<std::int32_t*>(&code[code.size() - sizeof(std::uint32_t)]);
                *std_output_handle = STD_OUTPUT_HANDLE;
                // mov rax, qword &GetStdHandle
                code.insert(code.end(), { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
                const auto get_std_handle = reinterpret_cast<std::intptr_t*>(&code[code.size() - sizeof(std::intptr_t)]);
                *get_std_handle = (std::intptr_t)&GetStdHandle;
                // call rax
                code.insert(code.end(), { 0xFF, 0xD0 });
                // mov rcx, rax
                code.insert(code.end(), { 0x48, 0x89, 0xC1 });
                // mov rdx, rbx 
                code.insert(code.end(), { 0x48, 0x89, 0xDA });
                // mov r8, 1
                code.insert(code.end(), { 0x49, 0xC7, 0xC0, 0x01, 0x00, 0x00, 0x00 });
                // lea r9, [rsp + 40]
                code.insert(code.end(), { 0x4C, 0x8D, 0x4C, 0x24, 0x28 });
                // mov qword [rsp + 32], 0
                code.insert(code.end(), { 0x48, 0xC7, 0x44, 0x24, 0x20, 0x00, 0x00, 0x00, 0x00 });
                // mov rax, qword &WriteConsoleA
                code.insert(code.end(), { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
                const auto write_console = reinterpret_cast<std::intptr_t*>(&code[code.size() - sizeof(std::intptr_t)]);
                *write_console = (std::intptr_t)&WriteConsoleA;
                // call rax
                code.insert(code.end(), { 0xFF, 0xD0 });
                // add rsp, 48
                code.insert(code.end(), { 0x48, 0x83, 0xC4, 0x30 });
                // pop rbx
                code.push_back(0x5B);
                // pop rcx 
                code.push_back(0x59);
            }
        } break;
        case Token::Kind::STDIN: {
            for (std::size_t i = 0; i < token.operand; ++i) {
                // push rcx
                code.push_back(0x51);
                // push rbx
                code.push_back(0x53);
                // sub rsp, 48
                code.insert(code.end(), { 0x48, 0x83, 0xEC, 0x30 });
                // mov rbx, rcx
                code.insert(code.end(), { 0x48, 0x89, 0xCB });
                // mov ecx, STD_INPUT_HANDLE
                code.insert(code.end(), { 0xB9, 0x00, 0x00, 0x00, 0x00 });
                const auto std_output_handle = reinterpret_cast<std::int32_t*>(&code[code.size() - sizeof(std::uint32_t)]);
                *std_output_handle = STD_INPUT_HANDLE;
                // mov rax, qword &GetStdHandle
                code.insert(code.end(), { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
                const auto get_std_handle = reinterpret_cast<std::intptr_t*>(&code[code.size() - sizeof(std::intptr_t)]);
                *get_std_handle = (std::intptr_t)&GetStdHandle;
                // call rax
                code.insert(code.end(), { 0xFF, 0xD0 });
                // mov rcx, rax
                code.insert(code.end(), { 0x48, 0x89, 0xC1 });
                // mov rdx, rbx 
                code.insert(code.end(), { 0x48, 0x89, 0xDA });
                // mov r8, 1
                code.insert(code.end(), { 0x49, 0xC7, 0xC0, 0x01, 0x00, 0x00, 0x00 });
                // lea r9, [rsp + 40]
                code.insert(code.end(), { 0x4C, 0x8D, 0x4C, 0x24, 0x28 });
                // mov qword [rsp + 32], 0
                code.insert(code.end(), { 0x48, 0xC7, 0x44, 0x24, 0x20, 0x00, 0x00, 0x00, 0x00 });
                // mov rax, qword &ReadConsoleA
                code.insert(code.end(), { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
                const auto write_console = reinterpret_cast<std::intptr_t*>(&code[code.size() - sizeof(std::intptr_t)]);
                *write_console = (std::intptr_t)&ReadConsoleA;
                // call rax
                code.insert(code.end(), { 0xFF, 0xD0 });
                // add rsp, 48
                code.insert(code.end(), { 0x48, 0x83, 0xC4, 0x30 });
                // pop rbx
                code.push_back(0x5B);
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