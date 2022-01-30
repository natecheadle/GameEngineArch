#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>

namespace nate::Modules::Memory {
    template <typename T, std::uint8_t* BEGIN, size_t SIZE>
    class StaticPoolMemoryBlock {
      private:
        static_assert(
            sizeof(T) >= sizeof(std::uint8_t*),
            "sizeof(T) must be greater than or equal to sizeof(std::uint8_t*)");

        std::uint8_t* m_NextLocation;
        size_t        m_UsedBlocks;

      public:
        StaticPoolMemoryBlock()
            : m_NextLocation(BEGIN)
            , m_UsedBlocks(0)
        {
            for (std::uint8_t* pLoc = BEGIN; pLoc < BEGIN + SIZE; pLoc += sizeof(T))
            {
                std::uint8_t* pNext = pLoc + sizeof(T);
                memcpy(pLoc, &pNext, sizeof(std::uint8_t*));
            }
        }

        size_t UsedSize() const { return m_UsedBlocks * sizeof(T); }
        size_t RemainingSize() const { return SIZE - UsedSize(); }

        template <typename... Args>
        std::unique_ptr<T, std::function<void(T*)>> MakeObject(Args&&... args)
        {
            if (RemainingSize() == 0)
            {
                return nullptr;
            }

            std::uint8_t* nextLocation = m_NextLocation;

            T* pObject = new (reinterpret_cast<void*>(nextLocation)) T(std::forward<Args>(args)...);

            m_NextLocation = nextLocation;

            auto destroyObject = [&](T* pObjectToDelete) {
                // Destroy Object
                pObjectToDelete->~T();

                // Calculate where it was in the pool.
                auto pObjectLocation = reinterpret_cast<std::uint8_t*>(pObjectToDelete);

                memcpy(pObjectLocation, &m_NextLocation, sizeof(std::uint8_t*));
                m_NextLocation = pObjectLocation;

                assert(std::distance(BEGIN, m_NextLocation) % sizeof(T) == 0);
                --m_UsedBlocks;
            };

            ++m_UsedBlocks;
            return std::unique_ptr<T, std::function<void(T*)>>(pObject, destroyObject);
        }
    };
} // namespace nate::Modules::Memory
