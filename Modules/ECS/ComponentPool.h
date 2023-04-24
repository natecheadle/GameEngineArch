#pragma once

#include "Component.h"
#include "Entity.h"

#include <MutexProtected.hpp>
#include <PoolMemoryBlock.hpp>
#include <UID.hpp>

#include <cstdint>
#include <map>
#include <mutex>

namespace nate::Modules::ECS
{
    template <class T>
    class ComponentPool
    {
        Memory::PoolMemoryBlock<Component<T>, std::mutex> m_MemData;

        MutexProtected<std::mutex, std::map<std::uint64_t, std::shared_ptr<Component<T>>>> m_EntityLookup;

      public:
        ComponentPool(size_t poolSize)
            : m_MemData(poolSize)
        {
        }

        ComponentPool(const ComponentPool& other) = delete;
        ComponentPool(ComponentPool&& other)      = default;

        ComponentPool& operator=(const ComponentPool& other) = delete;
        ComponentPool& operator=(ComponentPool&& other)      = default;

        std::shared_ptr<Component<T>> GetComponent(std::uint64_t id) const
        {
            auto getComponent = [id](const std::map<std::uint64_t, std::shared_ptr<Component<T>>>& components)
                -> std::shared_ptr<Component<T>> {
                auto rslt = components.find(id);
                if (rslt == components.end())
                {
                    return nullptr;
                }
                return rslt->second;
            };
            return m_EntityLookup.template execute<std::shared_ptr<Component<T>>>(getComponent);
        }

        void AddComponent(std::uint64_t id, T val)
        {
            auto addComponent = [&](std::map<std::uint64_t, std::shared_ptr<Component<T>>>& components) -> void {
                auto rslt = components.find(id);
                if (rslt != components.end())
                {
                    return;
                }
                auto newComponent = m_MemData.MakeObject(std::move(val));
                components.emplace(id, std::move(newComponent));
            };
            return m_EntityLookup.execute(addComponent);
        }

        void RemoveComponent(std::uint64_t id)
        {
            auto removeComponent = [id](std::map<std::uint64_t, std::shared_ptr<Component<T>>>& components) -> void {
                auto rslt = components.find(id);
                if (rslt != components.end())
                {
                    components.erase(rslt);
                    return;
                }
            };
            return m_EntityLookup.execute(removeComponent);
        }
    };
} // namespace nate::Modules::ECS
