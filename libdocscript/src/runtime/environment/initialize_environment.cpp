#include "libdocscript/proc/builtin.h"
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/procedure.h"
#include <string>
#include <unordered_map>

namespace libdocscript::runtime {
Environment&
initialize_environment(Environment& env)
{
    std::unordered_map<std::string, BuiltInProcedure::inner_type> procs{
        { "equal-type?", proc::equal_type },
        { "equal?", proc::equal },

        { "boolean?", proc::boolean_is },
        { "boolean=?", proc::boolean_eq },
        { "boolean->string", proc::boolean_to_string },
        { "not", proc::boolean_not },

        { "list?", proc::list_is },
        { "list=?", proc::list_eq },
        { "pair?", proc::list_pair_is },
        { "list->string", proc::list_to_string },
        { "list->boolean", proc::list_to_boolean },
        //
        { "make-list", proc::list_make_list },
        { "list", proc::list_ },
        { "pair", proc::list_pair },
        { "first", proc::list_first },
        { "last", proc::list_last },
        { "car", proc::list_car },
        { "cdr", proc::list_cdr },
        { "nth", proc::list_nth },
        { "length", proc::list_length },
        { "reverse", proc::list_reverse },
        { "concat", proc::list_concat },
        { "append", proc::list_append },
        { "empty?", proc::list_empty_is },

        { "symbol?", proc::symbol_is },
        { "symbol=?", proc::symbol_eq },
        { "symbol->string", proc::symbol_to_string },
        { "symbol->boolean", proc::symbol_to_boolean },

        { "string?", proc::string_is },
        { "string=?", proc::string_eq },
        { "string->symbol", proc::string_to_symbol },
        { "string->boolean", proc::string_to_boolean },
        //
        { "make-string", proc::string_make_string },
        { "string-gt?", proc::string_gt_is },
        { "string-lt?", proc::string_lt_is },
        { "string-length", proc::string_length },
        { "string-append", proc::string_concat },
        { "string-concat", proc::string_concat },
        { "string-ltrim", proc::string_ltrim },
        { "string-rtrim", proc::string_rtrim },
        { "string-trim", proc::string_trim },
        { "string-split", proc::string_split },
        { "substring", proc::string_substring },

        { "number?", proc::number_is },
        { "number=?", proc::number_eq },
        { "number->string", proc::number_to_string },
        { "number->boolean", proc::number_to_boolean },
        //
        { "positive?", proc::number_positive_is },
        { "negative?", proc::number_negative_is },
        { "integer?", proc::number_integer_is },
        { "decimal", proc::number_decimal_is },
        { "zero?", proc::number_zero_is },
        { "even?", proc::number_even_is },
        { "odd?", proc::number_odd_is },
        //
        { "+", proc::number_add },
        { "-", proc::number_minus },
        { "*", proc::number_mul },
        { "/", proc::number_div },
        { "quotient", proc::number_quotient },
        { "remainder", proc::number_remainder },
        { "modulo", proc::number_modulo },
        { "expt", proc::number_expt },
        { "sqrt", proc::number_sqrt },
        { "abs", proc::number_abs },
        { "gcd", proc::number_gcd },
        { "lcm", proc::number_lcm },
        //
        { "integer+", proc::number_integer_add },
        { "integer-", proc::number_integer_minus },
        { "integer*", proc::number_integer_mul },
        { "integer/", proc::number_integer_div },
        //
        { ">", proc::number_great_than },
        { "<", proc::number_less_than },
        { "=", proc::number_equal },
        { ">=", proc::number_great_equal },
        { "<=", proc::number_less_equal },
        //
        { "sin", proc::number_sin },
        { "cos", proc::number_cos },
        { "tan", proc::number_tan },
        { "asin", proc::number_asin },
        { "acos", proc::number_acos },
        { "atan", proc::number_atan },
        //
        { "exp", proc::number_exp },
        { "log", proc::number_log },
        //
        { "max", proc::number_max },
        { "min", proc::number_min },
        //
        { "floor", proc::number_floor },
        { "ceiling", proc::number_ceiling },
        { "truncate", proc::number_truncate },
        { "round", proc::number_round },

        { "procedure?", proc::procedure_is },
        { "procedure->string", proc::procedure_to_string },
        { "procedure->boolean", proc::procedure_to_boolean },

        { "error?", proc::error_is },
        { "error->string", proc::error_to_string },
        { "error->boolean", proc::error_to_boolean },
        //
        { "error", proc::error_ },

        { "nil?", proc::nil_is },
        { "nil->string", proc::nil_to_string },
        { "nil->boolean", proc::nil_to_boolean }
    };

    for (const auto& pair : procs) {
        env.set(pair.first, Value(BuiltInProcedure(pair.second)));
    }

    return env;
}
}