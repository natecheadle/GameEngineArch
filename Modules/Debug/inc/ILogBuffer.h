#pragma once

#include <string>
#include <vector>

namespace amc::Shared {

    class ILogBuffer {
      public:
        virtual std::vector<std::string> RetrieveAllLogs()       = 0;
        virtual std::vector<std::string> RetrieveNLogs(size_t n) = 0;

      protected:
        ILogBuffer()  = default;
        ~ILogBuffer() = default;
    };
} // namespace amc::Shared
