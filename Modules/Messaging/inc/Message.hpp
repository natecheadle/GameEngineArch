#pragma once

#include <type_traits>
#include <cstdint>

namespace nate::Modules::Messaging {
    template <class ID_T>
    class Message {
        static_assert(std::is_integral_v<ID_T> || std::is_enum_v<ID_T>, "ID Type must be integral or enum type.");
      public:
        Message(ID_T id)
            : m_ID(id)
        {
        }

        virtual ~Message() = default;

        ID_T ID() const { return m_ID; }

      private:
        ID_T m_ID;
    };

    using GenericMessage = Message<std::int64_t>;
} // namespace nate::Modules::Messaging