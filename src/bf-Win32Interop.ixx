module;
#if _WIN32 // Windows
#include "Windows.h"
#endif
export module bf:Win32Interop;

import std;

namespace bf
{
#if _WIN32
    export class Win32Interop
    {
    public:
        __declspec(noinline) static void read(char* memory);
        static std::string GetLastErrorAsString();
    };
#endif
}