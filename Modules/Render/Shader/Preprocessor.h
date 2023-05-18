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
        std::vector<std::filesystem::path> m_IncludePaths;
        std::regex                         m_IncRegex{R"(\#include\s\".*\"\n)"};
        std::regex                         m_FileRegex{R"(\".*\")"};

      public:
        std::string PreProcess(const std::filesystem::path& file_path, const std::string& code);
        void        AddIncludePath(std::filesystem::path path) { m_IncludePaths.push_back(std::move(path)); }
        void        IncludePaths(std::vector<std::filesystem::path> paths) { m_IncludePaths = std::move(paths); }
        const std::vector<std::filesystem::path>& IncludePaths() const { return m_IncludePaths; }

      private:
        std::string           ProcessInclude(const std::filesystem::path& file_path, const std::filesystem::path& path);
        std::filesystem::path FindInclude(const std::filesystem::path& file_path, const std::filesystem::path& path);
    };
} // namespace nate::Modules::Render