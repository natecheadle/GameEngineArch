#include <cstddef>
#include <cstdint>
#include <iterator>

namespace nate::Modules::Memory {
    template <std::uint8_t* BEGIN, size_t SIZE>
    class StaticLinearMemoryBlock {
      private:
        static constexpr std::uint8_t* END = BEGIN + SIZE;

        std::uint8_t* const m_InitialLoc;
        std::uint8_t*       m_CurrentLoc;

      public:
        StaticLinearMemoryBlock()
            : m_InitialLoc(BEGIN + reinterpret_cast<std::uintptr_t>(BEGIN) % sizeof(std::uint8_t*))
            , m_CurrentLoc(m_InitialLoc)
        {
        }

        void Reset() { m_CurrentLoc = m_InitialLoc; }

        size_t UsedSize() const { return std::distance(m_InitialLoc, m_CurrentLoc); }
        size_t RemainingSize() const { return std::distance(m_CurrentLoc, END); }

        template <typename T, typename... Args>
        T* MakeObject(Args&&... args)
        {
            if (sizeof(T) > RemainingSize())
            {
                return nullptr;
            }

            T* pObject = new (reinterpret_cast<void*>(m_CurrentLoc)) T(std::forward<Args>(args)...);
            m_CurrentLoc += sizeof(T);

            return pObject;
        }
    };
} // namespace nate::Modules::Memory
