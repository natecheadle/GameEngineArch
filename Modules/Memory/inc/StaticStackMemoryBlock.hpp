#include <array>
#include <cstdint>

namespace nate::Modules::Memory {
    template <std::uint8_t* BEGIN, size_t SIZE>
    class StaticStackMemoryBlock {
      private:
        static constexpr std::uint8_t* END = BEGIN + SIZE;

        std::uint8_t* m_CurrentLoc;
        size_t        m_TotalObjectCount;

      public:
        StaticStackMemoryBlock()
            : m_CurrentLoc(BEGIN)
            , m_TotalObjectCount(0)
        {
        }

        void Reset()
        {
            m_CurrentLoc       = BEGIN;
            m_TotalObjectCount = 0;
        }

        void Release(size_t numOfObjects)
        {
            if (numOfObjects >= m_TotalObjectCount)
            {
                Reset();
                return;
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

        size_t UsedSize() const { return std::distance(BEGIN, m_CurrentLoc); }
        size_t RemainingSize() const { return std::distance(m_CurrentLoc, END); }

        template <typename T, typename... Args>
        T* MakeObject(Args&&... args)
        {

            if ((sizeof(T) + sizeof(std::uint8_t*)) > RemainingSize())
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
    };
} // namespace nate::Modules::Memory
