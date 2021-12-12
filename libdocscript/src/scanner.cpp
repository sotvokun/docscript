#include "libdocscript/scanner.h"
#include "libdocscript/token.h"
#include <memory>
#include <sstream>
#include <string>

namespace libdocscript {
// +-------------------------+
//        Constructor
// +-------------------------+

Scanner::Scanner(StringStream& stream)
  : _stream(stream)
{
    register_adapter(Mode::Normal, new ScannerNormalMode(*this));
    register_adapter(Mode::Text, new ScannerTextMode(*this));
    enter_mode(Mode::Normal);
}

// +-------------------------+
//      Public Functions
// +-------------------------+

Token
Scanner::get()
{
    auto& adapter = get_adapter();
    return adapter.scan();
}

void
Scanner::reset()
{
    _stream.reset();
    _mode_stack = stack_type();
    _mode_stack.push(Mode::Normal);
}

bool
Scanner::done() const
{
    return _stream.eof();
}

// +-------------------------+
//       Type Conversion
// +-------------------------+

Scanner::operator bool() const
{
    return !done();
}

// +------------------------------+
//  Private [Scanner Mode Control]
// +------------------------------+

Scanner::Mode
Scanner::current_mode()
{
    return _mode_stack.top();
}

void
Scanner::enter_mode(Mode mode)
{
    _mode_stack.push(mode);
}

void
Scanner::exit_current_mode()
{
    if (_mode_stack.size() != 1)
        _mode_stack.pop();
}

// +-------------------------+
//  Private [Adapter Control]
// +-------------------------+

void
Scanner::register_adapter(Mode mode, ModeAdapter* adapter)
{
    _adaptor_dict.insert({ mode, std::unique_ptr<ModeAdapter>(adapter) });
}

Scanner::ModeAdapter&
Scanner::get_adapter()
{
    return *_adaptor_dict.at(current_mode());
}

// +-------------------------+
//      Static Functions
// +-------------------------+

Scanner::token_list
Scanner::tokenize(StringStream& str)
{
    Scanner scanner(str);
    token_list result;
    while (scanner) {
        result.push_back(scanner.get());
    }
    return result;
}

std::string
Scanner::dump(const token_list& list)
{
    std::ostringstream oss;
    for (const auto& t : list) {
        oss << t.position_ref().line << ":" << t.position_ref().column << "\t"
            << stringify(t.type) << "\t" << t.content << "\n";
    }
    return oss.str();
}

} // namespace libdocscript