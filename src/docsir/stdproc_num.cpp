#include "docscript.h"
#include "docsir/datatype.h"
#include "docsir/datatype_list.h"
#include "docsir/exception.h"
#include "docsir/stdproc.h"
#include "docsir/value.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

namespace docscript::docsir::stdproc {

// Number

Value num_add(const argument_type &args) {
    handle_arguments_size(args, 1, true);
    handle_argument_type(args[0], ValueType::Number);
    if (args.size() == 1) {
        const auto &num = args[0].cast<Number>();
        if (num.number_type() == Number::Integer) {
            return Value::Number(std::abs(num.value_int()));
        } else {
            return Value::Number(std::abs(num.value_float()));
        }
    } else {
        ds_float result = 0;
        for (const auto &i : args) {
            handle_argument_type(i, ValueType::Number);
            result += i.cast<Number>().value_float();
        }
        return Value::Number(result);
    }
}

Value num_min(const argument_type &args) {
    handle_arguments_size(args, 1, true);
    handle_argument_type(args[0], ValueType::Number);
    if (args.size() == 1) {
        const auto &num = args[0].cast<Number>();
        if (num.number_type() == Number::Integer) {
            return (num.value_int() > 0 ? Value::Number(num.value_int() * -1)
                                        : Value::Number(num.value_int()));
        } else {
            return (num.value_float() > 0 ? Value::Number(num.value_float() * -1)
                                          : Value::Number(num.value_float()));
        }
    } else {
        ds_float result = 0;
        for (argument_type::size_type i = 0; i != args.size(); ++i) {
            if (i == 0) {
                result = args[i].cast<Number>().value_float();
            } else {
                result -= args[i].cast<Number>().value_float();
            }
        }
        return Value::Number(result);
    }
}

Value num_mul(const argument_type &args) {
    handle_arguments_size(args, 2, true);
    handle_argument_type(args[0], ValueType::Number);
    handle_argument_type(args[1], ValueType::Number);
    ds_float result = args[0].cast<Number>().value_float() *
                      args[1].cast<Number>().value_float();
    for (argument_type::size_type i = 2; i != args.size(); ++i) {
        handle_argument_type(args[i], ValueType::Number);
        result *= args[i].cast<Number>().value_float();
    }
    return Value::Number(result);
}

Value num_div(const argument_type &args) {
    handle_arguments_size(args, 2, true);
    handle_argument_type(args[0], ValueType::Number);
    handle_argument_type(args[1], ValueType::Number);
    if (args[1].cast<Number>().value_float() == 0) {
        throw DivisionByZeroException();
    }
    ds_float result = args[0].cast<Number>().value_float() /
                      args[1].cast<Number>().value_float();
    for (argument_type::size_type i = 2; i != args.size(); ++i) {
        handle_argument_type(args[i], ValueType::Number);
        if (args[i].cast<Number>().value_float() == 0) {
            throw DivisionByZeroException();
        }
        result *= args[i].cast<Number>().value_float();
    }
    return Value::Number(result);
}

Value num_quotient(const argument_type &args) {
    handle_arguments_size(args, 2);
    handle_argument_type(args[0], ValueType::Number);
    handle_argument_type(args[1], ValueType::Number);
    if (args[1].cast<Number>().value_int() == 0) {
        throw DivisionByZeroException();
    }
    ds_integer result = std::div(args[0].cast<Number>().value_int(),
                                 args[1].cast<Number>().value_int())
                            .quot;
    return Value::Number(result);
}

Value num_remainder(const argument_type &args) {
    handle_arguments_size(args, 2);
    handle_argument_type(args[0], ValueType::Number);
    handle_argument_type(args[1], ValueType::Number);
    if (args[1].cast<Number>().value_int() == 0) {
        throw DivisionByZeroException();
    }
    ds_integer result = std::div(args[0].cast<Number>().value_int(),
                                 args[1].cast<Number>().value_int())
                            .rem;
    return Value::Number(result);
}

Value num_modulo(const argument_type &args) {
    handle_arguments_size(args, 2);
    handle_argument_type(args[0], ValueType::Number);
    handle_argument_type(args[1], ValueType::Number);
    if (args[1].cast<Number>().value_int() == 0) {
        throw DivisionByZeroException();
    }
    ds_integer result =
        args[0].cast<Number>().value_int() % args[1].cast<Number>().value_int();
    return Value::Number(result);
}

Value num_expt(const argument_type &args) {
    handle_arguments_size(args, 2);
    handle_argument_type(args[0], ValueType::Number);
    handle_argument_type(args[1], ValueType::Number);
    ds_float result = std::pow(args[0].cast<Number>().value_float(),
                               args[1].cast<Number>().value_float());
    return Value::Number(result);
}

Value num_sqrt(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    if (const auto &num = args[0].cast<Number>();
        num.number_type() == Number::Integer) {
        ds_integer result = std::sqrt(args[0].cast<Number>().value_int());
        return Value::Number(result);
    } else {
        ds_float result = std::sqrt(args[0].cast<Number>().value_float());
        return Value::Number(result);
    }
}

Value num_abs(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    if (const auto &num = args[0].cast<Number>();
        num.number_type() == Number::Integer) {
        ds_integer result = std::abs(args[0].cast<Number>().value_int());
        return Value::Number(result);
    } else {
        ds_float result = std::abs(args[0].cast<Number>().value_float());
        return Value::Number(result);
    }
}

Value num_gcd(const argument_type &args) {
    handle_arguments_size(args, 2);
    handle_argument_type(args[0], ValueType::Number);
    handle_argument_type(args[1], ValueType::Number);
    ds_integer result = std::gcd(args[0].cast<Number>().value_int(),
                                 args[1].cast<Number>().value_int());
    return Value::Number(result);
}

Value num_lcm(const argument_type &args) {
    handle_arguments_size(args, 2);
    handle_argument_type(args[0], ValueType::Number);
    handle_argument_type(args[1], ValueType::Number);
    ds_integer result = std::lcm(args[0].cast<Number>().value_int(),
                                 args[1].cast<Number>().value_int());
    return Value::Number(result);
}

Value num_floor(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Number(std::floor(args[0].cast<Number>().value_float()));
}

Value num_ceiling(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Number(std::ceil(args[0].cast<Number>().value_float()));
}

Value num_truncate(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Number(std::trunc(args[0].cast<Number>().value_float()));
}

Value num_round(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Number(std::round(args[0].cast<Number>().value_float()));
}

Value num_gt(const argument_type &args) {
    handle_arguments_size(args, 2, true);
    for (argument_type::size_type i = 0; i != args.size() - 2; ++i) {
        handle_argument_type(args[i], ValueType::Number);
        handle_argument_type(args[i + 1], ValueType::Number);
        if (!(args[i].cast<Number>().value_float() >
              args[i + 1].cast<Number>().value_float())) {
            return Value::Boolean(false);
        }
    }
    return Value::Boolean(true);
}

Value num_lt(const argument_type &args) {
    handle_arguments_size(args, 2, true);
    for (argument_type::size_type i = 0; i != args.size() - 2; ++i) {
        handle_argument_type(args[i], ValueType::Number);
        handle_argument_type(args[i + 1], ValueType::Number);
        if (!(args[i].cast<Number>().value_float() <
              args[i + 1].cast<Number>().value_float())) {
            return Value::Boolean(false);
        }
    }
    return Value::Boolean(true);
}

Value num_eq(const argument_type &args) {
    handle_arguments_size(args, 2, true);
    for (argument_type::size_type i = 0; i != args.size() - 1; ++i) {
        handle_argument_type(args[i], ValueType::Number);
        handle_argument_type(args[i + 1], ValueType::Number);
        if (!(args[i].cast<Number>().value_float() ==
              args[i + 1].cast<Number>().value_float())) {
            return Value::Boolean(false);
        }
    }
    return Value::Boolean(true);
}

Value num_ge(const argument_type &args) {
    handle_arguments_size(args, 2, true);
    for (argument_type::size_type i = 0; i != args.size() - 2; ++i) {
        handle_argument_type(args[i], ValueType::Number);
        handle_argument_type(args[i + 1], ValueType::Number);
        if (!(args[i].cast<Number>().value_float() >=
              args[i + 1].cast<Number>().value_float())) {
            return Value::Boolean(false);
        }
    }
    return Value::Boolean(true);
}

Value num_le(const argument_type &args) {
    handle_arguments_size(args, 2, true);
    for (argument_type::size_type i = 0; i != args.size() - 2; ++i) {
        handle_argument_type(args[i], ValueType::Number);
        handle_argument_type(args[i + 1], ValueType::Number);
        if (!(args[i].cast<Number>().value_float() <=
              args[i + 1].cast<Number>().value_float())) {
            return Value::Boolean(false);
        }
    }
    return Value::Boolean(true);
}

Value num_sin(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Number(std::sin(args[0].cast<Number>().value_float()));
}

Value num_cos(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Number(std::cos(args[0].cast<Number>().value_float()));
}

Value num_tan(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Number(std::tan(args[0].cast<Number>().value_float()));
}

Value num_asin(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Number(std::asin(args[0].cast<Number>().value_float()));
}

Value num_acos(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Number(std::acos(args[0].cast<Number>().value_float()));
}

Value num_atan(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Number(std::atan(args[0].cast<Number>().value_float()));
}

Value num_exp(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Number(std::exp(args[0].cast<Number>().value_float()));
}

Value num_log(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Number(std::log(args[0].cast<Number>().value_float()));
}

Value num_maximum(const argument_type &args) {
    if (args.size() == 0) {
        return Value::Number(std::numeric_limits<ds_float>().max());
    } else if (args.size() == 1) {
        handle_argument_type(args[0], ValueType::Number);
        return args[0];
    } else {
        auto result = Value::Number(0l);
        for (argument_type::size_type i = 0; i != args.size(); ++i) {
            handle_argument_type(args[i], ValueType::Number);
            const auto &val = args[i];
            if (i == 0) {
                result = val;
                continue;
            }
            if (result.cast<Number>().value_float() >
                val.cast<Number>().value_float()) {
                result = val;
                continue;
            }
        }
        return result;
    }
}

Value num_minimum(const argument_type &args) {
    if (args.size() == 0) {
        return Value::Number(std::numeric_limits<ds_float>().min());
    } else if (args.size() == 1) {
        handle_argument_type(args[0], ValueType::Number);
        return args[0];
    } else {
        auto result = Value::Number(0l);
        for (argument_type::size_type i = 0; i != args.size(); ++i) {
            handle_argument_type(args[i], ValueType::Number);
            const auto &val = args[i];
            if (i == 0) {
                result = val;
                continue;
            }
            if (result.cast<Number>().value_float() <
                val.cast<Number>().value_float()) {
                result = val;
                continue;
            }
        }
        return result;
    }
}

Value num_is_positive(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Boolean(args[0].cast<Number>().value_float() > 0);
}

Value num_is_negative(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Boolean(args[0].cast<Number>().value_float() < 0);
}

Value num_is_integer(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Boolean(args[0].cast<Number>().number_type() ==
                          Number::Integer);
}

Value num_is_float(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Boolean(args[0].cast<Number>().number_type() ==
                          Number::Float);
}

Value num_is_zero(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Boolean(args[0].cast<Number>().value_float() == 0);
}

Value num_is_odd(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Boolean(args[0].cast<Number>().value_int() % 2 != 0);
}

Value num_is_even(const argument_type &args) {
    handle_arguments_size(args, 1);
    handle_argument_type(args[0], ValueType::Number);
    return Value::Boolean(args[0].cast<Number>().value_int() % 2 == 0);
}

} // namespace docscript::docsir::stdproc