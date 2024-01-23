module;
#if _WIN32 // Windows - Use VirtualAlloc
#include "Windows.h"
#else // Use mmap
#error platform not supported
#endif
module bf;

using namespace bf;

namespace bf
{
    Exec::Exec(const Jit jit)
        : _jit(jit), _exec(nullptr)
    {
        _exec = VirtualAlloc(nullptr, jit.code.size(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (_exec == nullptr) {
            throw std::runtime_error(GetLastErrorAsString());
        }

        _jit.backpatch_calls((std::intptr_t)_exec);

        memcpy(_exec, jit.code.data(), jit.code.size());
    }

    Exec::~Exec()
    {
        if (_exec != nullptr) {
            VirtualFree(_exec, _jit.code.size(), MEM_RELEASE);
        }
    }

    Exec::Exec(Exec&& other) noexcept
        : _jit(std::move(other._jit)), _exec(std::exchange(other._exec, nullptr))
    {
    }

    Exec& Exec::operator=(Exec&& other) noexcept
    {
        if (&other == this) {
            return *this;
        }

        std::swap(_jit, other._jit);
        std::swap(_exec, other._exec);
        return *this;
    }

    void Exec::run()
    {
        DWORD protect = 0;
        const auto error = VirtualProtect(_exec, _jit.code.size(), PAGE_EXECUTE, &protect);
        if (error == ERROR) {
            throw std::runtime_error(GetLastErrorAsString());
        }

        std::vector<unsigned char> mem(10 * 1024 * 1024);

        ((void(*)(void*))_exec)(mem.data());

        VirtualProtect(_exec, _jit.code.size(), protect, &protect);
    }

    std::string Exec::GetLastErrorAsString()
    {
        DWORD error_jit = GetLastError();
        if (error_jit == 0) {
            return "unknown error";
        }

        LPSTR buffer = nullptr;

        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, error_jit, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buffer, 0, NULL);

        std::string message(buffer, size);

        LocalFree(buffer);

        return message;
    }
}