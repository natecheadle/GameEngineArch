#pragma once

#include <NullMutex.hpp>

#include <cassert>
#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>

namespace nate::Modules::Memory {
    template <typename T>
    using unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;

    template <typename T, std::uint8_t* BEGIN, size_t SIZE, typename MUTEX = NullMutex>
    class StaticPoolMemoryBlock {
      private:
        static_assert(
            sizeof(T) >= sizeof(std::uint8_t*),
            "sizeof(T) must be greater than or equal to sizeof(std::uint8_t*)");

        std::uint8_t* const m_InitialLoc;
        std::uint8_t*       m_NextLocation;
        size_t              m_UsedBlocks;
        MUTEX               m_Mutex;

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

        size_t UsedSize()
        {
            std::unique_lock<MUTEX> lock(m_Mutex);
            return PrivUsedSize();
        }
        size_t RemainingSize()
        {
            std::unique_lock<MUTEX> lock(m_Mutex);
            return PrivRemainingSize();
        }

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
            std::unique_lock<MUTEX> lock(m_Mutex);

            static_assert(sizeof(OTHER) <= sizeof(T), "Cannot build object OTHER in pool because it exceed sizeof(T)");
            static_assert(std::is_base_of_v<OTHER_BASE, OTHER>, "OTHER_BASE must be a base class of OTHER");

            if (PrivRemainingSize() == 0)
            {
                return nullptr;
            }

            std::uint8_t* nextLocation{nullptr};
            memcpy(&nextLocation, m_NextLocation, sizeof(std::uint8_t*));

            OTHER_BASE* pObject =
                new (reinterpret_cast<void*>(m_NextLocation)) OTHER(std::forward<Args>(std::move(args))...);

            m_NextLocation = nextLocation;

            auto destroyObject = [this](OTHER_BASE* pObjectToDelete) {
                std::unique_lock<MUTEX> lock(m_Mutex);

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

      private:
        size_t PrivUsedSize() { return m_UsedBlocks * sizeof(T); }
        size_t PrivRemainingSize() { return SIZE - PrivUsedSize(); }
    };
} // namespace nate::Modules::Memory
