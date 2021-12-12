#include "libdocscript/exception.h"
#include "libdocscript/proc/builtin.h"
#include "libdocscript/proc/utility.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/runtime/value.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>

namespace libdocscript::proc {

Value number_is(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    auto& first = args.front();
    return Boolean(first.type() == DataType::Kind::Number);
}

Value number_eq(args_list args, Environment& env)
{
    if (args.size() < 1) throw UnexceptNumberOfArgument(1, args.size());

    for (const auto& val : args) {
        if (val.type() != DataType::Kind::Number)
            throw UnexceptType(DataType::to_string(DataType::Kind::String),
                               DataType::to_string(val.type()));
    }

    const auto& first = args.front();
    for (auto beg = args.begin() + 1; beg != args.end(); ++beg) {
        if (first != *beg) return Boolean(false);
    }
    return Boolean(true);
}

Value number_to_string(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::Number)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    return String(args.front());
}

Value number_to_boolean(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::Number)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    return Boolean(args.front());
}

Value number_positive_is(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::Number)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    return Boolean(args.front().c_cast<Number>().decimal() > 0);
}

Value number_negative_is(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::Number)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    return Boolean(args.front().c_cast<Number>().decimal() < 0);
}

Value number_integer_is(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::Number)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    return Boolean(args.front().c_cast<Number>().type() == Number::Integer);
}

Value number_decimal_is(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::Number)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    return Boolean(args.front().c_cast<Number>().type() == Number::Decimal);
}

Value number_zero_is(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::Number)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    return Boolean(args.front().c_cast<Number>().decimal() == 0);
}

Value number_even_is(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::Number)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    auto& num = args.front().c_cast<Number>();
    return Boolean((num.integer() % 2) == 0);
}

Value number_odd_is(args_list args, Environment& env)
{
    if (args.size() != 1) throw UnexceptNumberOfArgument(1, args.size());

    if (args.front().type() != DataType::Kind::Number)
        throw UnexceptType(DataType::to_string(DataType::Kind::String),
                           DataType::to_string(args.front().type()));

    auto& num = args.front().c_cast<Number>();
    return Boolean(num.integer() % 2 == 1);
}

Value number_add(args_list args, Environment& env)
{
    return Utility::number_func_helper(
        args, [](const Number& v, Utility::number_list list) -> Number {
            if (list.size() == 0) {
                if (v.type() == Number::Integer)
                    return Number(std::abs(v.integer()));
                else
                    return Number(std::abs(v.decimal()));
            }

            long double result = v.decimal();
            for (const auto& num : list) {
                result += num.decimal();
            }
            return Number(result);
        });
}

Value number_minus(args_list args, Environment& env)
{
    return Utility::number_func_helper(
        args, [](const Number& v, Utility::number_list list) -> Number {
            if (list.size() == 0 && v.decimal() > 0) {
                if (v.type() == Number::Integer)
                    return Number(v.integer() * -1);
                else
                    return Number(v.decimal() * -1);
            }

            long double result = v.decimal();
            for (const auto& num : list) {
                result -= num.decimal();
            }
            return Number(result);
        });
}

Value number_mul(args_list args, Environment& env)
{
    return Utility::number_func_helper(args,
                                       [](const Number& v1, const Number& v2,
                                          Utility::number_list list) -> Number {
                                           long double result =
                                               v1.decimal() * v2.decimal();
                                           for (const auto& num : list) {
                                               result *= num.decimal();
                                           }
                                           return Number(result);
                                       });
}

Value number_div(args_list args, Environment& env)
{
    return Utility::number_func_helper(args,
                                       [](const Number& v1, const Number& v2,
                                          Utility::number_list list) -> Number {
                                           long double result =
                                               v1.decimal() * v2.decimal();
                                           for (const auto& num : list) {
                                               result /= num.decimal();
                                           }
                                           return Number(result);
                                       });
}

Value number_quotient(args_list args, Environment& env)
{
    return Utility::number_func_helper(
        args, [](const Number& v1, const Number& v2) -> Number {
            return Number(v1.decimal() / v2.decimal());
        });
}

Value number_remainder(args_list args, Environment& env)
{
    return Utility::number_func_helper(
        args, [](const Number& v1, const Number& v2) -> Number {
            return Number(std::remainderl(v1.decimal(), v2.decimal()));
        });
}

Value number_modulo(args_list args, Environment& env)
{
    return Utility::number_func_helper(
        args, [](const Number& v1, const Number& v2) -> Number {
            return Number(std::fmodl(v1.decimal(), v2.decimal()));
        });
}

Value number_expt(args_list args, Environment& env)
{
    return Utility::number_func_helper(
        args, [](const Number& v1, const Number& v2) -> Number {
            return Number(std::powl(v1.decimal(), v2.decimal()));
        });
}

Value number_sqrt(args_list args, Environment& env)
{
    return Utility::number_func_helper(args, [](const Number& v1) -> Number {
        return Number(std::sqrtl(v1.decimal()));
    });
}

Value number_abs(args_list args, Environment& env)
{
    return Utility::number_func_helper(args, [](const Number& v1) -> Number {
        if (v1.type() == Number::Integer) {
            return Number(std::abs(v1.integer()));
        }
        else {
            return Number(std::fabsl(v1.decimal()));
        }
    });
}

Value number_gcd(args_list args, Environment& env)
{
    return Utility::number_func_helper(
        args, [](const Number& v1, const Number& v2) -> Number {
            if (v1.type() != Number::Integer || v2.type() != Number::Integer)
                throw UnexceptType("an integer value", "a decimal value");

            return Number(std::gcd(v1.integer(), v2.integer()));
        });
}

Value number_lcm(args_list args, Environment& env)
{
    return Utility::number_func_helper(
        args, [](const Number& v1, const Number& v2) -> Number {
            if (v1.type() != Number::Integer || v2.type() != Number::Integer)
                throw UnexceptType("an integer value", "a decimal value");

            return Number(std::lcm(v1.integer(), v2.integer()));
        });
}

Value number_integer_add(args_list args, Environment& env)
{
    return Utility::number_func_helper(
        args, [](const Number& v, Utility::number_list list) -> Number {
            if (list.size() == 0) {
                if (v.type() == Number::Integer)
                    return Number(std::abs(v.integer()));
                else
                    return Number(std::abs(v.decimal()));
            }

            Number::int_type result = v.integer();
            for (const auto& num : list) {
                result += num.integer();
            }
            return Number(result);
        });
}

Value number_integer_minus(args_list args, Environment& env)
{
    return Utility::number_func_helper(
        args, [](const Number& v, Utility::number_list list) -> Number {
            if (list.size() == 0 && v.decimal() > 0) {
                if (v.type() == Number::Integer)
                    return Number(v.integer() * -1);
                else
                    return Number(v.decimal() * -1);
            }

            Number::int_type result = v.integer();
            for (const auto& num : list) {
                result -= num.integer();
            }
            return Number(result);
        });
}

Value number_integer_mul(args_list args, Environment& env)
{
    return Utility::number_func_helper(args,
                                       [](const Number& v1, const Number& v2,
                                          Utility::number_list list) -> Number {
                                           Number::int_type result =
                                               v1.integer() * v2.integer();
                                           for (const auto& num : list) {
                                               result *= num.integer();
                                           }
                                           return Number(result);
                                       });
}

Value number_integer_div(args_list args, Environment& env)
{
    return Utility::number_func_helper(args,
                                       [](const Number& v1, const Number& v2,
                                          Utility::number_list list) -> Number {
                                           Number::int_type result =
                                               v1.integer() * v2.integer();
                                           for (const auto& num : list) {
                                               result /= num.integer();
                                           }
                                           return Number(result);
                                       });
}

Value number_great_than(args_list args, Environment& env)
{
    if (args.size() < 2) throw UnexceptNumberOfArgument(2, args.size(), true);
    for (const auto& val : args) {
        if (val.type() != DataType::Kind::Number)
            throw UnexceptType(DataType::to_string(DataType::Kind::String),
                               DataType::to_string(val.type()));
    }

    long double last = args.front().c_cast<Number>().decimal();
    for (auto beg = args.begin() + 1; beg != args.end(); ++beg) {
        if (last <= beg->c_cast<Number>().decimal()) return Boolean(false);
        last = beg->c_cast<Number>().decimal();
    }
    return Boolean(true);
}

Value number_less_than(args_list args, Environment& env)
{
    if (args.size() < 2) throw UnexceptNumberOfArgument(2, args.size(), true);
    for (const auto& val : args) {
        if (val.type() != DataType::Kind::Number)
            throw UnexceptType(DataType::to_string(DataType::Kind::String),
                               DataType::to_string(val.type()));
    }

    long double last = args.front().c_cast<Number>().decimal();
    for (auto beg = args.begin() + 1; beg != args.end(); ++beg) {
        if (last >= beg->c_cast<Number>().decimal()) return Boolean(false);
        last = beg->c_cast<Number>().decimal();
    }
    return Boolean(true);
}

Value number_equal(args_list args, Environment& env)
{
    if (args.size() < 2) throw UnexceptNumberOfArgument(2, args.size(), true);
    for (const auto& val : args) {
        if (val.type() != DataType::Kind::Number)
            throw UnexceptType(DataType::to_string(DataType::Kind::String),
                               DataType::to_string(val.type()));
    }

    long double last = args.front().c_cast<Number>().decimal();
    for (auto beg = args.begin() + 1; beg != args.end(); ++beg) {
        if (last != beg->c_cast<Number>().decimal()) return Boolean(false);
        last = beg->c_cast<Number>().decimal();
    }
    return Boolean(true);
}

Value number_great_equal(args_list args, Environment& env)
{
    if (args.size() < 2) throw UnexceptNumberOfArgument(2, args.size(), true);
    for (const auto& val : args) {
        if (val.type() != DataType::Kind::Number)
            throw UnexceptType(DataType::to_string(DataType::Kind::String),
                               DataType::to_string(val.type()));
    }

    long double last = args.front().c_cast<Number>().decimal();
    for (auto beg = args.begin() + 1; beg != args.end(); ++beg) {
        if (last < beg->c_cast<Number>().decimal()) return Boolean(false);
        last = beg->c_cast<Number>().decimal();
    }
    return Boolean(true);
}

Value number_less_equal(args_list args, Environment& env)
{
    if (args.size() < 2) throw UnexceptNumberOfArgument(2, args.size(), true);
    for (const auto& val : args) {
        if (val.type() != DataType::Kind::Number)
            throw UnexceptType(DataType::to_string(DataType::Kind::String),
                               DataType::to_string(val.type()));
    }

    long double last = args.front().c_cast<Number>().decimal();
    for (auto beg = args.begin() + 1; beg != args.end(); ++beg) {
        if (last > beg->c_cast<Number>().decimal()) return Boolean(false);
        last = beg->c_cast<Number>().decimal();
    }
    return Boolean(true);
}

Value number_sin(args_list args, Environment& env)
{
    return Utility::number_func_helper(args, [](const Number& v1) -> Number {
        return Number(std::sinl(v1.decimal()));
    });
}

Value number_cos(args_list args, Environment& env)
{
    return Utility::number_func_helper(args, [](const Number& v1) -> Number {
        return Number(std::cosl(v1.decimal()));
    });
}

Value number_tan(args_list args, Environment& env)
{
    return Utility::number_func_helper(args, [](const Number& v1) -> Number {
        return Number(std::tanl(v1.decimal()));
    });
}

Value number_asin(args_list args, Environment& env)
{
    return Utility::number_func_helper(args, [](const Number& v1) -> Number {
        return Number(std::asinl(v1.decimal()));
    });
}

Value number_acos(args_list args, Environment& env)
{
    return Utility::number_func_helper(args, [](const Number& v1) -> Number {
        return Number(std::acosl(v1.decimal()));
    });
}

Value number_atan(args_list args, Environment& env)
{
    return Utility::number_func_helper(args, [](const Number& v1) -> Number {
        return Number(std::atanl(v1.decimal()));
    });
}

Value number_exp(args_list args, Environment& env)
{
    return Utility::number_func_helper(args, [](const Number& v1) -> Number {
        return Number(std::expl(v1.decimal()));
    });
}

Value number_log(args_list args, Environment& env)
{
    return Utility::number_func_helper(args, [](const Number& v1) -> Number {
        return Number(std::logl(v1.decimal()));
    });
}

Value number_max(args_list args, Environment& env)
{
    return Utility::number_func_helper(
        args, [](Utility::number_list list) -> Number {
            if (list.size() == 0) {
                return Number(std::numeric_limits<Number::dec_type>().max());
            }
            else if (list.size() == 1) {
                return list.front();
            }
            else {
                return *(
                    std::max_element(list.begin(), list.end(),
                                     [](const Number& v1, const Number& v2) {
                                         return v1.decimal() > v2.decimal();
                                     }));
            }
        });
}
Value number_min(args_list args, Environment& env)
{
    return Utility::number_func_helper(
        args, [](Utility::number_list list) -> Number {
            if (list.size() == 0) {
                return Number(std::numeric_limits<Number::dec_type>().min());
            }
            else if (list.size() == 1) {
                return list.front();
            }
            else {
                return *(
                    std::min_element(list.begin(), list.end(),
                                     [](const Number& v1, const Number& v2) {
                                         return v1.decimal() < v2.decimal();
                                     }));
            }
        });
}

Value number_floor(args_list args, Environment& env)
{
    return Utility::number_func_helper(args, [](const Number& v) -> Number {
        return Number(std::floorl(v.decimal()));
    });
}

Value number_ceiling(args_list args, Environment& env)
{
    return Utility::number_func_helper(args, [](const Number& v) -> Number {
        return Number(std::ceill(v.decimal()));
    });
}

Value number_truncate(args_list args, Environment& env)
{
    return Utility::number_func_helper(args, [](const Number& v) -> Number {
        return Number(std::truncl(v.decimal()));
    });
}

Value number_round(args_list args, Environment& env)
{
    return Utility::number_func_helper(args, [](const Number& v) -> Number {
        return Number(std::roundl(v.decimal()));
    });
}

} // namespace libdocscript::proc