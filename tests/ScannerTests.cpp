#include "manifest/Scanner.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <fstream>
#include <string>
#include <vector>

namespace {

class TemporaryDirectory {
public:
    TemporaryDirectory() {
        const auto unique = std::chrono::steady_clock::now().time_since_epoch().count();
        path_ = std::filesystem::temp_directory_path() /
                ("manifest-scanner-test-" + std::to_string(unique));
        std::filesystem::create_directories(path_);
    }

    ~TemporaryDirectory() {
        std::error_code ignored;
        std::filesystem::remove_all(path_, ignored);
    }

    [[nodiscard]] const std::filesystem::path& path() const { return path_; }

private:
    std::filesystem::path path_;
};

void write_file(const std::filesystem::path& path, const std::string& content) {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream output(path, std::ios::binary);
    output.write(content.data(), static_cast<std::streamsize>(content.size()));
}

manifest::ScanOptions options_for(const std::filesystem::path& root) {
    manifest::ScanOptions options;
    options.repository_root = root;
    options.roots = {"README.md", "docs"};
    return options;
}

} // namespace

TEST(ScannerTests, DiscoversFilesAndNormalizesRepositoryPaths) {
    TemporaryDirectory temporary;
    write_file(temporary.path() / "README.md", "root");
    write_file(temporary.path() / "docs" / "nested" / "guide.md", "guide");

    const auto result = manifest::Scanner{}.scan(options_for(temporary.path()));

    ASSERT_TRUE(result.complete);
    ASSERT_EQ(result.files.size(), 2U);
    EXPECT_EQ(result.files[0].path, "README.md");
    EXPECT_EQ(result.files[1].path, "docs/nested/guide.md");
    EXPECT_EQ(result.files[0].status, manifest::FileStatus::indexed);
}

TEST(ScannerTests, ExcludesSecretsGeneratedFilesAndKeyFiles) {
    TemporaryDirectory temporary;
    write_file(temporary.path() / "README.md", "root");
    write_file(temporary.path() / "docs" / ".env", "secret");
    write_file(temporary.path() / "docs" / "credentials.json", "secret");
    write_file(temporary.path() / "docs" / "private.key", "secret");
    write_file(temporary.path() / "docs" / "manifest.json", "{}");
    write_file(temporary.path() / "docs" / "manifest_metadata.json", "{}");
    write_file(temporary.path() / "docs" / "keep.md", "safe");
    write_file(temporary.path() / "public" / "manifests" / "index.json", "{}");

    auto options = options_for(temporary.path());
    options.roots.push_back("public");
    const auto result = manifest::Scanner{}.scan(options);

    ASSERT_TRUE(result.complete);
    ASSERT_EQ(result.files.size(), 2U);
    EXPECT_EQ(result.files[0].path, "README.md");
    EXPECT_EQ(result.files[1].path, "docs/keep.md");
}

TEST(ScannerTests, RecordsOversizedFilesWithoutOpeningThem) {
    TemporaryDirectory temporary;
    write_file(temporary.path() / "README.md", "root");
    const auto large_path = temporary.path() / "docs" / "large.bin";
    std::filesystem::create_directories(large_path.parent_path());
    {
        std::ofstream output(large_path, std::ios::binary);
        output.seekp(static_cast<std::streamoff>(1024U * 1024U));
        output.put('\0');
    }

    auto options = options_for(temporary.path());
    options.max_file_size = 1024U;

    const auto result = manifest::Scanner{}.scan(options);

    ASSERT_TRUE(result.complete);
    ASSERT_EQ(result.files.size(), 2U);
    EXPECT_EQ(result.files[1].path, "docs/large.bin");
    EXPECT_EQ(result.files[1].status, manifest::FileStatus::skipped_large_file);
    EXPECT_GT(result.files[1].size, options.max_file_size);
}

TEST(ScannerTests, SkipsSymlinkedFilesAndDirectories) {
    TemporaryDirectory temporary;
    TemporaryDirectory outside;
    write_file(temporary.path() / "README.md", "root");
    write_file(outside.path() / "outside.md", "outside");
    write_file(temporary.path() / "docs" / "real.md", "real");

    std::error_code file_link_error;
    std::filesystem::create_symlink(
        outside.path() / "outside.md",
        temporary.path() / "docs" / "linked.md",
        file_link_error);

    std::error_code directory_link_error;
    std::filesystem::create_directory_symlink(
        outside.path(),
        temporary.path() / "docs" / "linked-directory",
        directory_link_error);

    if (file_link_error || directory_link_error) {
        GTEST_SKIP() << "Symlink creation is not permitted on this platform.";
    }

    const auto result = manifest::Scanner{}.scan(options_for(temporary.path()));

    ASSERT_TRUE(result.complete);
    ASSERT_EQ(result.files.size(), 2U);
    EXPECT_EQ(result.files[0].path, "README.md");
    EXPECT_EQ(result.files[1].path, "docs/real.md");
    EXPECT_GE(result.diagnostics.size(), 2U);
}

TEST(ScannerTests, ReportsMissingConfiguredRootsAsIncomplete) {
    TemporaryDirectory temporary;
    write_file(temporary.path() / "README.md", "root");

    const auto result = manifest::Scanner{}.scan(options_for(temporary.path()));

    EXPECT_FALSE(result.complete);
    EXPECT_FALSE(result.diagnostics.empty());
}

TEST(ScannerTests, RejectsEmptyRepositoryRoot) {
    manifest::ScanOptions options;
    options.repository_root.clear();

    const auto result = manifest::Scanner{}.scan(options);

    EXPECT_FALSE(result.complete);
    ASSERT_FALSE(result.diagnostics.empty());
    EXPECT_EQ(result.diagnostics.front().code, manifest::DiagnosticCode::invalid_root);
}

TEST(ScannerTests, NormalizesWindowsSeparators) {
    const auto normalized = manifest::Scanner::normalize_repository_path(
        std::filesystem::path("C:/repo"),
        std::filesystem::path("C:/repo/docs/research.md"));

    EXPECT_EQ(normalized, "docs/research.md");
}
