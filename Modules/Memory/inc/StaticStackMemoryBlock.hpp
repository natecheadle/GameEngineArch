#include <array>
#include <cstdint>

namespace nate::Modules::Memory {
    template <size_t SIZE>
    class StaticStackMemoryBlock {
      private:
        std::array<std::uint8_t, SIZE> m_Memory;

        size_t m_NextBlockLocation;
        size_t m_TotalObjectCount;

      public:
        StaticStackMemoryBlock()
            : m_NextBlockLocation(0)
            , m_TotalObjectCount(0)
        {
        }

        void Reset()
        {
            m_NextBlockLocation = 0;
            m_TotalObjectCount  = 0;
        }

        void Release(size_t numOfObjects)
        {
            for (size_t i = 0; i < numOfObjects; i++)
            {
                if (m_NextBlockLocation <= sizeof(size_t))
                {
                    Reset();
                    return;
                }

                size_t* pLastSize = reinterpret_cast<size_t*>(&m_Memory[m_NextBlockLocation - sizeof(size_t)]);
                m_NextBlockLocation -= (*pLastSize + sizeof(size_t));
            }
        }

        size_t UsedSize() const { return m_NextBlockLocation; }
        size_t RemainingSize() const { return m_Memory.size() - m_NextBlockLocation; }

        template <typename T, typename... Args>
        T* MakeObject(Args&&... args)
        {
            if ((sizeof(T) + sizeof(size_t)) > m_Memory.size() - m_NextBlockLocation)
            {
                return nullptr;
            }

            T* pObject = new ((void*)(&m_Memory[m_NextBlockLocation])) T(std::forward<Args>(args)...);
            m_NextBlockLocation += sizeof(T);

            size_t* pObjectSize = reinterpret_cast<size_t*>(&m_Memory[m_NextBlockLocation]);
            *pObjectSize        = sizeof(T);
            m_NextBlockLocation += sizeof(size_t);

            return pObject;
        }
    };
} // namespace nate::Modules::Memory
