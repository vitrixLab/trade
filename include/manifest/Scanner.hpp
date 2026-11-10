#pragma once

#include "manifest/Types.hpp"

namespace manifest {

class Scanner {
public:
    [[nodiscard]] ScanResult scan(const ScanOptions& options) const;
    [[nodiscard]] static std::string normalize_repository_path(
        const std::filesystem::path& repository_root,
        const std::filesystem::path& absolute_path);
};

} // namespace manifest
