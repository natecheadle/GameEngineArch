#pragma once

#include <glad/glad.h>

#include <initializer_list>
#include <memory>
#include <vector>

namespace nate::Modules::Render
{
    template <typename T>
    class ObjectBuffer
    {
        unsigned int   m_BO;
        std::vector<T> m_BufferData;

      public:
      protected:
        template <typename IT>
        ObjectBuffer<T>(IT begin, IT end)
            : m_BufferData(begin, end)
        {
        }

        ObjectBuffer<T>(std::vector<T> data)
            : m_BufferData(std::move(data))
        {
        }

        ObjectBuffer<T>(std::initializer_list<T> data)
            : m_BufferData(data)
        {
        }

        void InitializeBuffer() {}
    };
} // namespace nate::Modules::Render