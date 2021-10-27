#include "docsir/datatype.h"
#include "docsir/datatype_list.h"
#include "docsir/environment.h"
#include "docsir/exception.h"
#include "docsir/interpreter.h"
#include "docsir/stdproc.h"
#include "docsir/stdproc_ext.h"
#include "docsir/value.h"
#include "libdocscript/exception.h"
#include "libdocscript/parser.h"
#include <exception>
#include <iostream>
#include <string>

using namespace docscript::docsir;

Environment Env;
docscript::libdocscript::Parser Pars;

void initialize(Environment &env);
std::ostream &print(std::ostream &os, const Value &val);

int main(int argc, const char *argv[]) {
    bool looptoken = true;
    initialize(Env);
    std::string input;
    do {
        std::cout << "> ";
        std::getline(std::cin, input);
        try {
            auto ast = Pars.parse(input);
            for (const auto &i : ast) {
                auto result = Interpreter(Env).eval(i);
                if (result.has_value()) {
                    print(std::cout, result.value()) << std::endl;
                }
            }
        } catch (const BasicException &e) {
            std::cout << e.msg() << std::endl;
        } catch (const docscript::libdocscript::BasicException &e) {
            std::cout << e.msg() << std::endl;
        } catch (const std::exception &e) {
            std::cout << "Unknown Error:" << e.what() << std::endl;
            looptoken = false;
        }
    } while (looptoken);
    return 0;
}

void initialize(Environment &env) {
    // Construction
    env.set_definition("list", stdproc::create_list);
    env.set_definition("pair", stdproc::create_pair);

    // Equivalence
    env.set_definition("equal?", stdproc::is_equal);

    // Type conversion
    env.set_definition("symbol->string", stdproc::symbol_to_string);
    env.set_definition("number->string", stdproc::number_to_string);
    env.set_definition("number->boolean", stdproc::number_to_boolean);
    env.set_definition("string->number", stdproc::string_to_number);
    env.set_definition("string->boolean", stdproc::string_to_boolean);
    env.set_definition("string->symbol", stdproc::string_to_symbol);
    env.set_definition("boolean->number", stdproc::boolean_to_number);
    env.set_definition("boolean->string", stdproc::boolean_to_string);
    env.set_definition("list->string", stdproc::list_to_string);
    env.set_definition("list->boolean", stdproc::list_to_boolean);

    // Identity Predicate
    env.set_definition("symbol?", stdproc::is_symbol);
    env.set_definition("number?", stdproc::is_number);
    env.set_definition("string?", stdproc::is_string);
    env.set_definition("boolean?", stdproc::is_boolean);
    env.set_definition("list?", stdproc::is_list);
    env.set_definition("pair?", stdproc::is_pair);
    env.set_definition("procedure?", stdproc::is_procedure);
    env.set_definition("type->string", stdproc::type_to_string);
    env.set_definition("type->symbol", stdproc::type_to_symbol);

    // Logical
    env.set_definition("not", stdproc::logical_not);

    // String
    env.set_definition("string-length", stdproc::str_length);
    env.set_definition("string-equal?", stdproc::str_eq);
    env.set_definition("string-gt?", stdproc::str_gt);
    env.set_definition("string-lt?", stdproc::str_lt);
    env.set_definition("string-concat", stdproc::str_concat);
    env.set_definition("string-append", stdproc::str_append);
    env.set_definition("string-prefix", stdproc::str_prefix);
    env.set_definition("string-suffix", stdproc::str_suffix);
    env.set_definition("string-ltrim", stdproc::str_ltrim);
    env.set_definition("string-rtrim", stdproc::str_rtrim);
    env.set_definition("string-trim", stdproc::str_trim);
    env.set_definition("string-split", stdproc::str_split);
    env.set_definition("substring", stdproc::str_substring);

    // List
    env.set_definition("length", stdproc::list_length);
    env.set_definition("first", stdproc::list_first);
    env.set_definition("last", stdproc::list_last);
    env.set_definition("nth", stdproc::list_nth);
    env.set_definition("reverse", stdproc::list_reverse);
    env.set_definition("car", stdproc::list_car);
    env.set_definition("cdr", stdproc::list_cdr);
    env.set_definition("concat", stdproc::list_concat);
    env.set_definition("append", stdproc::list_append);
    env.set_definition("empty?", stdproc::list_is_empty);

    // Number
    // Number::Basic
    env.set_definition("+", stdproc::num_add);
    env.set_definition("-", stdproc::num_min);
    env.set_definition("*", stdproc::num_mul);
    env.set_definition("/", stdproc::num_div);
    env.set_definition("quotient", stdproc::num_quotient);
    env.set_definition("remainder", stdproc::num_remainder);
    env.set_definition("modulo", stdproc::num_modulo);
    env.set_definition("expt", stdproc::num_expt);
    env.set_definition("sqrt", stdproc::num_sqrt);
    env.set_definition("abs", stdproc::num_abs);
    env.set_definition("gcd", stdproc::num_gcd);
    env.set_definition("lcm", stdproc::num_lcm);

    // Number::approximation
    env.set_definition("floor", stdproc::num_floor);
    env.set_definition("ceiling", stdproc::num_ceiling);
    env.set_definition("truncate", stdproc::num_truncate);
    env.set_definition("round", stdproc::num_round);

    // Number::inequalities
    env.set_definition(">", stdproc::num_gt);
    env.set_definition("<", stdproc::num_lt);
    env.set_definition("=", stdproc::num_eq);
    env.set_definition(">=", stdproc::num_ge);
    env.set_definition("<=", stdproc::num_le);

    // Number::Trigonometry
    env.set_definition("sin", stdproc::num_sin);
    env.set_definition("cos", stdproc::num_cos);
    env.set_definition("tan", stdproc::num_tan);
    env.set_definition("asin", stdproc::num_asin);
    env.set_definition("acos", stdproc::num_acos);
    env.set_definition("atan", stdproc::num_atan);

    // Number::Exponentials
    env.set_definition("exp", stdproc::num_exp);
    env.set_definition("log", stdproc::num_log);

    // Number::Maximum-and-Minimum
    env.set_definition("max", stdproc::num_maximum);
    env.set_definition("min", stdproc::num_minimum);

    // Number::Misc
    env.set_definition("positive?", stdproc::num_is_positive);
    env.set_definition("negative?", stdproc::num_is_negative);
    env.set_definition("integer?", stdproc::num_is_integer);
    env.set_definition("float?", stdproc::num_is_float);
    env.set_definition("zero?", stdproc::num_is_zero);
    env.set_definition("odd?", stdproc::num_is_odd);
    env.set_definition("even?", stdproc::num_is_even);

    // IO
    env.set_definition("write-file", stdproc::write_file);
    env.set_definition("read-file", stdproc::read_file);
    env.set_definition("message", stdproc::message);

    // Environment
    env.set_definition("load!", stdproc::privilege_load(Env));

    // Interpretation
    env.set_definition("error", stdproc::error);
    env.set_definition("exit", stdproc::exit);
}

std::ostream &print(std::ostream &os, const Value &val) {
    switch (val.type()) {
    case ValueType::Symbol:
        os << "'" << val.cast<Symbol>().value();
        break;
    case ValueType::String:
        os << "\"" << val.cast<String>().value() << "\"";
        break;
    case ValueType::Boolean:
        os << (val.cast<Boolean>().value() ? "#t" : "#f");
        break;
    case ValueType::Number:
        os << (val.cast<Number>().number_type() == Number::Integer
                   ? val.cast<Number>().value_int()
                   : val.cast<Number>().value_float());
        break;
    case ValueType::List:
        os << stdproc::list_to_string({val}).cast<String>().value();
        break;
    case ValueType::Procedure:
        os << "#procedure";
        break;
    }
    return os;
}