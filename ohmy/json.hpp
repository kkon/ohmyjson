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


#ifndef __ohmyjson_h__
#define __ohmyjson_h__

#include <inttypes.h>
#include <string.h>
#include <memory>
#include <iostream>
#include <vector>
#include <string>

#include "exceptions.hpp"
#include "num.hpp"

namespace ohmy { namespace json {

  enum vtype {
    t_empty          = 0x0, // 0b0000
    t_null           = 0x1, // 0b0001
    t_true           = 0x2, // 0b0010
    t_false          = 0x3, // 0b0011

    t_int            = 0x6, // 0b0110
    t_double         = 0x7, // 0b0111

    t_string         = 0x8, // 0b1000
    t_short_string   = 0x9, // 0b1001
    t_int_string     = 0xA, // 0b1010
    t_double_string  = 0xB, // 0b1011

    t_array          = 0xC, // 0b1100
    t_object         = 0xE, // 0b1110
    t_doc            = 0xF, // 0b1111
  };



  template<typename type>
  class iterator {
    type *first;
    int pos;

  public:
    inline iterator(type *cont, int position = 0): first(cont), pos(position){}
    inline iterator(const iterator &other): first(other.first), pos(other.pos){}

    inline bool operator != (const iterator &it) const { return pos != it.pos; }

    inline iterator &operator++ (int) {pos++; return *this;};
    inline iterator &operator++ ()    {pos++; return *this;};
    inline type     &operator*  () const { return first[pos]; }

  };


  template<typename type>
  class pair_ref : public type {

    struct pair{ type key; type value; };

  public:
    inline iterator<pair> end()          { return { (pair *)(this->content.data.val), (int)this->content.size }; }
    inline iterator<pair> begin()        { return { (pair *)(this->content.data.val), 0      }; }
    inline iterator<pair> end()    const { return { (pair *)(this->content.data.val), (int)this->content.size }; }
    inline iterator<pair> begin()  const { return { (pair *)(this->content.data.val), 0      }; }
  };




  class value {

    typedef pair_ref<value> object_ref;

    struct short_string {
      unsigned char type:  4;
      unsigned char size:  4;
      unsigned char data[15] = {0u};
    };

    struct value_content {
      unsigned char type  :4;
      unsigned size;

      union value_data {
        const char *chars;
        void       *val;
        int64_t    _int;
        double     _dbl;
      } data;
    };


    inline value*          first() { return (value *)content.data.val; }
    inline const value*    first() const { return (const value *)content.data.val; }

  public:

    union { short_string str; value_content content; };

    inline value() : value(t_empty,0,0) {}

    inline value(vtype type, unsigned size, const void *ptr) noexcept {
      content = {(unsigned char)type, size, value_content::value_data{ (const char *)ptr } };
    }


    inline void set(unsigned char type, unsigned size, void *data) noexcept {
      content = {type, size, value_content::value_data{ (const char *)data } };
    }

    inline void set_str(unsigned char size, const void *data) noexcept {
      str.type = t_short_string;
      str.size = size;
      memcpy(&str.data[0], data, size);
    }


    inline size_t size()            const { return content.size; }
    inline vtype  type()            const { return (vtype)content.type; }
    inline bool is(unsigned char t) const { return content.type == t; }

    inline bool is_bool()      const { return is(t_true)   || is(t_false); }
    inline bool is_double()    const { return is(t_double) || is(t_double_string); }
    inline bool is_int()       const { return is(t_int)    || is(t_int_string); }
    inline bool is_null()      const { return is(t_null); }
    inline bool is_string()    const { return is(t_string); }
    inline bool is_object()    const { return is(t_object); }
    inline bool is_array()     const { return is(t_array);  }


    inline iterator<value> end()   const { return { (value *)first(), (int)size() }; }
    inline iterator<value> begin() const { return { (value *)first(), 0      }; }
    inline iterator<value> end()         { return { first(), (int)size() }; }
    inline iterator<value> begin()       { return { first(), 0      }; }


    inline const object_ref &as_object() const {
      if(is_object()) return *(object_ref *)this;
      else throw bad_object_exception();
    }


    inline const int as_int() const {
      switch(content.type){
        case t_int:        return (int)content.data._int;
        case t_int_string: return num::atoi(content.data.chars);
        default: throw bad_int_exception();
      }
    }

    inline const bool as_bool() const {
      switch(content.type){
        case t_true:       return true;
        case t_false:      return false;
        default: throw bad_bool_exception();
      }
    }

    inline const double as_double() const {
      switch(content.type){
        case t_int:        return (int)content.data._int;
        case t_int_string: return num::atoi(content.data.chars);

        case t_double:         return content.data._dbl;
        case t_double_string:  return num::atof(content.data.chars);

        default: throw bad_number_exception();
      }
    }

    inline const float       as_float()  const { return float(as_double()); }
    inline const std::string as_string() const { return std::string(as_chars(), strlen()); }

    inline const char *as_chars() const {
      switch(content.type){
        case t_short_string: return (const char *)&str.data[0];

        case t_string:
        case t_int_string:
        case t_double_string:
          return content.data.chars;

        default: throw bad_string_exception();
      }
    }


    inline size_t strlen() const {
      switch(content.type){
        case t_short_string:  return str.size;

        case t_string:
        case t_int_string:
        case t_double_string: return content.size;

        default: throw bad_string_exception();
      }
    }


    inline bool contains(const char *sg) const {
      return is_object() && find_object_key(sg) != nullptr;
    }



    inline const value *find_object_key(const char *index) const {
      auto sz = size();
      if( sz > 0 ){
        auto len = ::strlen(index);
        value *nn = (value *)first();

        for(auto i=0u; i<sz; i++)
          if( memcmp((nn++)->as_chars(), index, len) == 0 ) return nn;
          else nn++;
      }

      return nullptr;
    }


    inline value &operator[](int index) const {
      if(!is_array()) throw bad_array_exception();

      if((unsigned)index < content.size) return ((value *)content.data.val)[index];
      else throw std::out_of_range("array access on index " + std::to_string(index));
    }


    inline value &operator[](const char *k) const {
      if(!is_object()) throw bad_object_exception();

      auto node = (value *)find_object_key(k);

      if( node != nullptr) return *node;
      else throw std::out_of_range(std::string("object key not found ") + k);
    }
  };




  template<class T>
  struct datalist {
    T *_data, *end, *pos;

    datalist(size_t bs):
      _data((T*)malloc(bs * sizeof(T))),
      end(_data+bs),
      pos(_data){}

    ~datalist(){free(_data);}

    inline size_t left() const{ return end - pos; }
  };




  /** Json storage class */

  template <typename value>
  struct store {

    typedef datalist<value>* allocation;
    std::vector<allocation> allocations;

  public:

    ~store(){ for(auto &el: allocations) delete el; }

    inline datalist<value> *track(size_t size){
      allocations.emplace_back(allocation{new datalist<value>(size)});
      return allocations.back();
    }

  };



  class doc : public value {

    std::shared_ptr<json::store<value>> storage;

  public:

    doc(): value(t_empty,0, nullptr), storage(std::make_shared<json::store<value>>()) {}

    inline doc(doc const &other) noexcept : value(other), storage(other.storage){}

    inline store<value>* get_store(){ return storage.get(); }
  };




  template<size_t alloc_size>
  class parser {

    const char      *pos;

    unsigned short  index      = 0;
    unsigned short  max        = 1;

    datalist<value> **     list_pointer;
    datalist<value> *      list;
    datalist<value> *      levels[32];


    store<value> *storage;

    static constexpr const int chr_value_true   = 0x65757274;
    static constexpr const int chr_value_false  = 0x736c6166;
    static constexpr const int chr_value_null   = 0x6c6c756e;


    inline bool node(value *result){
      switch(*pos){
        case '{': return object(result);
        case '[': return array(result);
        case '"': return string(result);

        default: switch(*(uint32_t *)pos){

          case chr_value_true:
            pos += 4;
            result->content = { t_true };
            return true;

          case chr_value_false:
            pos += 5;
            result->content = { t_false };
            return *(pos-1) == 'e';

          case chr_value_null:
            pos += 4;
            result->content = { t_null };
            return true;

          default:
            return number(result);
        }
      }
    }


    inline bool is_quote_escaped(){
      int i = 1;
      while( *(pos-i) == '\\') i++;
      return (i % 2)==0;
    }


    inline bool string(value *result){
      auto from = ++pos;

      while( *pos != '\0' ){
        pos = (char *)memchr((unsigned char *)pos, '"', 8196);
        if( !is_quote_escaped() ){
          result->set(t_string, (unsigned)(pos++ - from), (void *)from);
          return true;
        }
        pos++;
      }
      return false;
    }


    inline bool key(value *result){
      auto from = ++pos;

      while( *pos != '\0' ){
        pos = (char *)memchr((unsigned char *)pos, '"', 512);
        if( !is_quote_escaped() ){
          auto len = (unsigned)(pos++ - from);

          if(len < 16)
            result->set_str((unsigned char)len, from);
           else
            result->set(t_string, len, (void *)from);

          return true;
        }
        pos++;
      }
      return false;
    }


    inline bool number(value *result){

      if(*pos=='-' || *pos=='.' || (unsigned)(*pos-'0') <= 9 ) {

        const char *from = pos++;
        unsigned char rbuf[4];
        uint32_t *buf_ptr = (uint32_t *)rbuf;

        // no leading zeros
        if(from[0]=='0' && (from[1]=='e' || from[1]=='E' || ((unsigned)from[1]-'0')<=9))
          return false;

        while(true) {
          *buf_ptr = *(uint32_t*)(pos) - 0x30303030;

          if( rbuf[0] <= 9 ) pos++;
          else if(rbuf[0] == 254) goto read_double;
          else if(rbuf[0] == 0x15 || rbuf[0] == 0x35) goto read_exp;
          else { result->set( t_int_string, unsigned(pos-from),  (void *)from ); return true; }


          if( rbuf[1] <= 9 ) pos++;
          else if(rbuf[1] == 254) goto read_double;
          else if(rbuf[1] == 0x15 || rbuf[1] == 0x35) goto read_exp;
          else { result->set( t_int_string, unsigned(pos-from),  (void *)from ); return true; }

          if( rbuf[2] <= 9 ) pos++;
          else if(rbuf[2] == 254) goto read_double;
          else if(rbuf[2] == 0x15 || rbuf[2] == 0x35) goto read_exp;
          else { result->set( t_int_string, unsigned(pos-from),  (void *)from ); return true; }

          if( rbuf[3] <= 9 ) pos++;
          else if(rbuf[3] == 254) goto read_double;
          else if(rbuf[3] == 0x15 || rbuf[3] == 0x35) goto read_exp;
          else { result->set( t_int_string, unsigned(pos-from),  (void *)from ); return true; }
        }

      read_double:
        pos++;

        while(true) {
          *buf_ptr = *(uint32_t*)(pos) - 0x30303030;

          if(rbuf[0] <= 9) pos++;
          else if(rbuf[0] == 0x15 || rbuf[0] == 0x35) goto read_exp;
          else {result->set( t_double_string, unsigned(pos-from), (void *)from);  return true;}

          if(rbuf[1] <= 9) pos++;
          else if(rbuf[1] == 0x15 || rbuf[1] == 0x35) goto read_exp;
          else {result->set( t_double_string, unsigned(pos-from), (void *)from);  return true;}

          if(rbuf[2] <= 9) pos++;
          else if(rbuf[2] == 0x15 || rbuf[2] == 0x35) goto read_exp;
          else {result->set( t_double_string, unsigned(pos-from), (void *)from);  return true;}

          if(rbuf[3] <= 9) pos++;
          else if(rbuf[3] == 0x15 || rbuf[3] == 0x35) goto read_exp;
          else {result->set( t_double_string, unsigned(pos-from), (void *)from);  return true;}

        }

      read_exp:
        pos++;
        if(*pos=='-' || *pos=='+') pos++;

        *buf_ptr = *(uint32_t*)(pos) - 0x30303030;
        if((rbuf[0]<=9 && pos++) && ( rbuf[1]<=9 && pos++) && ( rbuf[2]<=9 && pos++) && (rbuf[3]<=9)) pos++;

        result->set( t_double_string, unsigned(pos-from), (void *)from);
        return true;
      }
      return false;
    }


    inline bool object(value *result){
      ++pos;
      push();

      unsigned requested_size = 32;
      unsigned size = 0;

      value *first = reserve(requested_size*2);
      value *last = first;

      spaces();

      while( *pos == '"' && key(last++)
        && spaces() && chr(':') && spaces() &&
        node(last++) && spaces() && ++size &&
        chr(',') && spaces()){

        if(size == requested_size) {
          auto prev_first = first;

          requested_size += requested_size;
          first = reserve(requested_size*2);

          if(first != prev_first)
            memcpy(first,prev_first,sizeof(value)*size*2);

          last = first + size*2;
        };
      }

      end_container(t_object, '}', result, first, last, size);
      return true;
    }



    inline bool array(value *result){
      ++pos;
      push();

      unsigned requested_size = 128;
      unsigned size = 0;

      value *first = reserve(requested_size);
      value *last = first;

      spaces();
      while( node(last++) && spaces() && ++size && chr(',') && spaces()){
        if(size == requested_size) {
          auto prev_first = first;
          requested_size += requested_size;
          first = reserve(requested_size);

          if(first != prev_first)
            memcpy(first,prev_first,sizeof(value)*size);

          last = first + size;
        };
      }


      end_container(t_array, ']', result, first, last, size);
      return true;
    }


    inline void end_container(vtype t, char x, value *result, value *first, value *last, unsigned size){
      if(chr(x)){
        spaces();
        result->set(t,size,first);
        list->pos = last;
        pop();
      } else throw parse_error(pos);
    }



    inline bool chr(char x){ return *pos == x && pos++; }


    inline bool spaces(){
      switch(*(uint32_t *)pos){
        // common cases
        case 0x2020200A:
        case 0x20202020:
          pos+=4;
        default:
          while(*pos == ' ' || *pos == '\t' || *pos == '\r' || *pos == '\n' ) pos++;
      }
      return true;
    }


    inline void push() {
      if (max != ++index)
        list = *(++list_pointer);
      else {
        max++;
        levels[index] = list = storage->track(alloc_size);
        list_pointer = &levels[index];
      }
    }


    inline void pop() {
      list = *(--list_pointer);
      index--;
    }


    inline value* reserve(size_t len){
      if( list->left() < len )
        levels[index] = list = storage->track(std::max(alloc_size,len));

      return list->pos;
    }

  public:

    inline parser(doc &ss, const char *src): storage(ss.get_store()){
      pos = src;
      spaces();

      switch(*pos){
        case '{':
          if(!(object(&ss) && *pos == '\0' && index == 0))
            throw parse_error(pos);
          break;

        case '[':
          if(!(array(&ss) && *pos == '\0' && index == 0))
            throw parse_error(pos);
          break;

        default:
          throw parse_error(pos);
      }
    }

  };


  template <int alloc_size=1024*16>
  inline static const doc parse(const char *str){
    doc result;
    parser<alloc_size>(result, str);
    return result;
  }


}}

#endif
