#pragma once

#include <algorithm>
#include <string_view>

namespace Ignosi::Libraries
{
    template <size_t N>
    struct StringLiteral
    {
        char Value[N];

      public:
        constexpr StringLiteral(const char (&str)[N]) { std::copy(str, str + N, Value); }

         constexpr std::string_view String() const { return Value; }
    };
} // namespace Ignosi::Libraries