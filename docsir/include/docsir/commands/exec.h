#ifndef DOCSIR_COMMANDS_EXEC_H
#define DOCSIR_COMMANDS_EXEC_H
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>

namespace docsir {
class EXEC final
{
  public:
    static bool is_exist(const std::string& path);
    static std::string full_path(const std::string& path);
    static std::string read_content(const std::string &path);
};

bool
EXEC::is_exist(const std::string& path)
{
    return std::filesystem::exists(path);
}

std::string
EXEC::full_path(const std::string& path)
{
    return std::filesystem::absolute(path).string();
}

std::string
EXEC::read_content(const std::string &path)
{
    std::ostringstream oss;
    std::ifstream file(full_path(path));
    oss << file.rdbuf();
    return oss.str();
}

}

#endif