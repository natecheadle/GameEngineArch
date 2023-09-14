#pragma once

#include <NullMutex.hpp>

#include <array>
#include <cstdint>
#include <mutex>

namespace Ignosi::Modules::Memory {
    template <std::uint8_t* BEGIN, size_t SIZE, typename MUTEX = NullMutex>
    class StaticStackMemoryBlock {
      private:
        static constexpr std::uint8_t* END = BEGIN + SIZE;

        std::uint8_t* const m_InitialLoc;
        std::uint8_t*       m_CurrentLoc;
        size_t              m_TotalObjectCount;
        MUTEX               m_Mutex;

      public:
        StaticStackMemoryBlock()
            : m_InitialLoc(BEGIN + reinterpret_cast<std::uintptr_t>(BEGIN) % sizeof(std::uint8_t*))
            , m_CurrentLoc(m_InitialLoc)
            , m_TotalObjectCount(0)
        {
        }

        void Reset()
        {
            std::unique_lock<MUTEX> lock(m_Mutex);
            PrivReset();
        }

        void Release(size_t numOfObjects)
        {
            std::unique_lock<MUTEX> lock(m_Mutex);
            PrivRelease(numOfObjects);
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

        template <typename T, typename... Args>
        T* MakeObject(Args&&... args)
        {
            std::unique_lock<MUTEX> lock(m_Mutex);
            if ((sizeof(T) + sizeof(std::uint8_t*)) > PrivRemainingSize())
            {
                return nullptr;
            }

            T* pObject = new (reinterpret_cast<void*>(m_CurrentLoc)) T(std::forward<Args>(args)...);

            std::uint8_t* newLoc = m_CurrentLoc + sizeof(T) + sizeof(std::uint8_t*);
            memcpy(newLoc - sizeof(std::uint8_t*), &m_CurrentLoc, sizeof(std::uint8_t*));
            m_CurrentLoc = newLoc;
            m_TotalObjectCount++;
            return pObject;
        }

      private:
        void PrivReset()
        {
            m_CurrentLoc       = m_InitialLoc;
            m_TotalObjectCount = 0;
        }

        void PrivRelease(size_t numOfObjects)
        {
            if (numOfObjects >= m_TotalObjectCount)
            {
                PrivReset();
            }
            else
            {
                for (size_t i = 0; i < numOfObjects; i++)
                {
                    memcpy(&m_CurrentLoc, m_CurrentLoc - sizeof(std::uint8_t*), sizeof(std::uint8_t*));
                    m_TotalObjectCount--;
                }
            }
        }

        size_t PrivUsedSize() { return std::distance(m_InitialLoc, m_CurrentLoc); }
        size_t PrivRemainingSize() { return std::distance(m_CurrentLoc, END); }
    };
} // namespace Ignosi::Modules::Memory
