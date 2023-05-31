#pragma once

#include <LinearAlgebra/SquareMatrix.hpp>
#include <LinearAlgebra/Vector.hpp>

#include <cassert>
#include <cstddef>
#include <limits>
#include <type_traits>
#include <vector>

namespace nate::Modules::Render
{
    class VertexDataConfig
    {
      public:
        enum class DataType : std::uint16_t
        {
            INT32,
            UINT32,
            FLOAT32,

            LAST
        };

        struct Config
        {
            DataType      BaseType;
            std::uint16_t Length;

            inline int TotalSize() const
            {
                switch (BaseType)
                {
                case DataType::INT32: return static_cast<int>(Length * sizeof(std::int32_t));
                case DataType::UINT32: return static_cast<int>(Length * sizeof(std::uint32_t));
                case DataType::FLOAT32: return static_cast<int>(Length * sizeof(float));
                default: assert(false); return 0;
                }
            }
        };

      private:
        std::vector<Config> m_DataConfig;

      public:
        template <class T, size_t SIZE>
        void PushBackConfig(const SquareMatrix<SIZE, T>&)
        {
            PushBackConfig<T, SIZE * SIZE>();
        }

        template <class T, size_t SIZE>
        void PushBackConfig(const Vector<SIZE, T>&)
        {
            PushBackConfig<T, SIZE>();
        }

        template <class T, size_t SIZE>
        void PushBackConfig()
        {
            static_assert(SIZE < std::numeric_limits<std::uint16_t>::max(), "Max size is UINT16_MAX");

            if constexpr (std::is_same_v<T, std::int32_t>)
            {
                m_DataConfig.push_back({DataType::INT32, SIZE});
                return;
            }
            if constexpr (std::is_same_v<T, std::uint32_t>)
            {
                m_DataConfig.push_back({DataType::UINT32, SIZE});
                return;
            }
            if constexpr (std::is_same_v<T, float>)
            {
                m_DataConfig.push_back({DataType::FLOAT32, SIZE});
                return;
            }

            assert(false);
        }

        void                       Clear() { m_DataConfig.clear(); }
        const std::vector<Config>& Describe() const { return m_DataConfig; }
        inline size_t              ConfigSize() const
        {
            size_t size{0};
            for (const auto& config : m_DataConfig)
            {
                size += config.TotalSize();
            }
            return size;
        }
    };
} // namespace nate::Modules::Render
