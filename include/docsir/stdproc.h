#ifndef DOCSIR_STDPROC_H
#define DOCSIR_STDPROC_H
#include "docsir/datatype.h"
#include "docsir/datatype_list.h"
#include "docsir/exception.h"
#include "docsir/value.h"
#include <vector>

namespace docscript::docsir::stdproc {

using argument_type = std::vector<docsir::Value>;

// Standard Procedure Utility

void handle_arguments_size(const argument_type &,
                           argument_type::size_type expect,
                           bool at_least = false);
void handle_argument_type(const Value &, ValueType expect);

// Construction

Value create_list(const argument_type &args);
Value create_pair(const argument_type &args);

// Equivalence

Value is_equal(const argument_type &args);

// Type conversion

Value symbol_to_string(const argument_type &args);
Value number_to_string(const argument_type &args);
Value number_to_boolean(const argument_type &args);
Value string_to_number(const argument_type &args);
Value string_to_boolean(const argument_type &args);
Value string_to_symbol(const argument_type &args);
Value boolean_to_number(const argument_type &args);
Value boolean_to_string(const argument_type &args);
Value list_to_string(const argument_type &args);
Value list_to_boolean(const argument_type &args);

// Identity Predicate

Value is_symbol(const argument_type &args);
Value is_number(const argument_type &args);
Value is_string(const argument_type &args);
Value is_boolean(const argument_type &args);
Value is_list(const argument_type &args);
Value is_pair(const argument_type &args);
Value is_procedure(const argument_type &args);
Value type_to_string(const argument_type &args);
Value type_to_symbol(const argument_type &args);

// Logical

Value logical_not(const argument_type &args);

// String

Value str_length(const argument_type &args);
Value str_eq(const argument_type &args);
Value str_gt(const argument_type &args);
Value str_lt(const argument_type &args);
Value str_concat(const argument_type &args);
Value str_append(const argument_type &args);
Value str_prefix(const argument_type &args);
Value str_suffix(const argument_type &args);
Value str_ltrim(const argument_type &args);
Value str_rtrim(const argument_type &args);
Value str_trim(const argument_type &args);
Value str_split(const argument_type &args);
Value str_substring(const argument_type &args);

// List

Value list_length(const argument_type &args);
Value list_first(const argument_type &args);
Value list_last(const argument_type &args);
Value list_nth(const argument_type &args);
Value list_reverse(const argument_type &args);
Value list_car(const argument_type &args);
Value list_cdr(const argument_type &args);
Value list_concat(const argument_type &args);
Value list_append(const argument_type &args);
Value list_is_empty(const argument_type &args);

// Number

Value num_add(const argument_type &args);
Value num_min(const argument_type &args);
Value num_mul(const argument_type &args);
Value num_div(const argument_type &args);
Value num_quotient(const argument_type &args);
Value num_remainder(const argument_type &args);
Value num_modulo(const argument_type &args);
Value num_expt(const argument_type &args);
Value num_sqrt(const argument_type &args);
Value num_abs(const argument_type &args);
Value num_gcd(const argument_type &args);
Value num_lcm(const argument_type &args);

Value num_floor(const argument_type &args);
Value num_ceiling(const argument_type &args);
Value num_truncate(const argument_type &args);
Value num_round(const argument_type &args);

Value num_gt(const argument_type &args);
Value num_lt(const argument_type &args);
Value num_eq(const argument_type &args);
Value num_ge(const argument_type &args);
Value num_le(const argument_type &args);

Value num_sin(const argument_type &args);
Value num_cos(const argument_type &args);
Value num_tan(const argument_type &args);
Value num_asin(const argument_type &args);
Value num_acos(const argument_type &args);
Value num_atan(const argument_type &args);

Value num_exp(const argument_type &args);
Value num_log(const argument_type &args);

Value num_maximum(const argument_type &args);
Value num_minimum(const argument_type &args);

Value num_is_positive(const argument_type &args);
Value num_is_negative(const argument_type &args);
Value num_is_integer(const argument_type &args);
Value num_is_float(const argument_type &args);
Value num_is_zero(const argument_type &args);
Value num_is_odd(const argument_type &args);
Value num_is_even(const argument_type &args);

} // namespace docscript::docsir::stdproc

#endif