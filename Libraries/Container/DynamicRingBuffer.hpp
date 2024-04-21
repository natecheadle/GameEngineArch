#pragma once

#include <cstddef>
#include <iterator>
#include <optional>
#include <vector>

namespace Ignosi::Libraries::Containers
{
    template <class T>
    class DynamicRingBuffer
    {

        std::vector<std::optional<T>> m_Data;

        using IT  = decltype(m_Data.begin());
        using cIT = decltype(m_Data.cbegin());

        IT m_Front = m_Data.begin();
        IT m_Back  = m_Data.begin();

      public:
        DynamicRingBuffer(size_t initSize = 16)
            : m_Data(initSize)
        {
        }

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
            else
            {
                resize(m_Data.size() * 2);
                return push(std::forward<T>(value));
            }
        }

        std::optional<T> pop()
        {
            if (!empty())
            {
                T rslt = std::move((*m_Front).value());
                m_Front++;
                if (m_Front == m_Data.end())
                    m_Front = m_Data.begin();
                return rslt;
            }
            return std::nullopt;
        }

        bool full() const { return m_Front == m_Data.begin() ? m_Back == m_Data.end() - 1 : m_Back == m_Front - 1; }
        bool empty() const { return m_Back == m_Front; }

        size_t size() const { return m_Data.size(); }

        size_t count() const
        {
            return m_Front < m_Back ? std::distance<cIT>(m_Front, m_Back)
                                    : std::distance<cIT>(m_Front, m_Data.end()) + std::distance<cIT>(m_Data.begin(), m_Back);
        }

        void resize(size_t newSize)
        {
            std::vector<std::optional<T>> newData(newSize);
            size_t                        i = 0;
            if (m_Front > m_Back)
            {
                for (auto it = m_Front; it != m_Data.end(); ++it)
                {
                    newData[i++] = std::move((*it).value());
                }
                for (auto it = m_Data.begin(); it != m_Back; ++it)
                {
                    newData[i++] = std::move((*it).value());
                }
            }
            else
            {
                for (auto it = m_Front; it != m_Back; ++it)
                {
                    newData[i++] = std::move((*it).value());
                }
            }

            m_Data  = std::move(newData);
            m_Front = m_Data.begin();
            m_Back  = m_Front + i;
        }
    };
} // namespace Ignosi::Libraries::Containers
