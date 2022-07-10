#include "DataMessage.hpp"
#include "Message.hpp"

#include <StaticPoolMemoryBlock.hpp>

#include <array>

namespace nate::Modules::Messaging {

    template <std::uint8_t* BEGIN_MESSAGE_MEMORY, size_t MESSAGE_MEMORY_SIZE>
    class MessageFactory {
        Memory::StaticPoolMemoryBlock<GenericDataMessage, BEGIN_MESSAGE_MEMORY, MESSAGE_MEMORY_SIZE> m_DataMessagePool;

      public:
        template <class ID_T, class DATA>
        std::unique_ptr<Message<ID_T>, std::function<void(Message<ID_T>*)>> CreateDataMessage(
            ID_T                  id,
            std::unique_ptr<DATA> pData)
        {
            return m_DataMessagePool
                .template MakeBaseOtherObject<DataMessage<ID_T, DATA>, Message<ID_T>, ID_T, std::unique_ptr<DATA>>(
                    std::move(id),
                    std::move(pData));
        }

        template <class ID_T>
        std::unique_ptr<Message<ID_T>, std::function<void(Message<ID_T>*)>> CreateMessage(ID_T id)
        {
            return m_DataMessagePool.template MakeOtherObject<Message<ID_T>, ID_T>(std::move(id));
        }
    };
}; // namespace nate::Modules::Messaging