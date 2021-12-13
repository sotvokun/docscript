#ifndef LIBDOCSCRIPT_UTILITY_POSITION_H
#define LIBDOCSCRIPT_UTILITY_POSITION_H
#include <cstdint>
#include <sstream>
#include <string>
#include <tuple>

namespace libdocscript {
class PositionDiff final
{
public:
    using value_type = long long;

    PositionDiff(value_type ln, value_type col) : line(ln), column(col) {}
    ~PositionDiff() = default;

    value_type line;
    value_type column;

    inline const value_type& line_ref() const
    {
        return line;
    }
    inline const value_type& column_ref() const
    {
        return column;
    }
};

class Position final
{
public:
    using value_type = unsigned long long;

    Position() : line(0), column(0) {}
    Position(value_type ln, value_type col) : line(ln), column(col) {}
    ~Position() = default;

    inline PositionDiff operator-(const Position& rhs)
    {
        return PositionDiff(line - rhs.line, column - rhs.column);
    }

    value_type line;
    value_type column;

    inline const value_type& line_ref() const
    {
        return line;
    }
    inline const value_type& column_ref() const
    {
        return column;
    }

    inline std::string to_string() const
    {
        std::ostringstream oss;
        oss << line << ":" << column;
        return oss.str();
    }
};

} // namespace libdocscript

#endif