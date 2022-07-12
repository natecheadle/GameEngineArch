#pragma once

#include "Message.hpp"

#include <memory>

namespace nate::Modules::Messaging {
    template <class ID_T, class DATA>
    class DataMessage : public Message<ID_T> {
      public:
        DataMessage(ID_T id, std::unique_ptr<DATA> pData)
            : Message<ID_T>(id)
            , m_pData(std::move(pData))
        {
        }

        virtual ~DataMessage() = default;

        std::unique_ptr<DATA> ReleaseData() { return std::move(m_pData); }
        DATA*                 GetData() const { return m_pData.get(); }

      private:
        std::unique_ptr<DATA> m_pData;
    };

    using GenericDataMessage = DataMessage<std::int64_t, std::int64_t>;
} // namespace nate::Modules::Messaging