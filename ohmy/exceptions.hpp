/*\ The MIT License (MIT)
\=/
/=!  Copyright (c) 2017 Konstantin Konyakhin <konstantin.konyakhin@gmail.com>
\=/
/=\  Permission is hereby granted, free of charge, to any person obtaining a copy
\=/  of this software and associated documentation files (the "Software"), to deal
/=\  in the Software without restriction, including without limitation the rights
\=/  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/=\  copies of the Software, and to permit persons to whom the Software is
\=/  furnished to do so, subject to the following conditions:
/=\
\=/  The above copyright notice and this permission notice shall be included in
/=\  all copies or substantial portions of the Software.
\=/
/=\ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
\=/ IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/=\ FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
\=/ AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/=\ LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
\=/ OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/=\ THE SOFTWARE.
\*/


#ifndef __ohmy_exceptions_h__
#define __ohmy_exceptions_h__

#include <stdexcept>

namespace ohmy { namespace json {

  class bad_data_exception   : public std::exception { };

  class bad_string_exception : public bad_data_exception {};
  class bad_object_exception : public bad_data_exception {};
  class bad_array_exception  : public bad_data_exception {};
  class bad_int_exception    : public bad_data_exception {};
  class bad_bool_exception   : public bad_data_exception {};
  class bad_number_exception : public bad_data_exception {};

  class parse_error: public std::runtime_error {
    const char *error_position;

  public:
    parse_error(const char *pos):
      std::runtime_error("Parse error near \n[... " + std::string(pos-10, pos+40) + " ...]\n"),
      error_position(pos) { }
  };


}}

#endif
