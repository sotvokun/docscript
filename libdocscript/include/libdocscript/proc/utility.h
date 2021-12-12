#ifndef LIBDOCSCRIPT_PROC_UTILITY_H
#define LIBDOCSCRIPT_PROC_UTILITY_H
#include "libdocscript/proc/builtin.h"
#include <functional>
#include <string>
#include <vector>

namespace libdocscript::proc {
class Utility final
{
  public:
    using number_list = const std::vector<Number>&;
    using func_num_1 = std::function<Number(const Number&)>;
    using func_num_1_multiple =
      std::function<Number(const Number&, number_list)>;
    using func_num_2 = std::function<Number(const Number&, const Number&)>;
    using func_num_2_multiple =
      std::function<Number(const Number&, const Number&, number_list)>;
    using func_num_multiple = std::function<Number(number_list)>;

    static std::vector<Number> convert_to_number_list(
      std::vector<Value>::const_iterator beg,
      std::vector<Value>::const_iterator end);

    static Number number_func_helper(args_list args, func_num_1 func);
    static Number number_func_helper(args_list args, func_num_1_multiple func);
    static Number number_func_helper(args_list args, func_num_2 func);
    static Number number_func_helper(args_list args, func_num_2_multiple func);
    static Number number_func_helper(args_list args, func_num_multiple func);

    static std::string ltrim(const std::string&);
    static std::string rtrim(const std::string&);
    static std::string trim(const std::string&);

    static std::vector<std::string> split(const std::string& str,
                                          const std::string& d);
};
}

#endif