#include "manifest/Scanner.hpp"

#include <algorithm>
#include <set>
#include <system_error>

namespace manifest {
namespace {

constexpr const char* kGeneratedManifest = "manifest.json";
constexpr const char* kGeneratedMetadata = "manifest_metadata.json";

std::string to_forward_slashes(std::string value) {
    std::replace(value.begin(), value.end(), '\\', '/');
    return value;
}

bool is_excluded_path(const std::string& normalized_path,
                      const std::filesystem::path& file_path) {
    const auto filename = file_path.filename().string();
    if (filename == ".env" || filename == "credentials.json" ||
        filename == kGeneratedManifest || filename == kGeneratedMetadata) {
        return true;
    }

    if (file_path.extension() == ".key") {
        return true;
    }

    constexpr const char* kPublicManifestPrefix = "public/manifests/";
    return normalized_path.rfind(kPublicManifestPrefix, 0) == 0 ||
           normalized_path == "public/manifests";
}

void add_error(ScanResult& result, DiagnosticCode code,
               const std::filesystem::path& path, const std::string& message) {
    result.complete = false;
    result.diagnostics.push_back(
        Diagnostic{code, path.generic_string(), message});
}

} // namespace

std::string Scanner::normalize_repository_path(
    const std::filesystem::path& repository_root,
    const std::filesystem::path& absolute_path) {
    const auto relative = absolute_path.lexically_normal().lexically_relative(
        repository_root.lexically_normal());
    return to_forward_slashes(relative.generic_string());
}

ScanResult Scanner::scan(const ScanOptions& options) const {
    ScanResult result;

    if (options.repository_root.empty()) {
        add_error(result, DiagnosticCode::invalid_root, {},
                  "Repository root must not be empty.");
        return result;
    }

    std::error_code ec;
    const auto root = std::filesystem::absolute(options.repository_root, ec).lexically_normal();
    if (ec || !std::filesystem::exists(root, ec) || ec ||
        !std::filesystem::is_directory(root, ec) || ec) {
        add_error(result, DiagnosticCode::invalid_root, options.repository_root,
                  ec ? ec.message() : "Repository root is not an accessible directory.");
        return result;
    }

    std::set<std::string> seen_paths;

    const auto process_file = [&](const std::filesystem::path& file_path,
                                  ScanResult& scan_result) {
        const auto normalized = normalize_repository_path(root, file_path);
        if (normalized.empty() || normalized.rfind("../", 0) == 0) {
            add_error(scan_result, DiagnosticCode::filesystem_error, file_path,
                      "Discovered path is outside the repository root.");
            return;
        }

        if (is_excluded_path(normalized, file_path)) {
            return;
        }

        if (!seen_paths.insert(normalized).second) {
            return;
        }

        std::error_code size_error;
        const auto size = std::filesystem::file_size(file_path, size_error);
        if (size_error) {
            add_error(scan_result, DiagnosticCode::filesystem_error, file_path,
                      "Unable to read file size: " + size_error.message());
            return;
        }

        std::error_code time_error;
        const auto modified = std::filesystem::last_write_time(file_path, time_error);
        if (time_error) {
            add_error(scan_result, DiagnosticCode::filesystem_error, file_path,
                      "Unable to read modification time: " + time_error.message());
            return;
        }

        const auto status = size > options.max_file_size
            ? FileStatus::skipped_large_file
            : FileStatus::indexed;

        scan_result.files.push_back(
            FileRecord{normalized, size, status, modified});
    };

    for (const auto& configured_root : options.roots) {
        const auto candidate = (root / configured_root).lexically_normal();
        std::error_code status_error;
        const auto candidate_status = std::filesystem::symlink_status(candidate, status_error);

        if (status_error) {
            add_error(result, DiagnosticCode::filesystem_error, candidate,
                      "Unable to inspect scan root: " + status_error.message());
            continue;
        }

        if (!std::filesystem::exists(candidate_status)) {
            add_error(result, DiagnosticCode::invalid_root, candidate,
                      "Configured scan root does not exist.");
            continue;
        }

        if (std::filesystem::is_symlink(candidate_status)) {
            result.diagnostics.push_back(Diagnostic{
                DiagnosticCode::symlink_skipped,
                normalize_repository_path(root, candidate),
                "Symlinked scan root was skipped."
            });
            continue;
        }

        if (std::filesystem::is_regular_file(candidate_status)) {
            process_file(candidate, result);
            continue;
        }

        if (!std::filesystem::is_directory(candidate_status)) {
            add_error(result, DiagnosticCode::invalid_root, candidate,
                      "Configured scan root is neither a regular file nor a directory.");
            continue;
        }

        std::error_code iterator_error;
        std::filesystem::recursive_directory_iterator iterator(
            candidate, std::filesystem::directory_options::none, iterator_error);
        const std::filesystem::recursive_directory_iterator end;

        if (iterator_error) {
            add_error(result, DiagnosticCode::filesystem_error, candidate,
                      "Unable to begin directory traversal: " + iterator_error.message());
            continue;
        }

        while (iterator != end) {
            const auto entry_path = iterator->path();
            std::error_code entry_error;
            const auto entry_status = iterator->symlink_status(entry_error);

            if (entry_error) {
                add_error(result, DiagnosticCode::filesystem_error, entry_path,
                          "Unable to inspect directory entry: " + entry_error.message());
            } else if (std::filesystem::is_symlink(entry_status)) {
                iterator.disable_recursion_pending();
                result.diagnostics.push_back(Diagnostic{
                    DiagnosticCode::symlink_skipped,
                    normalize_repository_path(root, entry_path),
                    "Symlinked file or directory was skipped."
                });
            } else if (std::filesystem::is_regular_file(entry_status)) {
                process_file(entry_path, result);
            }

            iterator.increment(iterator_error);
            if (iterator_error) {
                add_error(result, DiagnosticCode::filesystem_error, candidate,
                          "Directory traversal failed: " + iterator_error.message());
                iterator_error.clear();
            }
        }
    }

    std::sort(result.files.begin(), result.files.end(),
              [](const FileRecord& left, const FileRecord& right) {
                  return left.path < right.path;
              });

    return result;
}

} // namespace manifest
