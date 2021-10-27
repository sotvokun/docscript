#include "docsir/stdproc.h"
#include "docsir/datatype.h"
#include "docsir/datatype_list.h"
#include "docsir/exception.h"
#include "docsir/value.h"
#include <sstream>
#include <string>
#include <vector>

namespace docscript::docsir::stdproc {

// Standard Procedure Utility

void handle_arguments_size(const argument_type &args,
                           argument_type::size_type expect, bool at_least) {
    if (at_least) {
        if (args.size() < expect) {
            throw ArgumentException::unexpected(expect, args.size(), true);
        }
    } else {
        if (args.size() != expect) {
            throw ArgumentException::unexpected(expect, args.size());
        }
    }
}
void handle_argument_type(const Value &val, ValueType expect) {
    if (val.type() != expect) {
        throw TypeException::unexpected(expect, val.type());
    }
}

// Construction

Value create_list(const argument_type &args) {
    List::list_type list;
    for (const auto &i : args) {
        list.push_back(i);
    }
    return Value::List(list);
}

Value create_pair(const argument_type &args) {
    handle_arguments_size(args, 2);
    List::list_type list{args[0], args[1]};
    return Value::List(list);
}

// Equivalence

Value is_equal(const argument_type &args) {
    handle_arguments_size(args, 2, true);
    auto type = args[0].type();
    for (argument_type::size_type i = 1; i != args.size(); ++i) {
        if (args[i].type() != type) {
            return Value::Boolean(false);
        }
    }
    return Value::Boolean(true);
}

// Type conversion

Value symbol_to_string(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    handle_argument_type(val, ValueType::Symbol);
    return Value::String(val.cast<Symbol>().value());
}

Value number_to_string(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    handle_argument_type(val, ValueType::Number);

    const auto &numval = val.cast<Number>();
    if (numval.number_type() == Number::Integer) {
        return Value::String(std::to_string(numval.value_int()));
    } else {
        return Value::String(std::to_string(numval.value_float()));
    }
}

Value number_to_boolean(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    handle_argument_type(val, ValueType::Number);
    const auto &numval = val.cast<Number>();
    if (numval.value_int() == 0) {
        return Value::Boolean(false);
    } else {
        return Value::Boolean(true);
    }
}

Value string_to_number(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    handle_argument_type(val, ValueType::String);
    const auto &strval = val.cast<String>();
    try {
        if (strval.value().find('.') != strval.value().npos) {
            return Value::Number(std::stod(strval.value()));
        } else {
            return Value::Number(std::stol(strval.value()));
        }
    } catch (...) {
        return Value::List();
    }
}

Value string_to_boolean(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    handle_argument_type(val, ValueType::String);
    const auto &strval = val.cast<String>();
    if (strval.value() == "#t") {
        return Value::Boolean(true);
    } else if (strval.value() == "#f") {
        return Value::Boolean(false);
    } else {
        return Value::List();
    }
}

Value string_to_symbol(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    handle_argument_type(val, ValueType::String);
    const auto &strval = val.cast<String>();
    if (strval.value().size() != 0) {
        return Value::Symbol(strval.value());
    } else {
        return Value::List();
    }
}

Value boolean_to_number(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    handle_argument_type(val, ValueType::Boolean);
    if (val.cast<Boolean>().value() == true) {
        return Value::Number(1l);
    } else {
        return Value::Number(0l);
    }
}

Value boolean_to_string(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    handle_argument_type(val, ValueType::Boolean);
    if (val.cast<Boolean>().value() == true) {
        return Value::String("#t");
    } else {
        return Value::String("#f");
    }
}

Value list_to_string(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::List);
    const auto &listval = args[0].cast<List>();
    std::ostringstream oss;
    oss << "(";
    for (List::list_type::size_type i = 0; i != listval.value().size(); ++i) {
        const auto &val = listval.value()[i];
        switch (val.type()) {
        case ValueType::Symbol:
            oss << symbol_to_string({val}).cast<String>().value();
            break;

        case ValueType::Number:
            oss << number_to_string({val}).cast<String>().value();
            break;

        case ValueType::String:
            oss << "\"" << val.cast<String>().value() << "\"";
            break;

        case ValueType::Boolean:
            oss << boolean_to_string({val}).cast<String>().value();
            break;

        case ValueType::List:
            oss << list_to_string({val}).cast<String>().value();
            break;

        case ValueType::Procedure:
            oss << "#procedure";
            break;
        }
        if (i != listval.value().size() - 1) {
            oss << " ";
        }
    }
    oss << ")";
    return Value::String(oss.str());
}

Value list_to_boolean(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    handle_argument_type(val, ValueType::List);
    if (val.cast<List>().value().size() == 0) {
        return Value::Boolean(false);
    } else {
        return Value::Boolean(true);
    }
}

// Identity Predicate

Value is_symbol(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    return Value::Boolean(val.type() == ValueType::Symbol);
}

Value is_number(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    return Value::Boolean(val.type() == ValueType::Number);
}

Value is_string(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    return Value::Boolean(val.type() == ValueType::String);
}

Value is_boolean(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    return Value::Boolean(val.type() == ValueType::Boolean);
}

Value is_list(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    return Value::Boolean(val.type() == ValueType::List);
}

Value is_pair(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    return Value::Boolean(val.type() == ValueType::List &&
                          val.cast<List>().value().size() == 2);
}

Value is_procedure(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    return Value::Boolean(val.type() == ValueType::Procedure);
}

Value type_to_string(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    return Value::String(datatype_stringify(val.type()));
}

Value type_to_symbol(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    return Value::Symbol(type_to_string({val}).cast<String>().value());
}

// Logical

Value logical_not(const argument_type &args) {
    handle_arguments_size(args, 1);
    const auto &val = args[0];
    handle_argument_type(val, ValueType::Boolean);
    return Value::Boolean(!val.cast<Boolean>().value());
}
} // namespace docscript::docsir::stdproc