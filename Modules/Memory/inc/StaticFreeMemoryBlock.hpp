#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <memory>

namespace nate::Modules::Memory {
    template <std::uint8_t* BEGIN, size_t SIZE>
    class StaticFreeMemoryBlock {
      private:
        struct EmptySizeHeader
        {
            std::uint8_t* pNext;
            size_t        Size;
        };

        struct HeaderPair
        {
            EmptySizeHeader* pHeader;
            EmptySizeHeader* pPrevHeader;
        };

        static constexpr std::uint8_t* END = BEGIN + SIZE;

        std::uint8_t* m_FirstLoc;
        size_t        m_UsedSize;

      public:
        StaticFreeMemoryBlock()
            : m_FirstLoc(BEGIN)
            , m_UsedSize(0)
        {
            auto pFirstHeader   = reinterpret_cast<EmptySizeHeader*>(m_FirstLoc);
            pFirstHeader->pNext = nullptr;
            pFirstHeader->Size  = SIZE - sizeof(EmptySizeHeader);
        }

        size_t UsedSize() const { return m_UsedSize; }
        size_t RemainingSize() const { return SIZE - m_UsedSize; }

        template <typename T, typename... Args>
        std::unique_ptr<T, std::function<void(T*)>> MakeObject(Args&&... args)
        {
            static_assert(sizeof(T) >= sizeof(EmptySizeHeader), "T is too small.");

            if (sizeof(T) > RemainingSize())
            {
                return nullptr;
            }

            size_t TSize = sizeof(T) + sizeof(T) % sizeof(EmptySizeHeader);

            HeaderPair nextLoc = GetNextValidFreeLocation(TSize);
            if (!nextLoc.pHeader)
                return nullptr;

            // Size is larger than needed, need to divide it up.
            if (nextLoc.pHeader->Size > TSize)
            {
                EmptySizeHeader* pNew{nullptr};
                if (!nextLoc.pPrevHeader)
                {
                    m_FirstLoc = reinterpret_cast<std::uint8_t*>(nextLoc.pHeader) + TSize;
                    pNew       = reinterpret_cast<EmptySizeHeader*>(m_FirstLoc);
                }
                else
                {
                    nextLoc.pPrevHeader->pNext = reinterpret_cast<std::uint8_t*>(nextLoc.pHeader) + TSize;
                    pNew                       = reinterpret_cast<EmptySizeHeader*>(nextLoc.pPrevHeader->pNext);
                }

                pNew->pNext = nextLoc.pHeader->pNext;
                pNew->Size  = nextLoc.pHeader->Size - TSize;
            }
            else
            {
                if (!nextLoc.pPrevHeader)
                {
                    m_FirstLoc          = nextLoc.pHeader->pNext;
                    nextLoc.pPrevHeader = reinterpret_cast<EmptySizeHeader*>(m_FirstLoc);
                }

                nextLoc.pPrevHeader->pNext = reinterpret_cast<EmptySizeHeader*>(nextLoc.pHeader->pNext)->pNext;
            }

            T*   pObject      = new (reinterpret_cast<void*>(nextLoc.pHeader)) T(std::forward<Args>(args)...);
            auto deleteObject = [&](T* pObject) {
                pObject->~T();
                size_t sizeT = (sizeof(T) + sizeof(T) % sizeof(EmptySizeHeader));

                HeaderPair prevLoc = GetPreviousLocation(reinterpret_cast<std::uint8_t*>(pObject), sizeT);

                if (prevLoc.pPrevHeader == nullptr)
                {
                    auto pFirstLoc = reinterpret_cast<EmptySizeHeader*>(m_FirstLoc);
                    if (reinterpret_cast<std::uint8_t*>(pObject) == m_FirstLoc - sizeT)
                    {
                        prevLoc.pHeader->pNext = pFirstLoc->pNext;
                        prevLoc.pHeader->Size  = sizeT + pFirstLoc->Size;
                    }
                    else
                    {
                        prevLoc.pHeader->pNext = m_FirstLoc;
                        prevLoc.pHeader->Size  = sizeT;
                    }

                    m_FirstLoc = reinterpret_cast<std::uint8_t*>(prevLoc.pHeader);
                }
                else if (reinterpret_cast<std::uint8_t*>(pObject) == prevLoc.pPrevHeader->pNext - sizeT)
                {
                    size_t size =
                        std::distance(reinterpret_cast<std::uint8_t*>(prevLoc.pHeader), prevLoc.pHeader->pNext);
                    prevLoc.pPrevHeader->Size += size + prevLoc.pHeader->Size;
                    prevLoc.pPrevHeader->pNext = prevLoc.pHeader->pNext;
                }
                else
                {
                    auto pNew   = reinterpret_cast<EmptySizeHeader*>(pObject);
                    pNew->Size  = sizeT;
                    pNew->pNext = prevLoc.pPrevHeader->pNext;

                    prevLoc.pPrevHeader->pNext = reinterpret_cast<std::uint8_t*>(pObject);
                }

                m_UsedSize -= sizeT;
            };

            m_UsedSize += TSize;

            return std::unique_ptr<T, std::function<void(T*)>>(pObject, deleteObject);
        }

      private:
        HeaderPair GetNextValidFreeLocation(size_t sizeReq)
        {
            HeaderPair returnVal;
            returnVal.pHeader     = reinterpret_cast<EmptySizeHeader*>(m_FirstLoc);
            returnVal.pPrevHeader = nullptr;

            while (returnVal.pHeader->pNext != nullptr && returnVal.pHeader->Size < sizeReq)
            {
                returnVal.pPrevHeader = returnVal.pHeader;
                returnVal.pHeader     = reinterpret_cast<EmptySizeHeader*>(returnVal.pHeader->pNext);
            }

            if (returnVal.pHeader->Size >= sizeReq)
            {
                return returnVal;
            }

            return {nullptr, nullptr};
        }

        HeaderPair GetPreviousLocation(std::uint8_t* pObject, size_t objectSize)
        {
            if (pObject < m_FirstLoc)
            {
                auto pPrev   = reinterpret_cast<EmptySizeHeader*>(pObject);
                pPrev->Size  = objectSize;
                pPrev->pNext = m_FirstLoc;
                return {pPrev, nullptr};
            }

            HeaderPair returnVal;
            returnVal.pHeader     = reinterpret_cast<EmptySizeHeader*>(m_FirstLoc);
            returnVal.pPrevHeader = nullptr;

            while (reinterpret_cast<EmptySizeHeader*>(pObject) < returnVal.pHeader)
            {
                pObject               = returnVal.pHeader->pNext;
                returnVal.pPrevHeader = returnVal.pHeader;
                returnVal.pHeader     = reinterpret_cast<EmptySizeHeader*>(returnVal.pHeader->pNext);
            }

            assert(pObject);

            return returnVal;
        }
    };
} // namespace nate::Modules::Memory
