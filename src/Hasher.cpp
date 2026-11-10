#include "manifest/Hasher.hpp"

#include <openssl/err.h>
#include <openssl/evp.h>

#include <array>
#include <cerrno>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <system_error>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <fcntl.h>
#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
extern char** environ;
#endif

namespace manifest {
namespace {

constexpr std::size_t kHashChunkSize = 8U * 1024U;

void add_diagnostic(std::vector<Diagnostic>& diagnostics,
                    DiagnosticCode code,
                    const std::string& path,
                    const std::string& message) {
    diagnostics.push_back(Diagnostic{code, path, message});
}

std::string openssl_error_message(const std::string& operation) {
    const unsigned long error_code = ERR_get_error();
    std::string message = operation + " failed";
    if (error_code != 0UL) {
        std::array<char, 256> buffer{};
        ERR_error_string_n(error_code, buffer.data(), buffer.size());
        message += ": ";
        message += buffer.data();
    }
    ERR_clear_error();
    return message;
}

std::string trim_ascii(std::string value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return {};
    }
    const auto last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1U);
}

bool is_valid_git_hash(const std::string& value) {
    if (value.size() != 40U && value.size() != 64U) {
        return false;
    }
    for (const char character : value) {
        const bool is_hex = (character >= '0' && character <= '9') ||
                            (character >= 'a' && character <= 'f') ||
                            (character >= 'A' && character <= 'F');
        if (!is_hex) {
            return false;
        }
    }
    return true;
}

#ifdef _WIN32
std::string quote_windows_argument(const std::string& argument) {
    std::string quoted = "\"";
    std::size_t backslashes = 0U;

    for (const char character : argument) {
        if (character == '\\') {
            ++backslashes;
        } else if (character == '"') {
            quoted.append(backslashes * 2U + 1U, '\\');
            quoted.push_back('"');
            backslashes = 0U;
        } else {
            quoted.append(backslashes, '\\');
            backslashes = 0U;
            quoted.push_back(character);
        }
    }

    quoted.append(backslashes * 2U, '\\');
    quoted.push_back('"');
    return quoted;
}
#endif

} // namespace

HashResult Hasher::sha256_file(
    const std::filesystem::path& repository_root,
    const FileRecord& file) const {
    HashResult result;
    if (file.status != FileStatus::indexed) {
        add_diagnostic(result.diagnostics, DiagnosticCode::file_not_hashable, file.path,
                       "Files marked as skipped cannot be hashed.");
        return result;
    }

    const auto absolute_path = repository_root / std::filesystem::path(file.path);

    std::error_code size_error;
    const auto initial_size = std::filesystem::file_size(absolute_path, size_error);
    if (size_error || initial_size != file.size) {
        add_diagnostic(result.diagnostics, DiagnosticCode::file_changed, file.path,
                       size_error ? "Unable to re-check file size: " + size_error.message()
                                  : "File size changed after discovery.");
        return result;
    }

    if (file.modified_time.has_value()) {
        std::error_code time_error;
        const auto current_time = std::filesystem::last_write_time(absolute_path, time_error);
        if (time_error || current_time != *file.modified_time) {
            add_diagnostic(result.diagnostics, DiagnosticCode::file_changed, file.path,
                           time_error ? "Unable to re-check modification time: " + time_error.message()
                                      : "File modification time changed after discovery.");
            return result;
        }
    }

    std::ifstream input(absolute_path, std::ios::binary);
    if (!input.is_open()) {
        add_diagnostic(result.diagnostics, DiagnosticCode::open_failed, file.path,
                       "Unable to open file in binary mode.");
        return result;
    }

    ERR_clear_error();
    using EvpContext = std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)>;
    EvpContext context(EVP_MD_CTX_new(), &EVP_MD_CTX_free);
    if (!context) {
        add_diagnostic(result.diagnostics, DiagnosticCode::openssl_failure, file.path,
                       openssl_error_message("EVP_MD_CTX_new"));
        return result;
    }

    ERR_clear_error();
    if (EVP_DigestInit_ex(context.get(), EVP_sha256(), nullptr) != 1) {
        add_diagnostic(result.diagnostics, DiagnosticCode::openssl_failure, file.path,
                       openssl_error_message("EVP_DigestInit_ex"));
        return result;
    }

    std::array<unsigned char, kHashChunkSize> buffer{};
    while (input) {
        input.read(reinterpret_cast<char*>(buffer.data()),
                   static_cast<std::streamsize>(buffer.size()));
        const auto count = input.gcount();

        if (count > 0) {
            ERR_clear_error();
            if (EVP_DigestUpdate(context.get(), buffer.data(),
                                 static_cast<std::size_t>(count)) != 1) {
                add_diagnostic(result.diagnostics, DiagnosticCode::openssl_failure, file.path,
                               openssl_error_message("EVP_DigestUpdate"));
                return result;
            }
            result.bytes_read += static_cast<std::uintmax_t>(count);
        }
    }

    if (input.bad()) {
        add_diagnostic(result.diagnostics, DiagnosticCode::read_failed, file.path,
                       "I/O error while reading file.");
        return result;
    }

    if (result.bytes_read != file.size) {
        add_diagnostic(result.diagnostics, DiagnosticCode::file_changed, file.path,
                       "Bytes read differ from the size recorded during discovery.");
        return result;
    }

    std::error_code final_size_error;
    const auto final_size = std::filesystem::file_size(absolute_path, final_size_error);
    std::error_code final_time_error;
    const auto final_time = std::filesystem::last_write_time(absolute_path, final_time_error);

    if (final_size_error || final_time_error || final_size != file.size ||
        (file.modified_time.has_value() && final_time != *file.modified_time)) {
        add_diagnostic(result.diagnostics, DiagnosticCode::file_changed, file.path,
                       "File changed while it was being hashed.");
        return result;
    }

    std::array<unsigned char, EVP_MAX_MD_SIZE> digest{};
    unsigned int digest_length = 0U;
    ERR_clear_error();
    if (EVP_DigestFinal_ex(context.get(), digest.data(), &digest_length) != 1) {
        add_diagnostic(result.diagnostics, DiagnosticCode::openssl_failure, file.path,
                       openssl_error_message("EVP_DigestFinal_ex"));
        return result;
    }

    std::ostringstream hexadecimal;
    hexadecimal << std::hex << std::setfill('0');
    for (unsigned int index = 0U; index < digest_length; ++index) {
        hexadecimal << std::setw(2) << static_cast<unsigned int>(digest[index]);
    }

    result.sha256 = hexadecimal.str();
    ERR_clear_error();
    return result;
}

GitProvenanceResult Hasher::git_head(
    const std::filesystem::path& repository_root) const {
    GitProvenanceResult result;

#ifdef _WIN32
    SECURITY_ATTRIBUTES security_attributes{};
    security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    security_attributes.bInheritHandle = TRUE;

    HANDLE read_pipe = nullptr;
    HANDLE write_pipe = nullptr;
    if (!CreatePipe(&read_pipe, &write_pipe, &security_attributes, 0U)) {
        add_diagnostic(result.diagnostics, DiagnosticCode::git_unavailable, {},
                       "Unable to create a pipe for Git provenance.");
        return result;
    }
    SetHandleInformation(read_pipe, HANDLE_FLAG_INHERIT, 0U);

    const std::string command =
        quote_windows_argument("git") + " " +
        quote_windows_argument("rev-parse") + " " +
        quote_windows_argument("--verify") + " " +
        quote_windows_argument("HEAD");

    STARTUPINFOA startup_info{};
    startup_info.cb = sizeof(STARTUPINFOA);
    startup_info.dwFlags = STARTF_USESTDHANDLES;
    startup_info.hStdOutput = write_pipe;
    startup_info.hStdError = write_pipe;
    startup_info.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

    PROCESS_INFORMATION process_info{};
    std::string mutable_command = command;
    const auto working_directory = repository_root.string();

    const BOOL created = CreateProcessA(
        nullptr, mutable_command.data(), nullptr, nullptr, TRUE,
        CREATE_NO_WINDOW, nullptr, working_directory.c_str(),
        &startup_info, &process_info);

    CloseHandle(write_pipe);
    if (!created) {
        CloseHandle(read_pipe);
        add_diagnostic(result.diagnostics, DiagnosticCode::git_unavailable, {},
                       "Git is unavailable or the repository directory cannot be opened.");
        return result;
    }

    std::string output;
    std::array<char, 256> buffer{};
    DWORD bytes_read = 0U;
    while (ReadFile(read_pipe, buffer.data(),
                    static_cast<DWORD>(buffer.size()), &bytes_read, nullptr) &&
           bytes_read > 0U) {
        output.append(buffer.data(), static_cast<std::size_t>(bytes_read));
    }
    CloseHandle(read_pipe);

    WaitForSingleObject(process_info.hProcess, INFINITE);
    DWORD exit_code = 1U;
    GetExitCodeProcess(process_info.hProcess, &exit_code);
    CloseHandle(process_info.hThread);
    CloseHandle(process_info.hProcess);

    output = trim_ascii(output);
    if (exit_code == 0U && is_valid_git_hash(output)) {
        result.commit_hash = output;
    } else {
        add_diagnostic(result.diagnostics, DiagnosticCode::git_unavailable, {},
                       "Git could not resolve HEAD in the repository.");
    }
#else
    int pipe_descriptors[2]{-1, -1};
    if (pipe(pipe_descriptors) != 0) {
        add_diagnostic(result.diagnostics, DiagnosticCode::git_unavailable, {},
                       "Unable to create a pipe for Git provenance.");
        return result;
    }

    posix_spawn_file_actions_t actions;
    if (posix_spawn_file_actions_init(&actions) != 0) {
        close(pipe_descriptors[0]);
        close(pipe_descriptors[1]);
        add_diagnostic(result.diagnostics, DiagnosticCode::git_unavailable, {},
                       "Unable to initialize Git process file actions.");
        return result;
    }

    bool actions_ok = true;
    actions_ok = actions_ok &&
        posix_spawn_file_actions_adddup2(&actions, pipe_descriptors[1], STDOUT_FILENO) == 0;
    actions_ok = actions_ok &&
        posix_spawn_file_actions_addclose(&actions, pipe_descriptors[0]) == 0;
    actions_ok = actions_ok &&
        posix_spawn_file_actions_addclose(&actions, pipe_descriptors[1]) == 0;

    if (!actions_ok) {
        posix_spawn_file_actions_destroy(&actions);
        close(pipe_descriptors[0]);
        close(pipe_descriptors[1]);
        add_diagnostic(result.diagnostics, DiagnosticCode::git_unavailable, {},
                       "Unable to configure Git process file actions.");
        return result;
    }

    const auto repository_argument = repository_root.string();
    char* const arguments[] = {
        const_cast<char*>("git"),
        const_cast<char*>("-C"),
        const_cast<char*>(repository_argument.c_str()),
        const_cast<char*>("rev-parse"),
        const_cast<char*>("--verify"),
        const_cast<char*>("HEAD"),
        nullptr
    };

    pid_t process_id = 0;
    const int spawn_status = posix_spawnp(
        &process_id, "git", &actions, nullptr, arguments, environ);
    posix_spawn_file_actions_destroy(&actions);
    close(pipe_descriptors[1]);

    if (spawn_status != 0) {
        close(pipe_descriptors[0]);
        add_diagnostic(result.diagnostics, DiagnosticCode::git_unavailable, {},
                       "Git is unavailable or process creation failed.");
        return result;
    }

    std::string output;
    std::array<char, 256> buffer{};
    ssize_t bytes_read = 0;
    while ((bytes_read = read(pipe_descriptors[0], buffer.data(), buffer.size())) > 0) {
        output.append(buffer.data(), static_cast<std::size_t>(bytes_read));
    }
    close(pipe_descriptors[0]);

    int child_status = 0;
    pid_t wait_result = -1;
    do {
        wait_result = waitpid(process_id, &child_status, 0);
    } while (wait_result < 0 && errno == EINTR);

    if (wait_result != process_id) {
        add_diagnostic(result.diagnostics, DiagnosticCode::git_unavailable, {},
                       "Unable to wait for the Git provenance process.");
        return result;
    }

    output = trim_ascii(output);
    if (WIFEXITED(child_status) && WEXITSTATUS(child_status) == 0 &&
        is_valid_git_hash(output)) {
        result.commit_hash = output;
    } else {
        add_diagnostic(result.diagnostics, DiagnosticCode::git_unavailable, {},
                       "Git could not resolve HEAD in the repository.");
    }
#endif

    return result;
}

} // namespace manifest
