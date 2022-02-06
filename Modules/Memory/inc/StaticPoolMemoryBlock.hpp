#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>

namespace nate::Modules::Memory {
    template <typename T>
    using unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;

    template <typename T, std::uint8_t* BEGIN, size_t SIZE>
    class StaticPoolMemoryBlock {
      private:
        static_assert(
            sizeof(T) >= sizeof(std::uint8_t*),
            "sizeof(T) must be greater than or equal to sizeof(std::uint8_t*)");

        std::uint8_t* const m_InitialLoc;
        std::uint8_t*       m_NextLocation;
        size_t              m_UsedBlocks;

      public:
        StaticPoolMemoryBlock()
            : m_InitialLoc(BEGIN + reinterpret_cast<std::uintptr_t>(BEGIN) % sizeof(std::uint8_t*))
            , m_NextLocation(m_InitialLoc)
            , m_UsedBlocks(0)
        {
            for (std::uint8_t* pLoc = m_InitialLoc; pLoc < m_InitialLoc + SIZE; pLoc += sizeof(T))
            {
                std::uint8_t* pNext = pLoc + sizeof(T);
                memcpy(pLoc, &pNext, sizeof(std::uint8_t*));
            }
        }

        size_t UsedSize() const { return m_UsedBlocks * sizeof(T); }
        size_t RemainingSize() const { return SIZE - UsedSize(); }

        template <typename... Args>
        unique_ptr<T> MakeObject(Args&&... args)
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
                auto* pObjectLocation = reinterpret_cast<std::uint8_t*>(pObjectToDelete);

                memcpy(pObjectLocation, &m_NextLocation, sizeof(std::uint8_t*));
                m_NextLocation = pObjectLocation;

                assert(std::distance(m_InitialLoc, m_NextLocation) % sizeof(T) == 0);
                --m_UsedBlocks;
            };

            ++m_UsedBlocks;
            return unique_ptr<T>(pObject, destroyObject);
        }
    };
} // namespace nate::Modules::Memory
