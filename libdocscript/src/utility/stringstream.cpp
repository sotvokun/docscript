#include "libdocscript/utility/stringstream.h"
#include "libdocscript/utility/position.h"
#include <algorithm>
#include <cstdio>
#include <string>

namespace libdocscript {

// +------------------------+
//         Constructor
// +------------------------+

StringStream::StringStream()
  : StringStream("")
{}

StringStream::StringStream(const std::string& content)
  : _content(content)
  , _pos(1, 1)
  , _begin(_content.cbegin())
  , _end(_content.end())
{
    _iter = _content.cbegin();
}

// +------------------------+
//      Public Functions
// +------------------------+

char
StringStream::get()
{
    if (!eof()) {
        increase_position();
        return *(_iter++);
    } else {
        return EOF;
    }
}

char
StringStream::peek() const
{
    if (!eof()) {
        return *_iter;
    } else {
        return EOF;
    }
}

void
StringStream::unget()
{
    if (_iter != _begin) {
        decrease_position();
        --_iter;
    }
}

void
StringStream::ignore()
{
    if (!eof()) {
        increase_position();
        ++_iter;
    }
}

std::string
StringStream::getline()
{
    auto target_iter =
      std::find_if(_iter, _end, [](const auto& ch) { return ch == '\n'; });
    std::string result(_iter, target_iter);
    _pos.column += (target_iter - _iter);
    if (target_iter != _end) {
        _iter = target_iter;
        increase_position();
        ++_iter;
    } else {
        _pos.column++;
        _iter = _end;
    }
    return result;
}

void
StringStream::reset()
{
    _iter = _begin;
    _line_col_cnt = std::stack<Position::value_type>();
    _pos = Position(1, 1);
}

void
StringStream::reset(const std::string &str)
{
    _content = str;
    _begin = _content.cbegin();
    _end = _content.cend();
    reset();
}

bool
StringStream::eof() const
{
    return _iter == _end;
}

bool
StringStream::last() const
{
    return _iter == _end - 1;
}

const Position&
StringStream::position() const
{
    return _pos;
}

bool
StringStream::next()
{
    ignore();
    return this->operator bool();
}

char
StringStream::peek_next() const
{
    if (!last()) {
        return *(_iter + 1);
    } else {
        return EOF;
    }
}

void
StringStream::push_back(char ch)
{
    auto diff = _iter - _begin;
    _content.push_back(ch);
    _begin = _content.cbegin();
    _end = _content.cend();
    _iter = _begin + diff;
}

void
StringStream::push_back(const std::string& str)
{
    auto diff = _iter - _begin;
    _content.append(str);
    _begin = _content.cbegin();
    _end = _content.cend();
    _iter = _begin + diff;
}

// +------------------------+
//      Type Conversions
// +------------------------+

StringStream::operator bool() const
{
    return !eof();
}

// +------------------------+
//      Private Functions
// +------------------------+

void
StringStream::increase_position()
{
    if (peek() == '\n') {
        _line_col_cnt.push(_pos.column);
        ++_pos.line;
        _pos.column = 0;
    }
    ++_pos.column;
}

void
StringStream::decrease_position()
{
    if (_pos.column == 1) {
        --_pos.line;
        _pos.column = _line_col_cnt.top();
        _line_col_cnt.pop();
    } else {
        --_pos.column;
    }
}

} // namespace libdocscript