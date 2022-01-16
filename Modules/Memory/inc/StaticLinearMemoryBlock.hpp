#include <array>
#include <cstdint>

namespace nate::Modules::Memory {
    template <size_t SIZE>
    class StaticLinearMemoryBlock {
      private:
        typedef typename std::array<std::uint8_t, SIZE>::iterator MemIterator;

        std::array<std::uint8_t, SIZE> m_Memory;
        size_t                         m_NextBlockLocation;

      public:
        StaticLinearMemoryBlock()
            : m_NextBlockLocation(0)
        {
        }

        void Reset() { m_NextBlockLocation = 0; }

        size_t UsedSize() const { return m_NextBlockLocation; }
        size_t RemainingSize() const { return m_Memory.size() - m_NextBlockLocation; }

        template <typename T, typename... Args>
        T* MakeObject(Args&&... args)
        {
            if (sizeof(T) > RemainingSize())
            {
                return nullptr;
            }

            T* pObject = new ((void*)(&m_Memory[m_NextBlockLocation])) T(std::forward<Args>(args)...);
            if (pObject)
            {
                m_NextBlockLocation += sizeof(T);
            }
            else
            {
                return nullptr;
            }

            return pObject;
        }
    };
} // namespace nate::Modules::Memory
