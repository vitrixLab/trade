#include "manifest/Hasher.hpp"
#include "manifest/Scanner.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <fstream>
#include <string>

namespace {

class TemporaryDirectory {
public:
    TemporaryDirectory() {
        const auto unique = std::chrono::steady_clock::now().time_since_epoch().count();
        path_ = std::filesystem::temp_directory_path() /
                ("manifest-hasher-test-" + std::to_string(unique));
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

manifest::FileRecord record_for(const std::filesystem::path& root,
                                const std::filesystem::path& relative_path) {
    const auto absolute = root / relative_path;
    return manifest::FileRecord{
        relative_path.generic_string(),
        std::filesystem::file_size(absolute),
        manifest::FileStatus::indexed,
        std::filesystem::last_write_time(absolute)
    };
}

void write_bytes(const std::filesystem::path& path, const std::string& bytes) {
    std::ofstream output(path, std::ios::binary);
    output.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
}

} // namespace

TEST(HasherTests, ProducesKnownSha256ForText) {
    TemporaryDirectory temporary;
    write_bytes(temporary.path() / "sample.txt", "abc");

    const auto file = record_for(temporary.path(), "sample.txt");
    const auto result = manifest::Hasher{}.sha256_file(temporary.path(), file);

    ASSERT_TRUE(result.success());
    EXPECT_EQ(result.sha256.value(), "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
    EXPECT_EQ(result.bytes_read, 3U);
}

TEST(HasherTests, HashesBinaryBytesWithoutTextNormalization) {
    TemporaryDirectory temporary;
    const std::string bytes{"line1\r\nline2\n\0tail", 18};
    write_bytes(temporary.path() / "binary.dat", bytes);

    const auto file = record_for(temporary.path(), "binary.dat");
    const auto result = manifest::Hasher{}.sha256_file(temporary.path(), file);

    ASSERT_TRUE(result.success());
    EXPECT_EQ(result.bytes_read, bytes.size());
    EXPECT_EQ(result.sha256.value(),
              "a288c7b7f0a50455e581b5cb4f5831c8ba8825b9981cbabf71819da755e7a729");
}

TEST(HasherTests, RejectsFileChangedAfterDiscovery) {
    TemporaryDirectory temporary;
    write_bytes(temporary.path() / "mutable.txt", "before");

    auto file = record_for(temporary.path(), "mutable.txt");
    write_bytes(temporary.path() / "mutable.txt", "after-content");

    const auto result = manifest::Hasher{}.sha256_file(temporary.path(), file);

    EXPECT_FALSE(result.success());
    ASSERT_FALSE(result.diagnostics.empty());
    EXPECT_EQ(result.diagnostics.front().code, manifest::DiagnosticCode::file_changed);
    EXPECT_FALSE(result.sha256.has_value());
}

TEST(HasherTests, HashesEmptyFile) {
    TemporaryDirectory temporary;
    write_bytes(temporary.path() / "empty.bin", "");

    const auto file = record_for(temporary.path(), "empty.bin");
    const auto result = manifest::Hasher{}.sha256_file(temporary.path(), file);

    ASSERT_TRUE(result.success());
    EXPECT_EQ(result.sha256.value(), "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}

TEST(HasherTests, ReportsUnavailableGitProvenanceWithoutCrashing) {
    TemporaryDirectory temporary;

    const auto result = manifest::Hasher{}.git_head(temporary.path());

    EXPECT_FALSE(result.commit_hash.has_value());
    EXPECT_FALSE(result.diagnostics.empty());
    EXPECT_EQ(result.diagnostics.front().code, manifest::DiagnosticCode::git_unavailable);
}

TEST(HasherTests, StreamsFilesLargerThanOneHashChunk) {
    TemporaryDirectory temporary;
    const std::string bytes(32U * 1024U, 'x');
    write_bytes(temporary.path() / "multi-chunk.bin", bytes);

    const auto file = record_for(temporary.path(), "multi-chunk.bin");
    const auto result = manifest::Hasher{}.sha256_file(temporary.path(), file);

    ASSERT_TRUE(result.success());
    EXPECT_EQ(result.bytes_read, bytes.size());
    EXPECT_EQ(result.sha256->size(), 64U);
}
