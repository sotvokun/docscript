#ifndef LIBDOCSCRIPT_PROC_BUILTIN_H
#define LIBDOCSCRIPT_PROC_BUILTIN_H
#include "libdocscript/runtime/environment.h"
#include "libdocscript/runtime/procedure.h"
#include "libdocscript/runtime/value.h"

namespace libdocscript::proc {

using namespace libdocscript::runtime;

using args_list = const libdocscript::runtime::Procedure::args_list&;

// Equivalence

Value equal_type(args_list args, Environment& env);
Value equal(args_list args, Environment& env);

// Booleans

Value boolean_is(args_list args, Environment& env);
Value boolean_eq(args_list args, Environment& env);
Value boolean_to_string(args_list args, Environment& env);
Value boolean_not(args_list args, Environment& env);

// List

Value list_is(args_list args, Environment& env);
Value list_eq(args_list args, Environment& env);
Value list_pair_is(args_list args, Environment& env);
Value list_to_string(args_list args, Environment& env);
Value list_to_boolean(args_list args, Environment& env);

Value list_make_list(args_list args, Environment& env);
Value list_(args_list args, Environment& env);
Value list_pair(args_list args, Environment& env);
Value list_first(args_list args, Environment& env);
Value list_last(args_list args, Environment& env);
Value list_car(args_list args, Environment& env);
Value list_cdr(args_list args, Environment& env);
Value list_nth(args_list args, Environment& env);
Value list_length(args_list args, Environment& env);
Value list_reverse(args_list args, Environment& env);
Value list_concat(args_list args, Environment& env);
Value list_append(args_list args, Environment& env);
Value list_empty_is(args_list args, Environment& env);

// Symbol

Value symbol_is(args_list args, Environment& env);
Value symbol_eq(args_list args, Environment& env);
Value symbol_to_string(args_list args, Environment& env);
Value symbol_to_boolean(args_list args, Environment& env);

// String
Value string_is(args_list args, Environment& env);
Value string_eq(args_list args, Environment& env);
Value string_to_symbol(args_list args, Environment& env);
Value string_to_boolean(args_list args, Environment& env);

Value string_make_string(args_list args, Environment& env);
Value string_gt_is(args_list args, Environment& env);
Value string_lt_is(args_list args, Environment& env);
Value string_length(args_list args, Environment& env);
Value string_append(args_list args, Environment& env);
Value string_concat(args_list args, Environment& env);
Value string_ltrim(args_list args, Environment& env);
Value string_rtrim(args_list args, Environment& env);
Value string_trim(args_list args, Environment& env);
Value string_split(args_list args, Environment& env);
Value string_substring(args_list args, Environment& env);

// Number

Value number_is(args_list args, Environment& env);
Value number_eq(args_list args, Environment& env);
Value number_to_string(args_list args, Environment& env);
Value number_to_boolean(args_list args, Environment& env);

Value number_positive_is(args_list args, Environment& env);
Value number_negative_is(args_list args, Environment& env);
Value number_integer_is(args_list args, Environment& env);
Value number_decimal_is(args_list args, Environment& env);
Value number_zero_is(args_list args, Environment& env);
Value number_even_is(args_list args, Environment& env);
Value number_odd_is(args_list args, Environment& env);

Value number_add(args_list args, Environment& env);
Value number_minus(args_list args, Environment& env);
Value number_mul(args_list args, Environment& env);
Value number_div(args_list args, Environment& env);
Value number_quotient(args_list args, Environment& env);
Value number_remainder(args_list args, Environment& env);
Value number_modulo(args_list args, Environment& env);
Value number_expt(args_list args, Environment& env);
Value number_sqrt(args_list args, Environment& env);
Value number_abs(args_list args, Environment& env);
Value number_gcd(args_list args, Environment& env);
Value number_lcm(args_list args, Environment& env);

Value number_integer_add(args_list args, Environment& env);
Value number_integer_minus(args_list args, Environment& env);
Value number_integer_mul(args_list args, Environment& env);
Value number_integer_div(args_list args, Environment& env);

Value number_great_than(args_list args, Environment& env);
Value number_less_than(args_list args, Environment& env);
Value number_equal(args_list args, Environment& env);
Value number_great_equal(args_list args, Environment& env);
Value number_less_equal(args_list args, Environment& env);

Value number_sin(args_list args, Environment& env);
Value number_cos(args_list args, Environment& env);
Value number_tan(args_list args, Environment& env);
Value number_asin(args_list args, Environment& env);
Value number_acos(args_list args, Environment& env);
Value number_atan(args_list args, Environment& env);

Value number_exp(args_list args, Environment& env);
Value number_log(args_list args, Environment& env);

Value number_max(args_list args, Environment& env);
Value number_min(args_list args, Environment& env);

Value number_floor(args_list args, Environment& env);
Value number_ceiling(args_list args, Environment& env);
Value number_truncate(args_list args, Environment& env);
Value number_round(args_list args, Environment& env);

// Procedure

Value procedure_is(args_list args, Environment& env);
Value procedure_to_string(args_list args, Environment& env);
Value procedure_to_boolean(args_list args, Environment& env);

Value procedure_apply(args_list args, Environment& env);

// Error

Value error_is(args_list args, Environment& env);
Value error_to_string(args_list args, Environment& env);
Value error_to_boolean(args_list args, Environment& env);

Value error_(args_list args, Environment& env);

// Nil

Value nil_is(args_list args, Environment& env);
Value nil_to_string(args_list args, Environment& env);
Value nil_to_boolean(args_list args, Environment& env);

// STDIO

Value stdio_stdout_write(args_list args, Environment& env);
Value stdio_stdin_read_line(args_list args, Environment& env);
Value stdio_stdin_status(args_list args, Environment& env);
Value stdio_stdin_status_to_boolean(args_list args, Environment& env);

// Load and Eval

Value load_(args_list args, Environment& env);
Value load_script(args_list args, Environment& env);
} // namespace libdocscript::proc

#endif