#include "libdocscript/proc/utility.h"
#include "libdocscript/exception.h"
#include "libdocscript/proc/builtin.h"
#include "libdocscript/runtime/datatype.h"
#include "libdocscript/runtime/value.h"
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

namespace libdocscript::proc {
std::vector<Number>
Utility::convert_to_number_list(std::vector<Value>::const_iterator beg,
                                std::vector<Value>::const_iterator end)
{
    std::vector<Number> result;
    while (beg != end) {
        result.push_back(beg->c_cast<Number>());
        ++beg;
    }
    return result;
}

Number
Utility::number_func_helper(args_list args, func_num_1 func)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::Number)
        throw UnexceptType(DataType::to_string(DataType::Kind::Number),
                           DataType::to_string(args.front().type()));

    return func(args.front().c_cast<Number>());
}

Number
Utility::number_func_helper(args_list args, func_num_1_multiple func)
{
    if (args.size() < 1)
        throw UnexceptNumberOfArgument(1, args.size(), true);

    for (const auto& val : args) {
        if (val.type() != DataType::Kind::Number)
            throw UnexceptType(DataType::to_string(DataType::Kind::Number),
                               DataType::to_string(val.type()));
    }

    return func(args.front().c_cast<Number>(),
                convert_to_number_list(args.begin() + 1, args.end()));
}

Number
Utility::number_func_helper(args_list args, func_num_2 func)
{
    if (args.size() != 2)
        throw UnexceptNumberOfArgument(2, args.size());

    auto& first = args.front();
    auto& second = args.back();

    if (first.type() != DataType::Kind::Number)
        throw UnexceptType(DataType::to_string(DataType::Kind::Number),
                           DataType::to_string(first.type()));

    if (second.type() != DataType::Kind::Number)
        throw UnexceptType(DataType::to_string(DataType::Kind::Number),
                           DataType::to_string(second.type()));

    return func(first.c_cast<Number>(), second.c_cast<Number>());
}

Number
Utility::number_func_helper(args_list args, func_num_2_multiple func)
{
    if (args.size() < 2)
        throw UnexceptNumberOfArgument(2, args.size(), true);

    for (const auto& val : args) {
        if (val.type() != DataType::Kind::Number)
            throw UnexceptType(DataType::to_string(DataType::Kind::Number),
                               DataType::to_string(val.type()));
    }

    return func(args.begin()->c_cast<Number>(),
                (args.begin() + 1)->c_cast<Number>(),
                convert_to_number_list(args.begin() + 2, args.end()));
}

Number
Utility::number_func_helper(args_list args, func_num_multiple func)
{
    for (const auto& val : args) {
        if (val.type() != DataType::Kind::Number)
            throw UnexceptType(DataType::to_string(DataType::Kind::Number),
                               DataType::to_string(val.type()));
    }

    return func(convert_to_number_list(args.begin(), args.end()));
}

std::string
Utility::ltrim(const std::string& str)
{
    auto iter = std::find_if(
      str.begin(), str.end(), [](const char ch) { return !std::isspace(ch); });
    return iter != str.end() ? std::string(iter, str.end()) : str;
}

std::string
Utility::rtrim(const std::string& str)
{
    auto iter = std::find_if(str.rbegin(), str.rend(), [](const char ch) {
        return !std::isspace(ch);
    });

    return iter != str.rend() ? std::string(str.begin(), iter.base()) : str;
}

std::string
Utility::trim(const std::string& str)
{
    return ltrim(rtrim(str));
}

std::vector<std::string>
Utility::split(const std::string& str, const std::string &ch)
{
    std::vector<std::string> result;
    auto sstr = str;
    decltype(sstr.size()) i = str.npos;
    while((i = sstr.find(ch)) != sstr.npos) {
        auto part = std::string(sstr.begin(), sstr.begin() + i);
        result.push_back(part);
        sstr = sstr.substr(i + ch.size());
    }
    result.push_back(sstr);
    return result;
}

}