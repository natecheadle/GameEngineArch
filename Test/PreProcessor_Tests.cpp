#include <Shader/Preprocessor.h>

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>

using namespace Ignosi::Modules;

namespace Ignosi::Test
{
    namespace
    {
        void Validate(const std::filesystem::path& test_file_path, const std::filesystem::path& processed_file_path)
        {
            ASSERT_TRUE(std::filesystem::is_regular_file(test_file_path));
            ASSERT_TRUE(std::filesystem::is_regular_file(processed_file_path));

            std::ifstream     test_file(test_file_path);
            std::stringstream read_test_file;
            read_test_file << test_file.rdbuf();

            Render::Preprocessor processor;
            std::string          rslt = processor.PreProcess(test_file_path, read_test_file.str());

            std::ifstream     processed_file(processed_file_path);
            std::stringstream read_processed_file;
            read_processed_file << processed_file.rdbuf();
            std::string read_processed_file_str = read_processed_file.str();

            ASSERT_EQ(read_processed_file_str, rslt);
        }
    } // namespace

    TEST(PreProcessor_Tests, ValidateBasicInclude)
    {
        std::filesystem::path dir(std::string(TEST_SOURCE_DIR));
        dir /= "Preprocessor";

        auto test_file_path      = dir / "main.cpp";
        auto processed_file_path = dir / "processed_main.cpp";

        Validate(test_file_path, processed_file_path);
    }

    TEST(PreProcessor_Tests, ValidateIncludeInclude)
    {
        std::filesystem::path dir(std::string(TEST_SOURCE_DIR));
        dir /= "Preprocessor";

        auto test_file_path      = dir / "main2.cpp";
        auto processed_file_path = dir / "processed_main2.cpp";

        Validate(test_file_path, processed_file_path);
    }

    TEST(PreProcessor_Tests, ValidateMultiInclude)
    {
        std::filesystem::path dir(std::string(TEST_SOURCE_DIR));
        dir /= "Preprocessor";

        auto test_file_path      = dir / "main3.cpp";
        auto processed_file_path = dir / "processed_main3.cpp";

        Validate(test_file_path, processed_file_path);
    }
} // namespace Ignosi::Test