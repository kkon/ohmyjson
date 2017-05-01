
#include <iostream>

#include "ohmy/json.hpp"

using namespace ohmy;


int main(){

  int errors = 0;

  {

    auto dc = json::parse(R"(
    {
      "val_double"        : 12.245,
      "val_int"           : 42,
      "val_bool"          : true,
      "val_str"           : "content with escaped \" char",
      "val_null"          : null,
      "val_empty_object"  : {},
      "val_empty_array"   : [],
      "val_object_2_el"   : { "key_a" : 1, "key_b" : 20.8},
      "val_array_2_el"    : [ 1, "element_c" ]
    }
    )");

    if( dc.size() != 9 || !dc.is_object() )
      std::cerr << "Error " << ++errors << " wrong document" << std::endl;

    if( !dc["val_double"].is_double() || dc["val_double"].as_double() != 12.245 )
      std::cerr << "Error " << ++errors << " wrong double" << std::endl;

    if( !dc["val_object_2_el"]["key_b"].is_double() || dc["val_object_2_el"]["key_b"].as_double() != 20.8 )
      std::cerr << "Error " << ++errors << " wrong double" << std::endl;

    if( !dc["val_int"].is_int() || dc["val_int"].as_int() != 42 )
      std::cerr << "Error " << ++errors << " wrong int" << std::endl;

    if( !dc["val_bool"].is_bool() || dc["val_bool"].as_bool() != true )
      std::cerr << "Error " << ++errors << " wrong boolean" << std::endl;

    if( !dc["val_null"].is_null() )
      std::cerr << "Error " << ++errors << " wrong null" << std::endl;

    if( !dc["val_str"].is_string() || dc["val_str"].as_string() != "content with escaped \\\" char" )
      std::cerr << "Error " << ++errors << " wrong string" << std::endl;

    if( !dc["val_empty_object"].is_object() || dc["val_empty_object"].size() != 0 )
      std::cerr << "Error " << ++errors << " wrong empty object" << std::endl;

    if( !dc["val_empty_array"].is_array() || dc["val_empty_array"].size() != 0 )
      std::cerr << "Error " << ++errors << " wrong empty array" << std::endl;

    if( !dc["val_object_2_el"].is_object() || dc["val_object_2_el"].size() != 2 )
      std::cerr << "Error " << ++errors << " wrong non-empty object" << std::endl;

    if( !dc["val_array_2_el"].is_array() || dc["val_array_2_el"].size() != 2 )
      std::cerr << "Error " << ++errors << " wrong non-empty array" << std::endl;

  }


  std::cout << ((errors == 0) ? "test passed" : "test failed") << std::endl;
  return errors;
}