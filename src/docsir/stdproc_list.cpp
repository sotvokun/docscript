#include "docscript.h"
#include "docsir/datatype.h"
#include "docsir/datatype_list.h"
#include "docsir/exception.h"
#include "docsir/stdproc.h"
#include "docsir/value.h"
#include <algorithm>
#include <vector>

namespace docscript::docsir::stdproc {

// List

Value list_length(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::List);
    const auto &listval = args[0].cast<List>();
    return Value::Number(static_cast<ds_integer>(listval.value().size()));
}

Value list_first(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::List);
    const auto &listval = args[0].cast<List>();
    if (listval.value().size() == 0) {
        throw ArgumentException("expect a list, given an empty list");
    } else {
        return listval.value().front();
    }
}

Value list_last(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::List);
    const auto &listval = args[0].cast<List>();
    if (listval.value().size() == 0) {
        throw ArgumentException("expect a list, given an empty list");
    } else {
        return listval.value().back();
    }
}

Value list_nth(const argument_type &args) {
    handle_arguments_size(args, 2);
    handle_argument_type(args[0], ValueType::List);
    handle_argument_type(args[1], ValueType::Number);
    const auto &listval = args[0].cast<List>();
    const auto &numval = args[1].cast<Number>();
    if (numval.value_int() >= listval.value().size() ||
        numval.value_int() < 0) {
        throw ArgumentException("expect a number that smaller than the size of "
                                "list, and not less than 0");
    } else {
        return listval.value()[numval.value_int()];
    }
}

Value list_reverse(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::List);
    const auto &listval = args[0].cast<List>();
    List::list_type result;
    std::reverse_copy(listval.value().begin(), listval.value().end(),
                      result.begin());
    return Value::List(result);
}

Value list_car(const argument_type &args) { 
    // The same of list_first
    return list_first(args); 
}

Value list_cdr(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::List);
    const auto &listval = args[0].cast<List>();
    if (listval.value().size() == 0) {
        throw ArgumentException("expect a list, given an empty list");
    } else if (listval.value().size() == 1) {
        return Value::List();
    } else {
        List::list_type result(listval.value().begin() + 1,
                               listval.value().end());
        return Value::List(result);
    }
}

Value list_concat(const argument_type &args) {
    List::list_type result;
    for (const auto &i : args) {
        handle_argument_type(i, ValueType::List);
        const auto &listval = i.cast<List>();
        result.insert(result.end(), listval.value().begin(),
                      listval.value().end());
    }
    return Value::List(result);
}

Value list_append(const argument_type &args) {
    handle_arguments_size(args, 1, true);
    handle_argument_type(args[0], ValueType::List);
    auto result = args[0].cast<List>().value();
    for (List::list_type::size_type i = 1; i != args.size(); ++i) {
        result.push_back(args[i]);
    }
    return Value::List(result);
}

Value list_is_empty(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::List);
    const auto &listval = args[0].cast<List>();
    return Value::Boolean((listval.value().size() == 0 ? true : false));
}

} // namespace docscript::docsir::stdproc