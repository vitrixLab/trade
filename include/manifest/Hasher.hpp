#pragma once

#include "manifest/Types.hpp"

namespace manifest {

class Hasher {
public:
    [[nodiscard]] HashResult sha256_file(
        const std::filesystem::path& repository_root,
        const FileRecord& file) const;

    [[nodiscard]] GitProvenanceResult git_head(
        const std::filesystem::path& repository_root) const;
};

} // namespace manifest
