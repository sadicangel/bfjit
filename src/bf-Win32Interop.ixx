module;
#if _WIN32 // Windows - Use VirtualAlloc
#include "Windows.h"
#endif
export module bf:Win32Interop;

import std;

namespace bf
{
#if _WIN32
    export class Win32Interop
    {
        static HANDLE stdout_handle();
        static HANDLE stdin_handle();

        static void write_many(const char* memory, const auto length);
    public:
        __declspec(noinline) static void write(const char* memory);
        __declspec(noinline) static void read(char* memory);
        static std::string GetLastErrorAsString();
    };
#endif
}