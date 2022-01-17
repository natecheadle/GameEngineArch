#include <array>
#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>

namespace nate::Modules::Memory {
    template <typename T, size_t SIZE>
    class StaticPoolMemoryBlock {
      private:
        static_assert(sizeof(T) >= sizeof(size_t), "sizeof(T) must be greater than or equal to sizeof(size_t)");

        static constexpr size_t MemorySizeT = sizeof(T) * SIZE;

        std::array<std::uint8_t, MemorySizeT> m_Memory;

        size_t m_NextBlockLocation;
        size_t m_UsedBlocks;

      public:
        StaticPoolMemoryBlock()
            : m_NextBlockLocation(0)
            , m_UsedBlocks(0)
        {
            for (size_t i = 0; i < m_Memory.size(); i += sizeof(T))
            {
                size_t* pNext = reinterpret_cast<size_t*>(&m_Memory[i]);
                *pNext        = i + sizeof(T);
            }
        }

        size_t UsedSize() const { return m_UsedBlocks * sizeof(T); }
        size_t RemainingSize() const { return MemorySizeT - UsedSize(); }

        template <typename... Args>
        std::unique_ptr<T, std::function<void(T*)>> MakeObject(Args&&... args)
        {
            if (RemainingSize() == 0)
            {
                return nullptr;
            }

            size_t nextBlockLocation = *(reinterpret_cast<size_t*>(&m_Memory[m_NextBlockLocation]));

            T* pObject = new ((void*)(&m_Memory[m_NextBlockLocation])) T(std::forward<Args>(args)...);

            m_NextBlockLocation = nextBlockLocation;

            auto destroyObject = [&](T* pObjectToDelete) {
                // Destroy Object
                pObjectToDelete->~T();

                // Calculate where it was in the pool.
                auto   pObjectLocation = reinterpret_cast<std::uint8_t*>(pObjectToDelete);
                size_t objectLocation  = std::distance(&m_Memory[0], pObjectLocation);

                *(reinterpret_cast<size_t*>(&m_Memory[objectLocation])) = m_NextBlockLocation;
                m_NextBlockLocation                                     = objectLocation;

                assert(m_NextBlockLocation % sizeof(T) == 0);
                --m_UsedBlocks;
            };

            ++m_UsedBlocks;
            return std::unique_ptr<T, std::function<void(T*)>>(pObject, destroyObject);
        }
    };
} // namespace nate::Modules::Memory
