module;
#if _WIN32 // Windows - Use VirtualAlloc
#include "Windows.h"
#else // Use mmap
#error platform not supported
#endif
module bf;

using namespace bf;

Exec::Exec(const Jit jit)
    : _jit(jit), _exec(nullptr)
{
    _exec = VirtualAlloc(nullptr, jit.code.size(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (_exec == nullptr) {
        throw std::runtime_error(GetLastErrorAsString());
    }

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

__declspec(noinline) void Exec::run()
{
    DWORD protect = 0;
    const auto error = VirtualProtect(_exec, _jit.code.size(), PAGE_EXECUTE, &protect);
    if (error == ERROR) {
        throw std::runtime_error(GetLastErrorAsString());
    }

    std::vector<unsigned char> mem(10 * 1024 * 1024);

    //_jit.dump();
    //std::cout << std::endl;
    //std::cout << std::endl;
    //dump();

    const auto func = reinterpret_cast<void(*)(uint8_t*)>(_exec);
    func(mem.data());

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

void Exec::dump(const std::size_t bytes_per_row)
{
    if (_exec == nullptr)
        throw std::runtime_error("object has been disposed");

    std::ios_base::fmtflags f(std::cout.flags());
    std::cout << std::hex << std::setfill('0');

    const auto bytes = reinterpret_cast<std::uint8_t*>(_exec);
    for (std::size_t i = 0; i < _jit.code.size(); ++i) {
        if (i != 0 && i % bytes_per_row == 0)
            std::cout << std::endl;
        std::cout << std::setw(2) << (int)bytes[i] << ' ';
    }
    std::cout.flags(f);
}