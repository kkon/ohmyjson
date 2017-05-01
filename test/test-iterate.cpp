#include "ohmy/json.hpp"
#include <cmath>

using namespace ohmy;
using namespace ohmy::json;

const char *test_str = R"(
   {
     "id"           : 42,
     "title"        : "All right!",
     "list"         : [false, 1.2341, 3, "hello", 21, null, ["sub"], 0.1104 ],
     "obj"          : {
        "a": true,
        "b": 1.2341,
        "c": 3,
        "d": "hello",
        "e": 21,
        "f": null,
        "g": ["sub"],
        "h": 0.1104
      }
   }
)";


bool iterate_list_test();
bool iterate_object_test();


int main(){
  return !iterate_object_test() || !iterate_list_test();
}



bool iterate_list_test(){
  auto data = json::parse(test_str);

  auto n_items = 0u;
  auto sum = 0.0;
  // array iteration
  for(auto &item : data["list"]){
    n_items++;

    if( item.is_double()) sum += item.as_double();
    else if( item.is_int()) sum += item.as_int();
  }

  auto result_sum = std::abs(sum - 25.3445) < 0.001;
  std::cout << "sum is " << sum << ": " << (result_sum ? "OK" : "KO") << std::endl;

  auto result_items = (n_items == 8);
  std::cout << "n_items is: " << n_items << ": " << (result_items ? "OK" : "KO") << std::endl;

  return result_sum && result_items && data.contains("list");
}



bool iterate_object_test(){
  auto data = json::parse(test_str);

  // object iteration
  auto n_items = 0u;
  auto sum = 0.0;
  for(auto &item : data["obj"].as_object()){
    n_items++;

    if( item.value.is_double()) sum += item.value.as_double();
    else if( item.value.is_int()) sum += item.value.as_int();

    std::cout << "key " << item.key.as_string() << ": sum = " << sum << std::endl;
  }

  auto result_sum = std::abs(sum - 25.3445) < 0.001;
  std::cout << "sum is " << sum << ": " << (result_sum ? "OK" : "KO") << std::endl;

  auto result_items = (n_items == 8);
  std::cout << "n_items is: " << n_items << ": " << (result_items ? "OK" : "KO") << std::endl;

  return result_sum && result_items && data.contains("obj");
}

