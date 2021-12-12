#include "libdocscript/runtime/list.h"
#include "libdocscript/exception.h"
#include "libdocscript/proc/builtin.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/runtime/value.h"
#include <algorithm>
#include <vector>

namespace libdocscript::proc {
Value
list_is(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    auto& first = args.front();

    return Boolean(first.type() == DataType::Kind::List);
}

Value
list_eq(args_list args, Environment& env)
{
    if (args.size() < 1)
        throw UnexceptNumberOfArgument(1, args.size(), true);

    for (const auto& val : args) {
        if (val.type() != DataType::Kind::Boolean)
            throw UnexceptType(DataType::to_string(DataType::Kind::List),
                               DataType::to_string(val.type()));
    }

    Boolean result(true);
    auto& first = args.front().c_cast<List>();
    for (auto beg = args.begin() + 1; beg != args.end(); ++beg) {
        if (first != *beg) {
            return Boolean(false);
        }
    }
    return Boolean(true);
}

Value
list_pair_is(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (!list_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::List),
                           DataType::to_string(args.front().type()));

    auto& first = args.front().c_cast<List>();
    return Boolean(first.size() == 2);
}

Value
list_to_string(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (!list_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::List),
                           DataType::to_string(args.front().type()));

    auto& first = args.front().c_cast<List>();
    return String(first);
}

Value
list_to_boolean(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (!list_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::List),
                           DataType::to_string(args.front().type()));

    auto& first = args.front().c_cast<List>();
    return Boolean(first);
}

Value
list_make_list(args_list args, Environment& env)
{
    if (args.size() != 2)
        throw UnexceptNumberOfArgument(2, args.size());

    auto& first = args.front();
    auto& second = args.back();

    if (second.type() != DataType::Kind::Number)
        throw UnexceptType(DataType::to_string(DataType::Kind::Number),
                           DataType::to_string(second.type()));

    List result;
    Number::int_type dest = second.c_cast<Number>().integer();
    for (decltype(dest) i = 0; i != dest; ++i) {
        result.push_back(first);
    }

    return result;
}

Value
list_(args_list args, Environment& env)
{
    List result;
    for (const auto& elem : args) {
        result.push_back(elem);
    }
    return result;
}

Value
list_pair(args_list args, Environment& env)
{
    if (args.size() != 2)
        throw UnexceptNumberOfArgument(2, args.size());

    List result;
    result.push_back(args.front());
    result.push_back(args.back());

    return result;
}

Value
list_first(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (!list_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::List),
                           DataType::to_string(args.front().type()));

    auto& first = args.front().c_cast<List>();
    if (first.size() == 0)
        return Nil();
    else
        return *(first.cbegin());
}

Value
list_last(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (!list_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::List),
                           DataType::to_string(args.front().type()));

    auto& first = args.front().c_cast<List>();
    if (first.size() == 0)
        return Nil();
    else
        return *(first.cend() - 1);
}

Value
list_car(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (!list_is(args, env) && args.front().c_cast<List>().size() != 0)
        throw UnexceptType(DataType::to_string(DataType::Kind::List),
                           DataType::to_string(args.front().type()) +
                             ", or an empty list");

    auto& list = args.front().c_cast<List>();
    return *(list.cbegin());
}

Value
list_cdr(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (!list_is(args, env) && args.front().c_cast<List>().size() != 0)
        throw UnexceptType(DataType::to_string(DataType::Kind::List),
                           DataType::to_string(args.front().type()) +
                             ", or an empty list");

    auto& list = args.front().c_cast<List>();
    if(list.size() == 1) {
        return List();
    } else {
        return List(list.cbegin() + 1, list.cend());
    }
}

Value
list_nth(args_list args, Environment& env)
{
    if (args.size() != 2)
        throw UnexceptNumberOfArgument(1, args.size());

    auto& first = args.front();
    auto& second = args.back();

    if (first.type() != DataType::Kind::List)
        throw UnexceptType(DataType::to_string(DataType::Kind::List),
                           DataType::to_string(first.type()));

    if (second.type() != DataType::Kind::Number)
        throw UnexceptType(DataType::to_string(DataType::Kind::Number),
                           DataType::to_string(second.type()));

    auto i = second.c_cast<Number>().integer();

    if (i < 0 || i >= first.c_cast<List>().size())
        throw RuntimeError("out of list range");

    return first.c_cast<List>().craw()[i];
}

Value
list_length(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (!list_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::List),
                           DataType::to_string(args.front().type()));

    return Number(
      static_cast<Number::int_type>(args.front().c_cast<List>().size()));
}

Value
list_reverse(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (!list_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::List),
                           DataType::to_string(args.front().type()));

    auto& list = args.front().c_cast<List>();
    List result;
    for (auto beg = list.craw().crbegin(); beg != list.craw().crend(); ++beg) {
        result.push_back(*beg);
    }
    return result;
}

Value
list_concat(args_list args, Environment& env)
{
    if (args.size() < 1)
        throw UnexceptNumberOfArgument(1, args.size(), true);

    for (auto beg = args.begin(); beg != args.end(); ++beg) {
        if (beg->type() != DataType::Kind::List)
            throw UnexceptType(DataType::to_string(DataType::Kind::List),
                               DataType::to_string(beg->type()));
    }

    auto result = args.begin()->c_cast<List>();
    for (auto beg = args.begin() + 1; beg != args.end(); ++beg) {
        auto list = beg->c_cast<List>();
        for (const auto& val : list.craw()) {
            result.push_back(val);
        }
    }

    return result;
}

Value
list_append(args_list args, Environment& env)
{
    if (args.size() < 1)
        throw UnexceptNumberOfArgument(1, args.size(), true);

    auto& first = args.front();

    if (first.type() != DataType::Kind::List)
        throw UnexceptType(DataType::to_string(DataType::Kind::List),
                           DataType::to_string(first.type()));

    auto result = first.c_cast<List>();
    for (auto beg = args.begin() + 1; beg != args.end(); ++beg) {
        result.push_back(*beg);
    }
    return result;
}

Value
list_empty_is(args_list args, Environment& env)
{
    if (args.size() != 1)
        throw UnexceptNumberOfArgument(1, args.size());

    if (!list_is(args, env))
        throw UnexceptType(DataType::to_string(DataType::Kind::List),
                           DataType::to_string(args.front().type()));

    return Boolean(args.front().c_cast<List>().size() == 0);
}
}