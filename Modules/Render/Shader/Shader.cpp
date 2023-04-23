#include "Shader.h"

#include <fmt/format.h>

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <ios>
#include <limits>
#include <stdexcept>
#include <system_error>

namespace nate::Modules::Render
{

    Shader::Shader(std::filesystem::path shadersPath)
        : m_Path(std::move(shadersPath))
        , m_Data(ReadFileData(m_Path))
    {
    }
    Shader::~Shader() {}

    std::vector<std::uint8_t> Shader::ReadFileData(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path))
            throw std::invalid_argument(fmt::format("Shader file does not exist at path {}.", path.string()));

        std::uintmax_t size = std::filesystem::file_size(path);
        assert(size < (std::uintmax_t)std::numeric_limits<std::streamsize>::max());
        std::ifstream file(path, std::ios::binary);

        if (!file.is_open())
            throw std::runtime_error(fmt::format("Failed to open file {}.", path.string()));

        std::vector<std::uint8_t> data(size);
        file.read((char*)(data.data()), (std::streamsize)size);
        data.back() = '\0';

        file.close();

        return std::move(data);
    }
} // namespace nate::Modules::Render