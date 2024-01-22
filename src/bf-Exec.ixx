export module bf:Exec;

import std;

namespace bf
{
    export class Exec
    {
        std::vector<std::uint8_t> _code;
        void* _exec;

        static std::string GetLastErrorAsString();

    public:
        Exec(const std::vector<std::uint8_t>& code);
        ~Exec();
        Exec(const Exec& other) = delete;
        Exec(Exec&& other) noexcept;
        Exec& operator=(const Exec& other) = delete;
        Exec& operator=(Exec&& other) noexcept;

        void run();
    };
}