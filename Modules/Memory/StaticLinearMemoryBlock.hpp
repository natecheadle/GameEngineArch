#pragma once

#include <NullMutex.hpp>

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <mutex>

namespace Ignosi::Modules::Memory {
    template <std::uint8_t* BEGIN, size_t SIZE, typename MUTEX = NullMutex>
    class StaticLinearMemoryBlock {
      private:
        static constexpr std::uint8_t* END = BEGIN + SIZE;

        std::uint8_t* const m_InitialLoc;
        std::uint8_t*       m_CurrentLoc;
        MUTEX               m_Mutex;

      public:
        StaticLinearMemoryBlock()
            : m_InitialLoc(BEGIN + reinterpret_cast<std::uintptr_t>(BEGIN) % sizeof(std::uint8_t*))
            , m_CurrentLoc(m_InitialLoc)
        {
        }

        void Reset() { m_CurrentLoc = m_InitialLoc; }

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

        template <typename T, typename... Args>
        T* MakeObject(Args&&... args)
        {
            std::unique_lock<MUTEX> lock(m_Mutex);

            if (sizeof(T) > PrivRemainingSize())
            {
                return nullptr;
            }

            T* pObject = new (reinterpret_cast<void*>(m_CurrentLoc)) T(std::forward<Args>(args)...);
            m_CurrentLoc += sizeof(T);

            return pObject;
        }

      private:
        size_t PrivUsedSize() { return std::distance(m_InitialLoc, m_CurrentLoc); }
        size_t PrivRemainingSize() { return std::distance(m_CurrentLoc, END); }
    };
} // namespace Ignosi::Modules::Memory
