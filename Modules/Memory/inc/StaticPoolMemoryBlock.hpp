#pragma once

#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <type_traits>

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
            return MakeOtherObject<T, Args...>(std::forward<Args>(std::move(args))...);
        }

        template <class OTHER, typename... Args>
        std::unique_ptr<OTHER, std::function<void(OTHER*)>> MakeOtherObject(Args&&... args)
        {
            return MakeBaseOtherObject<OTHER, OTHER, Args...>(std::forward<Args>(std::move(args))...);
        }

        template <class OTHER, class OTHER_BASE, typename... Args>
        std::unique_ptr<OTHER_BASE, std::function<void(OTHER_BASE*)>> MakeBaseOtherObject(Args&&... args)
        {
            static_assert(sizeof(OTHER) <= sizeof(T), "Cannot build object OTHER in pool because it exceed sizeof(T)");
            static_assert(std::is_base_of_v<OTHER_BASE, OTHER>, "OTHER_BASE must be a base class of OTHER");

            if (RemainingSize() == 0)
            {
                return nullptr;
            }

            std::uint8_t* nextLocation = m_NextLocation;

            OTHER_BASE* pObject =
                new (reinterpret_cast<void*>(nextLocation)) OTHER(std::forward<Args>(std::move(args))...);

            m_NextLocation = nextLocation;

            auto destroyObject = [&](OTHER_BASE* pObjectToDelete) {
                // Destroy Object
                pObjectToDelete->~OTHER_BASE();

                // Calculate where it was in the pool.
                auto* pObjectLocation = reinterpret_cast<std::uint8_t*>(pObjectToDelete);

                memcpy(pObjectLocation, &m_NextLocation, sizeof(std::uint8_t*));
                m_NextLocation = pObjectLocation;

                assert(std::distance(m_InitialLoc, m_NextLocation) % sizeof(T) == 0);
                --m_UsedBlocks;
            };

            ++m_UsedBlocks;
            return std::unique_ptr<OTHER_BASE, std::function<void(OTHER_BASE*)>>(pObject, destroyObject);
        }
    };
} // namespace nate::Modules::Memory
