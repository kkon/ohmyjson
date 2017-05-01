# ohmyjson

**ohmyjson** is a header-only c++ library for extremely fast json parsing on 64 bit platforms.


Lightweight design and "lazy" data access make this parser work faster than other open-source libraries. 
Parser builds an index of pointers to json nodes that can be queried for contained json data while all expensive conversion operations are executed at the moment of accessing. 

## Example

*ohmy::json::parse(const char \*)* 
 will parse a string with json content and construct an immutable instance for accessing json node data.
 

```cpp
#include <iostream>
#include "ohmy/json.hpp"
using namespace ohmy;

int main(){
  auto data = json::parse(R"(
        {
          "id"           : 42,
          "title"        : "All right!",
          "list"         : [false, 1.2341, 3],
          "obj"          : { "a" : true, "b" : null}
        }
     )");

  auto num = data["id"].as_int();          // int    num = 42
  auto str = data["title"].as_string();    // string str = "All right!"
  auto bln = data["list"][0].as_bool();    // bool   bln = false
  auto dbl = data["list"][1].as_double();  // double dbl = 1.234

  auto sum = 0.0;
  // array iteration
  for(auto &item : data["list"]){
    if(item.is_double())   sum += item.as_double();
    else if(item.is_int()) sum += item.as_int();
  }

  std::cout << "sum is: " << sum << std::endl; // sum is: 4.2341

  // object iteration
  for(auto &item : data["obj"].as_object()){
    std::cout << "key " << item.key.as_string() << ": "
              << (item.value.is_bool() ? "bool" : "not bool") << std::endl;
  }

  return 0;
}
```


## Installation

Using cmake external project:

```cmake
include(ExternalProject)
externalproject_add(
  ohmyjson_ext
  PREFIX ${PROJECT_BINARY_DIR}/external/ohmyjson
  URL "https://github.com/kkon/ohmyjson/archive/master.zip"
  INSTALL_COMMAND ""
)

externalproject_get_property(ohmyjson_ext SOURCE_DIR)
add_dependencies(${your-project} ohmyjson_ext)
include_directories(${SOURCE_DIR})
```


## Benchmark

Uses [rapidjson](https://github.com/miloyip/rapidjson) parsing speed as baseline

* use ```make bench``` command to run the same benchmark on your platform
* result on *3.41GHz corei7-6700 osx clang* :

    2mb mixed-type generic data  | rel       | ops/sec
    -----------------------------|-----------|------------
    *ohmyjson*                   | x4.10     |**716.04**
    rapidjson                    | x1        |174.61
    
    2mb floating point data      | rel       | ops/sec
    -----------------------------|-----------|------------
    *ohmyjson*                   | x2.61     |**491.33**
    rapidjson                    | x1        |188.28




## License

The MIT License (MIT)

Copyright (c) 2017 Konstantin Konyakhin