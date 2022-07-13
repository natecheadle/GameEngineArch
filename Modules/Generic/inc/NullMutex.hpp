#pragma once

namespace nate::Modules {
    class NullMutex {
      public:
        void lock() {}
        void unlock() {}
        bool try_lock() { return true; }
    };
}; // namespace nate::Modules