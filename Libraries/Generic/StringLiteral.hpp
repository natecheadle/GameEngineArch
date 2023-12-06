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
        constexpr StringLiteral() = default;
        constexpr StringLiteral(const char (&str)[N]) { std::copy(str, str + N, Value); }

        ~StringLiteral() = default;

        constexpr StringLiteral(const StringLiteral& other) { std::copy(other.Value, other.Value + N, Value); }
        constexpr StringLiteral operator=(const StringLiteral& other) { std::copy(other.Value, other.Value + N, Value); }

        constexpr std::string_view String() const { return Value; }

        template <size_t M>
        constexpr friend StringLiteral<N + M - 1> operator+(const StringLiteral<N>& lhs, const StringLiteral<M>& rhs)
        {
            StringLiteral<N + M - 1> rslt;
            std::copy(lhs.Value, lhs.Value + N - 1, rslt.Value);
            std::copy(rhs.Value, rhs.Value + M, rslt.Value + N - 1);

            return rslt;
        }

        template <size_t M>
        constexpr friend bool operator==(const StringLiteral<M>& lhs, const StringLiteral<N>& rhs)
        {
            if (N != M)
                return false;

            if (rhs.Value == lhs.Value)
                return true;

            for (size_t i = 0; i < N; ++i)
            {
                if (rhs.Value[i] != lhs.Value[i])
                    return false;
            }
            return true;
        }
    };
} // namespace Ignosi::Libraries
