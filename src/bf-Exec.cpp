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
    Exec::Exec(const std::vector<std::uint8_t>& code)
        : _code(code), _exec(nullptr)
    {
        _exec = VirtualAlloc(nullptr, code.size(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (_exec == nullptr) {
            throw std::runtime_error(GetLastErrorAsString());
        }
        memcpy(_exec, code.data(), code.size());
    }

    Exec::~Exec()
    {
        if (_exec != nullptr) {
            VirtualFree(_exec, _code.size(), MEM_RELEASE);
        }
    }

    Exec::Exec(Exec&& other) noexcept
        : _code(std::move(other._code)), _exec(std::exchange(other._exec, nullptr))
    {
    }

    Exec& Exec::operator=(Exec&& other) noexcept
    {
        if (&other == this) {
            return *this;
        }

        std::swap(_code, other._code);
        std::swap(_exec, other._exec);
        return *this;
    }

    void Exec::run()
    {
        DWORD protect = 0;
        const auto error = VirtualProtect(_exec, _code.size(), PAGE_EXECUTE, &protect);
        if (error == ERROR) {
            throw std::runtime_error(GetLastErrorAsString());
        }

        std::vector<unsigned char> mem(1);

        ((void(*)(void*))_exec)(mem.data());

        VirtualProtect(_exec, _code.size(), protect, &protect);
    }

    std::string Exec::GetLastErrorAsString()
    {
        DWORD error_code = GetLastError();
        if (error_code == 0) {
            return "unknown error";
        }

        LPSTR buffer = nullptr;

        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buffer, 0, NULL);

        std::string message(buffer, size);

        LocalFree(buffer);

        return message;
    }
}