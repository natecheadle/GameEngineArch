#pragma once

#include "MutexProtected.hpp"
#include "NullMutex.hpp"

#include <memory>
#include <utility>

namespace Ignosi::Modules
{
    template <class T, class MUTEX = NullMutex>
    class Singleton
    {
        static MutexProtected<MUTEX, std::unique_ptr<T>> m_Singleton;
        static MUTEX                                     m_Mutex;

      public:
        static void SetInstance(std::unique_ptr<T> pSingleton)
        {
            m_Singleton.execute([&](std::unique_ptr<T>& singleton) { singleton = std::move(pSingleton); });
        }

        static T* Instance()
        {
            return m_Singleton.template execute<T*>(
                [](std::unique_ptr<T>& singleton) -> T* { return singleton.get(); });
        }

        static void Reset()
        {
            m_Singleton.execute([](std::unique_ptr<T>& singleton) { singleton.reset(); });
        }
    };

    template <class T, class MUTEX>
    MUTEX Singleton<T, MUTEX>::m_Mutex = MUTEX();

    template <class T, class MUTEX>
    MutexProtected<MUTEX, std::unique_ptr<T>> Singleton<T, MUTEX>::m_Singleton =
        MutexProtected<MUTEX, std::unique_ptr<T>>();

} // namespace Ignosi::Modules
