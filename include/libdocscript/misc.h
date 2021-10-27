#ifndef LIBDOCSCRIPT_MISC_H
#define LIBDOCSCRIPT_MISC_H
#include "docscript.h"
#include <algorithm>
#include <sstream>
#include <string>

namespace docscript::libdocscript {

///
/// \brief The line and column information of the source input
///
class Position {
  public:
    inline operator std::string() const {
        std::ostringstream oss;
        oss << "line " << line << " column " << column;
        return oss.str();
    }

    inline bool operator==(const Position &pos) const {
        return line == pos.line && column == pos.column;
    }

    inline bool operator!=(const Position &pos) const {
        return !this->operator==(pos);
    }

    size_type line;
    size_type column;
};

///
/// \brief Return a value inidicating whether a specific character occurs with a
/// string
///
inline static bool contain(const std::string &str, unsigned char ch) {
    return str.find(ch) != std::string::npos;
}

///
/// \brief Return a value indicating whether a string only has space character
///
inline static bool isspace(const std::string &str) {
    return str.size() ==
           std::count_if(str.begin(), str.end(), [](const auto &ch) {
               return contain(" \f\n\r\t\v", ch);
           });
}

///
/// \brief Return the position of the string.
///
inline static Position locate_position(Position curr, const std::string &str) {
    curr.column -= str.size();
    return curr;
}

inline static std::string string_trim(std::string str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), str.end());
    return str;
}
} // namespace libdocscript

#endif