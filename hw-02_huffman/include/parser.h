#pragma once

#include <string>
#include <tuple>

namespace parser_exceptions
{
    class ParserException : public std::exception
    {
    private:
        std::string _message{};
    public:
        explicit ParserException(std::string message);

        [[nodiscard]] const char *what() const noexcept override;
    };
}

namespace parser
{
    std::tuple<std::string, std::string, std::string> ParseArgs(int argc, char **argv);
}
