#pragma once

#include <array>
#include <iterator>
#include <type_traits>
#include <utility>

namespace Ignosi::Modules
{
    template <class KEY, KEY Begin, KEY End, class VALUE>
    class StaticMap
    {
        static_assert(std::is_enum_v<KEY> || std::is_integral_v<KEY>, "KEY must be enum or int type");
        static_assert(Begin < End, "End must be after Begin.");
        static_assert(
            static_cast<KEY>(0) <= Begin,
            "Begin value must have an integer value greater than or equal to 0");
        using DATA_T = std::array<std::pair<KEY, VALUE>, static_cast<size_t>(End) - static_cast<size_t>(Begin)>;
        DATA_T m_Data;

      public:
        using iterator               = typename DATA_T::iterator;
        using const_iterator         = typename DATA_T::const_iterator;
        using reverse_iterator       = typename DATA_T::reverse_iterator;
        using const_reverse_iterator = typename DATA_T::const_reverse_iterator;

        StaticMap(VALUE init = VALUE())
        {
            for (size_t i = static_cast<size_t>(Begin); i < static_cast<size_t>(End); ++i)
            {
                m_Data[i] = {static_cast<KEY>(i), init};
            }
        }

        template <class IT>
        StaticMap(IT begin, IT end)
        {
            static_assert(
                std::is_same_v<typename std::iterator_traits<IT>::value_type, std::pair<KEY, VALUE>>,
                "Iterator must be over std::pair<KEY, VALUE>");

            for (auto it = begin; it < end; ++it)
            {
                m_Data[static_cast<size_t>(*it.first)] = *it;
            }
        }

        StaticMap(const StaticMap& other)
            : m_Data(other.m_Data)
        {
        }

        StaticMap(StaticMap&& other) noexcept
            : m_Data(std::move(other.m_Data))
        {
        }

        StaticMap& operator=(const StaticMap& other)
        {
            if (this == &other)
                return *this;

            m_Data = other.m_Data;
            return *this;
        }

        StaticMap& operator=(StaticMap&& other) noexcept
        {
            if (this == &other)
                return *this;

            m_Data = std::move(other.m_Data);
            return *this;
        }

        VALUE& operator[](KEY key) { return m_Data[static_cast<size_t>(key) - static_cast<size_t>(Begin)].second; }
        const VALUE& operator[](KEY key) const
        {
            return m_Data[static_cast<size_t>(key) - static_cast<size_t>(Begin)].second;
        }

        VALUE&       at(KEY key) { return (*this)[key]; }
        const VALUE& at(KEY key) const { return (*this)[key]; }

        iterator       begin() { return m_Data.begin(); }
        const_iterator begin() const { return m_Data.begin(); }

        iterator       end() { return m_Data.end(); }
        const_iterator end() const { return m_Data.end(); }

        reverse_iterator       rbegin() { return m_Data.rbegin(); }
        const_reverse_iterator rbegin() const { return m_Data.rbegin(); }

        reverse_iterator       rend() { return m_Data.rend(); }
        const_reverse_iterator rend() const { return m_Data.rend(); }

        size_t size() const { return m_Data.size(); }
        size_t max_size() const { return m_Data.max_size(); }

        bool operator==(const StaticMap& other) { return m_Data == other.m_Data; }
        bool operator!=(const StaticMap& other) { return m_Data != other.m_Data; }
    };
} // namespace Ignosi::Modules
