#include "Preprocessor.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace nate::Modules::Render
{
    std::string Preprocessor::PreProcess(const std::filesystem::path& file_path, const std::string& code)
    {
        auto inc_iterator_begin = std::sregex_iterator(code.begin(), code.end(), m_IncRegex);

        std::string::const_iterator lastPos = code.cbegin();
        if (inc_iterator_begin == std::sregex_iterator())
            return code;

        std::string rslt{code};

        for (auto inc = inc_iterator_begin; inc != std::sregex_iterator(); ++inc)
        {
            std::smatch sm;
            std::string inc_str((*inc).str());
            if (std::regex_search(inc_str, sm, m_FileRegex))
            {
                std::string           match_str{sm.str()};
                std::filesystem::path inc_path(std::string_view(match_str.begin() + 1, match_str.end() - 1));
                std::string           code = ProcessInclude(file_path, inc_path);

                size_t pos = rslt.find(inc_str);
                if (pos != std::string::npos)
                {
                    rslt.replace(pos, inc_str.length(), code);
                }
            }
        }

        return rslt;
    }

    std::string Preprocessor::ProcessInclude(const std::filesystem::path& file_path, const std::filesystem::path& path)
    {
        std::filesystem::path inc_path{Preprocessor::FindInclude(file_path, path)};

        auto openedInc = std::find(m_Includes.begin(), m_Includes.end(), inc_path);
        if (openedInc != m_Includes.end())
        {
            return "\n";
        }

        std::ifstream     file(inc_path.string());
        std::stringstream file_str;
        file_str << file.rdbuf();

        std::string inc_file = file_str.str();
        if (std::regex_search(inc_file, m_IncRegex))
            return PreProcess(inc_path, inc_file) + '\n';
        return inc_file + '\n';
    }

    std::filesystem::path Preprocessor::FindInclude(
        const std::filesystem::path& file_path,
        const std::filesystem::path& path)
    {
        if (std::filesystem::is_regular_file(path))
            return path;

        std::filesystem::path inc_path = file_path.parent_path() / path;
        if (std::filesystem::is_regular_file(inc_path))
        {
            return inc_path;
        }

        for (auto& inc_path_search : m_IncludePaths)
        {
            inc_path = inc_path_search / path;
            if (std::filesystem::is_regular_file(inc_path))
                return inc_path;
        }

        throw std::runtime_error("Include file not found: " + path.string());
    }
} // namespace nate::Modules::Render