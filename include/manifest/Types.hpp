#pragma once

#include "manifest/Types.hpp"

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace manifest {

enum class FileStatus {
    indexed,
    skipped_large_file
};

enum class DiagnosticCode {
    invalid_root,
    filesystem_error,
    symlink_skipped,
    file_changed,
    file_not_hashable,
    open_failed,
    read_failed,
    openssl_failure,
    git_unavailable
};

struct Diagnostic {
    DiagnosticCode code{DiagnosticCode::filesystem_error};
    std::string path;
    std::string message;
};

struct FileRecord {
    std::string path;
    std::uintmax_t size{0};
    FileStatus status{FileStatus::indexed};
    std::optional<std::filesystem::file_time_type> modified_time;
};

struct ScanOptions {
    std::filesystem::path repository_root;
    std::vector<std::filesystem::path> roots{
        "README.md",
        "docs",
        "schemas",
        "examples",
        "api",
        "config"
    };
    std::uintmax_t max_file_size{10ULL * 1024ULL * 1024ULL};
};

struct ScanResult {
    std::vector<FileRecord> files;
    std::vector<Diagnostic> diagnostics;
    bool complete{true};
};

struct HashResult {
    std::optional<std::string> sha256;
    std::uintmax_t bytes_read{0};
    std::vector<Diagnostic> diagnostics;

    [[nodiscard]] bool success() const noexcept {
        return sha256.has_value() && diagnostics.empty();
    }
};

struct GitProvenanceResult {
    std::optional<std::string> commit_hash;
    std::vector<Diagnostic> diagnostics;
};

} // namespace manifest
