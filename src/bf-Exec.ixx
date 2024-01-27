export module bf:Exec;

import :Jit;

namespace bf
{
    export class Exec
    {
        Jit _jit;
        void* _exec;

    public:
        Exec(const bf::Jit jit);
        ~Exec();
        Exec(const Exec& other) = delete;
        Exec(Exec&& other) noexcept;
        Exec& operator=(const Exec& other) = delete;
        Exec& operator=(Exec&& other) noexcept;

        __declspec(noinline) void run();

        void dump(const std::size_t bytes_per_row = 16);
    };
}