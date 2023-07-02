#pragma once

#include "Message.hpp"

#include <memory>

namespace Ignosi::Modules::Messaging {
    template <class ID_T, class DATA>
    class LargeDataMessage : public Message<ID_T> {
      public:
        LargeDataMessage(ID_T id, std::unique_ptr<DATA> pData)
            : Message<ID_T>(id)
            , m_pData(std::move(pData))
        {
        }

        LargeDataMessage(LargeDataMessage&& other)
            : Message<ID_T>(other)
            , m_pData(std::move(other.m_pData))
        {
        }

        virtual ~LargeDataMessage() = default;

        std::unique_ptr<DATA> ReleaseData() { return std::move(m_pData); }
        DATA*                 GetData() const { return m_pData.get(); }

      private:
        std::unique_ptr<DATA> m_pData;
    };

    using GenericDataMessage = LargeDataMessage<std::int64_t, std::int64_t>;
} // namespace Ignosi::Modules::Messaging