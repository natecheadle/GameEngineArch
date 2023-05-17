#pragma once

#include <filesystem>
#include <map>
#include <regex>
#include <string>

namespace nate::Modules::Render
{
    class Preprocessor
    {
        // FileNames
        std::vector<std::filesystem::path> m_Includes;
        std::regex                         m_IncRegex{R"(\#include\s\".*\"\n)"};
        std::regex                         m_FileRegex{R"(\".*\")"};

      public:
        std::string PreProcess(const std::filesystem::path& file_path, const std::string& code);

      private:
        std::string ProcessInclude(const std::filesystem::path& file_path, const std::filesystem::path& path);
    };
} // namespace nate::Modules::Render