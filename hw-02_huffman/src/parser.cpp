#include "parser.h"
#include <string>
#include <tuple>

namespace parser_exceptions
{
    ParserException::ParserException(std::string message) : _message(std::move(message))
    {}

    const char *ParserException::what() const noexcept
    {
        return _message.c_str();
    }
}

namespace parser
{
    std::tuple<std::string, std::string, std::string> ParseArgs(int argc, char **argv)
    {
        if (argc != 6)
        {
            throw parser_exceptions::ParserException("Wrong args, check documentation");
        }

        std::string action{};
        std::string InFile{};
        std::string OutFile{};
        for (std::size_t i = 1; i < 6; ++i)
        {
            std::string s = std::string(argv[i]);

            if (s == "-u" || s == "-c")
            {
                action = s;
            } else if (s == "-f" && i < 5)
            {
                InFile = std::string(argv[i++ + 1]);
            } else if (s == "-o" && i < 5)
            {
                OutFile = std::string(argv[i++ + 1]);
            } else
            {
                throw parser_exceptions::ParserException("Wrong args, check documentation");
            }
        }
        return {action, InFile, OutFile};
    }
}