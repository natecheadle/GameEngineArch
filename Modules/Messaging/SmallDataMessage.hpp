#pragma once

#include "Message.hpp"

namespace Ignosi::Modules::Messaging {
    template <class ID_T, class DATA>
    class SmallDataMessage : public Message<ID_T> {
      public:
        SmallDataMessage(ID_T id, DATA data)
            : Message<ID_T>(id)
            , m_Data(data)
        {
            static_assert(sizeof(DATA) <= sizeof(std::int64_t), "sizeof(DATA) must be less than 8 bytes.");
        }

        virtual ~SmallDataMessage() = default;

        DATA GetData() const { return m_Data; }

      private:
        DATA m_Data;
    };
} // namespace Ignosi::Modules::Messaging