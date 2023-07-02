#include <cstdint>

namespace nate::Modules::ECS
{
    class IEntity
    {
      public:
        virtual ~IEntity()               = default;
        virtual std::uint64_t ID() const = 0;
    };
} // namespace nate::Modules::ECS
