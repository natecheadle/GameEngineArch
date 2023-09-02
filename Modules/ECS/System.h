#pragma once

#include "ComponentPool.h"
#include "ISystem.h"
#include "IWorld.h"

#include <string_view>
#include <tuple>

namespace Ignosi::Modules::ECS
{
    template <typename... ComponentTypes>
    class System : public ISystem
    {
        std::uint32_t                                 m_ID;
        IWorld*                                       m_pWorld;
        std::tuple<ComponentPool<ComponentTypes>*...> m_ComponentPools;

      protected:
        System(ComponentPool<ComponentTypes>*... pools)
            : m_ComponentPools(std::move(pools)...)
        {
        }

      public:
        virtual ~System() = default;

        System(const System& other) = delete;
        System(System&& other)      = default;

        System& operator=(const System& other) = delete;
        System& operator=(System&& other)      = default;

        SystemID ID() const override { return m_ID; }
        void     Initialize(std::uint32_t id, IWorld* pWorld) override
        {
            m_ID     = id;
            m_pWorld = pWorld;
        }

      protected:
        IWorld* World() const { return m_pWorld; }

        template <class ComponentType>
        ComponentPool<ComponentType>& GetPool()
        {
            return *std::get<ComponentPool<ComponentType>*>(m_ComponentPools);
        }

        template <class ComponentType>
        const ComponentPool<ComponentType>& GetPool() const
        {
            return *std::get<ComponentPool<ComponentType>*>(m_ComponentPools);
        }
    };
} // namespace Ignosi::Modules::ECS
