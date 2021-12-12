#include "libdocscript/exception.h"
#include "libdocscript/proc/builtin.h"
#include "libdocscript/proc/utility.h"
#include "libdocscript/runtime/list.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/runtime/value.h"
#include <string>

namespace libdocscript::proc {
Value
string_is(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    auto& first = args.front();
    return Boolean(first.type() == DataType::Kind::String);
}

Value
string_eq(args_list args, Environment& env)
{
    if (args.size() < 1)
        throw UnexceptNumberOfArgument(1, args.size(), true);

    for (const auto& val : args) {
        if (val.type() != DataType::Kind::String)
            throw UnexceptType(DataType::to_string(DataType::Kind::String),
                               DataType::to_string(val.type()));
    }

    auto& first = args.front();
    for (auto beg = args.begin() + 1; beg != args.end(); ++beg) {
        if (first != *beg)
            return Boolean(false);
    }
    return Boolean(true);
}

Value
string_to_symbol(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    return Symbol(args.front());
}

Value
string_to_boolean(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    return Boolean(args.front());
}

Value
string_make_string(args_list args, Environment& env)
{
    if (args.size() != 2)
        throw UnexceptNumberOfArgument(2, args.size());

    auto& first = args.front();
    auto& second = args.back();

    if (first.type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(first.type()));

    if (second.type() != DataType::Kind::Number)
        throw UnexceptType(DataType::to_string(DataType::Kind::Number),
                           DataType::to_string(second.type()));

    std::string content;
    for (Number::int_type i = 0; i != second.c_cast<Number>().integer(); ++i) {
        content.append(first.c_cast<String>());
    }
    return String(content);
}

Value
string_gt_is(args_list args, Environment& env)
{
    if (args.size() != 2)
        throw UnexceptNumberOfArgument(2, args.size());

    auto& first = args.front();
    auto& second = args.back();

    if (first.type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(first.type()));

    if (second.type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(second.type()));

    return Boolean(first.c_cast<String>().const_value() >
                   second.c_cast<String>().const_value());
}

Value
string_lt_is(args_list args, Environment& env)
{
    if (args.size() != 2)
        throw UnexceptNumberOfArgument(2, args.size());

    auto& first = args.front();
    auto& second = args.back();

    if (first.type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(first.type()));

    if (second.type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(second.type()));

    return Boolean(first.c_cast<String>().const_value() >
                   second.c_cast<String>().const_value());
}

Value
string_length(args_list args, Environment& env)
{
    if (args.size() != 2)
        throw UnexceptNumberOfArgument(2, args.size());

    auto& first = args.front();
    auto& second = args.back();

    if (first.type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(first.type()));

    if (second.type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(second.type()));

    return Boolean(first.c_cast<String>().const_value() <
                   second.c_cast<String>().const_value());
}

Value
string_append(args_list args, Environment& env)
{
    if (args.size() < 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    std::string content;
    for (auto beg = args.begin(); beg != args.end(); ++beg) {
        content.append(static_cast<std::string>(*beg));
    }
    return String(content);
}

Value
string_concat(args_list args, Environment& env)
{
    if (args.size() < 1)
        throw UnexceptNumberOfArgument(1, args.size());

    for (const auto& val : args) {
        if (val.type() != DataType::Kind::String)
            throw UnexceptType(DataType::to_string(DataType::Kind::String),
                               DataType::to_string(val.type()));
    }

    std::string content;
    for (auto beg = args.begin(); beg != args.end(); ++beg) {
        content.append(static_cast<std::string>(*beg));
    }
    return String(content);
}

Value
string_ltrim(args_list args, Environment& env)
{
    if (args.size() < 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    return String(Utility::ltrim(args.front().c_cast<String>()));
}

Value
string_rtrim(args_list args, Environment& env)
{
    if (args.size() < 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    return String(Utility::rtrim(args.front().c_cast<String>()));
}

Value
string_trim(args_list args, Environment& env)
{
    if (args.size() < 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    return String(Utility::trim(args.front().c_cast<String>()));
}

Value
string_split(args_list args, Environment& env)
{
    if (args.size() != 2)
        throw UnexceptNumberOfArgument(1, args.size());

    auto& first = args.front();
    auto& second = args.back();

    if (first.type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(first.type()));

    if (second.type() != DataType::Kind::String)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(second.type()));

    auto splited =
      Utility::split(first.c_cast<String>(), second.c_cast<String>());
    List result;
    for (const auto& str : splited) {
        result.push_back(String(str));
    }
    return result;
}

Value
string_substring(args_list args, Environment& env)
{
    // No Length
    if (args.size() == 2) {
        auto& first = args.front();
        auto& second = args.back();

        if (first.type() != DataType::Kind::String)
            throw UnexceptType(DataType::to_string(DataType::Kind::String),
                               DataType::to_string(first.type()));

        if (second.type() != DataType::Kind::Number)
            throw UnexceptType(DataType::to_string(DataType::Kind::Number),
                               DataType::to_string(second.type()));

        auto& str = first.c_cast<String>();
        auto& i = second.c_cast<Number>();
        if (i.integer() >= str.const_value().size() || i.integer() < 0)
            throw RuntimeError("except a number that smaller than the size of "
                               "the string, and not less than 0");

        return String(str.const_value().substr(i.integer()));
    }
    // Length
    else if (args.size() == 3) {
        auto& first = args.front();
        auto& second = *(args.begin() + 1);
        auto& third = args.back();

        if (first.type() != DataType::Kind::String)
            throw UnexceptType(DataType::to_string(DataType::Kind::String),
                               DataType::to_string(first.type()));

        if (second.type() != DataType::Kind::Number)
            throw UnexceptType(DataType::to_string(DataType::Kind::Number),
                               DataType::to_string(second.type()));

        if (third.type() != DataType::Kind::Number)
            throw UnexceptType(DataType::to_string(DataType::Kind::Number),
                               DataType::to_string(third.type()));

        auto& str = first.c_cast<String>();
        auto& i = second.c_cast<Number>();
        if (i.integer() >= str.const_value().size() || i.integer() < 0)
            throw RuntimeError("except a number that smaller than the size of "
                               "the string, and not less than 0");

        auto& l = second.c_cast<Number>();
        if (l.integer() <= 0 ||
            l.integer() > (str.const_value().size() - i.integer()))
            return String(str.const_value().substr(i.integer()));
        else
            return String(str.const_value().substr(i.integer(), l.integer()));
    } else {
        throw UnexceptNumberOfArgument(3, args.size());
    }
}

}