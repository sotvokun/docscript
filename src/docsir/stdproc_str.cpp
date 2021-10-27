#include "docscript.h"
#include "docsir/datatype.h"
#include "docsir/datatype_list.h"
#include "docsir/exception.h"
#include "docsir/stdproc.h"
#include "docsir/value.h"
#include <algorithm>
#include <string>
#include <vector>

namespace docscript::docsir::stdproc {

// String

Value str_length(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::String);
    return Value::Number(
        static_cast<ds_integer>(args[0].cast<String>().value().size()));
}

Value str_eq(const argument_type &args) {
    handle_arguments_size(args, 2);
    handle_argument_type(args[0], ValueType::String);
    handle_argument_type(args[1], ValueType::String);
    const auto &str1 = args[0].cast<String>().value();
    const auto &str2 = args[1].cast<String>().value();
    return Value::Boolean(str1 == str2);
}

Value str_gt(const argument_type &args) {
    handle_arguments_size(args, 2);
    handle_argument_type(args[0], ValueType::String);
    handle_argument_type(args[1], ValueType::String);
    const auto &str1 = args[0].cast<String>().value();
    const auto &str2 = args[1].cast<String>().value();
    return Value::Boolean(str1 > str2);
}

Value str_lt(const argument_type &args) {
    handle_arguments_size(args, 2);
    handle_argument_type(args[0], ValueType::String);
    handle_argument_type(args[1], ValueType::String);
    const auto &str1 = args[0].cast<String>().value();
    const auto &str2 = args[1].cast<String>().value();
    return Value::Boolean(str1 < str2);
}

Value str_concat(const argument_type &args) {
    std::string result;
    for (const auto &i : args) {
        handle_argument_type(i, ValueType::String);
        result.append(i.cast<String>().value());
    }
    return Value::String(result);
}

Value str_append(const argument_type &args) {
    handle_arguments_size(args, 1, true);
    handle_argument_type(args[0], ValueType::String);
    std::string result = args[0].cast<String>().value();
    for (argument_type::size_type i = 1; i != args.size(); ++i) {
        const auto &val = args[i];
        handle_argument_type(val, ValueType::String);
        result.append(val.cast<String>().value());
    }
    return Value::String(result);
}

Value str_prefix(const argument_type &args) {
    handle_arguments_size(args, 2);
    handle_argument_type(args[0], ValueType::String);
    handle_argument_type(args[1], ValueType::String);
    auto result = args[0].cast<String>().value();
    const auto &prefix = args[1].cast<String>().value();
    result.insert(result.begin(), prefix.begin(), prefix.end());
    return Value::String(result);
}

Value str_suffix(const argument_type &args) {
    handle_arguments_size(args, 2);
    handle_argument_type(args[0], ValueType::String);
    handle_argument_type(args[1], ValueType::String);
    auto result = args[0].cast<String>().value();
    const auto &suffix = args[1].cast<String>().value();
    result.append(suffix);
    return Value::String(result);
}

Value str_ltrim(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::String);
    auto s = args[0].cast<String>().value();
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
    return Value::String(s);
}

Value str_rtrim(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::String);
    auto s = args[0].cast<String>().value();
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
    return Value::String(s);
}

Value str_trim(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::String);
    return str_ltrim({str_rtrim(args)});
}

Value str_split(const argument_type &args) {
    handle_arguments_size(args, 2);
    handle_argument_type(args[0], ValueType::String);
    handle_argument_type(args[1], ValueType::String);
    List::list_type result;
    auto s = args[0].cast<String>().value();
    const auto &sp = args[1].cast<String>().value();
    decltype(s.size()) i = s.npos;
    while((i = s.find(sp)) != s.npos) {
        auto part = std::string(s.begin(), s.begin() + i);
        result.push_back(Value::String(part));
        s = s.substr(i + sp.size());
    }
    result.push_back(Value::String(s));
    return Value::List(result);
}

Value str_substring(const argument_type &args) {
    handle_arguments_size(args, 3, true);
    handle_argument_type(args[0], ValueType::String);
    handle_argument_type(args[1], ValueType::Number);
    handle_argument_type(args[2], ValueType::Number);
    auto result = args[0].cast<String>().value();
    const auto &i = args[1].cast<Number>().value_int();
    const auto &l = args[2].cast<Number>().value_int();
    if (i >= result.size() || i < 0) {
        throw ArgumentException("expect a number that smaller than the size of "
                                "string, and not less than 0");
    }
    try {
        if (l > 0) {
            return Value::String(result.substr(i, l));
        } else {
            return Value::String(result.substr(i));
        }
    } catch (...) {
        return Value::List();
    }
}

} // namespace docscript::docsir::stdproc