module;
#if _WIN32 // Windows - Use VirtualAlloc
#include "Windows.h"
#endif
module bf;

using namespace bf;

#if _WIN32
HANDLE Win32Interop::stdout_handle()
{
    const auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!handle) {
        throw std::runtime_error(GetLastErrorAsString());
    }
    return handle;
}

HANDLE Win32Interop::stdin_handle()
{
    const auto handle = GetStdHandle(STD_INPUT_HANDLE);
    if (!handle) {
        throw std::runtime_error(GetLastErrorAsString());
    }
    return handle;
}

void Win32Interop::write_many(const char* memory, const auto length)
{
    static const auto stdout = stdout_handle();
    LPDWORD written = nullptr;
    if (!WriteConsoleA(stdout, memory, length, written, 0)) {
        throw std::runtime_error(GetLastErrorAsString());
    }
}

void Win32Interop::read(char* memory)
{
    static const auto stdin = stdin_handle();
    static const auto stdout = stdout_handle();
    DWORD read = 0;
    INPUT_RECORD buffer;
    char wrt[3] = "\b ";
    char del[4] = "\b \b";
    char end[3] = "\r\n";
    while (true) {
        if (!ReadConsoleInputA(stdin, &buffer, 1, &read)) {
            throw std::runtime_error(GetLastErrorAsString());
        }

        if (read > 0 && buffer.EventType == KEY_EVENT && buffer.Event.KeyEvent.bKeyDown) {
            const auto c = buffer.Event.KeyEvent.uChar.AsciiChar;
            if (std::isprint(c)) {
                memory[0] = wrt[1] = c;
                write_many(wrt, 3);
            }
            if (c == '\b') {
                write_many(del, 4);
            }
            if (c == '\r') {
                write_many(end, 3);
                break;
            }
        }
    }
}

std::string Win32Interop::GetLastErrorAsString()
{
    auto error = GetLastError();
    if (!error) {
        return "unknown error";
    }

    auto  buffer = nullptr;

    auto size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buffer, 0, NULL);

    std::string message(buffer, size);

    LocalFree(buffer);

    return message;
}
#endif
