#pragma once

#include <array>
#include <iterator>
#include <optional>
#include <utility>

namespace Ignosi::Libraries::Containers
{
    template <class T, size_t SIZE>
    class RingBuffer
    {
        std::array<std::optional<T>, SIZE + 1> m_Data;

        using IT  = decltype(m_Data.begin());
        using cIT = decltype(m_Data.cbegin());

        IT m_Front = m_Data.begin();
        IT m_Back  = m_Data.begin();

      public:
        RingBuffer() {}
        std::optional<std::reference_wrapper<T>> push(const T& value) { return push(T(value)); }

        std::optional<std::reference_wrapper<T>> push(T&& value)
        {
            if (!full())
            {
                *m_Back = std::forward<T>(value);
                T& rslt = (*m_Back).value();
                m_Back++;
                if (m_Back == m_Data.end())
                    m_Back = m_Data.begin();

                return rslt;
            }

            return std::nullopt;
        }

        std::optional<T> pop()
        {
            if (!empty())
            {
                T rslt = std::move((*m_Front).value());
                m_Front->reset();
                m_Front++;
                if (m_Front == m_Data.end())
                    m_Front = m_Data.begin();
                return rslt;
            }
            return std::nullopt;
        }

        bool full() const { return m_Front == m_Data.begin() ? m_Back == m_Data.end() - 1 : m_Back == m_Front - 1; }
        bool empty() const { return m_Back == m_Front; }

        constexpr size_t size() const { return SIZE; }
        size_t           count() const
        {
            return m_Front < m_Back ? std::distance<cIT>(m_Front, m_Back)
                                    : std::distance<cIT>(m_Front, m_Data.end()) + std::distance<cIT>(m_Data.begin(), m_Back);
        }
    };
} // namespace Ignosi::Libraries::Containers
