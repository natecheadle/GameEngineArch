#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string_view>
#include <vector>

namespace nate::Modules::Render {
    class Shader {
      protected:
        Shader(std::filesystem::path shadersPath);

      public:
        virtual ~Shader();

        const std::filesystem::path&     Path() const { return m_Path; }
        const std::vector<std::uint8_t>& ShaderData() const { return m_Data; }

      private:
        std::vector<std::uint8_t> m_Data;
        std::filesystem::path     m_Path;

        static std::vector<std::uint8_t> ReadFileData(const std::filesystem::path& path);
    };
} // namespace nate::Modules::Render