#pragma once

namespace nate::Modules::Messaging {
    template <class ID_T>
    class Message {
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
} // namespace nate::Modules::Messaging