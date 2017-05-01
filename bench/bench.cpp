#define RAPIDJSON_48BITPOINTER_OPTIMIZATION 1
#define RAPIDJSON_HAS_STDSTRING 1

#include <iostream>
#include <fstream>
#include <iomanip>

#include "bench.h"
#include "../test/read_file.h"

using namespace bench;
using namespace ohmy::test;


#include <rapidjson/document.h>
#include <ohmy/json.hpp>

using namespace ohmy;
using namespace ohmy::json;
using namespace std;


// ohmyjson vs rapidjson
int main() {
  std::cout << "Hello, World!" << std::endl;

  // Benchmark "canada set" (a lot of double values)

  string dt;
  read_file("./data/canada.json", dt);
  auto smf = dt.data();
  std::cout << console::cyan  << "strlen of canada set: " << strlen(smf) << console::reset << std::endl;

  for(int i = 0; i<3; ++i)
    time_compare(500, "Json parse canada set",
      func("ohmyjson",  [smf]{ doc sref = json::parse(smf); }),
      func("rapidjson", [smf]{ rapidjson::Document rapidjs; rapidjs.Parse<0>(smf); })
    );


  string json_people;
  read_file("./data/people.json", json_people);
  const char *filedata = json_people.data();

  {
    int sz = 0;
    int rsz = 0;

    for(int i = 0; i<3; ++i)
      time_compare(500, "Json parsing",
        func("ohmyjson ", [&]{ doc sref = json::parse(filedata); sz += sref.size(); }),
        func("rapidjson", [&]{ rapidjson::Document rapidjs; rapidjs.Parse<0>(filedata); rsz += rapidjs.GetObject().MemberCount();})
      );

    doc sref = json::parse(filedata);
    if(sz != rsz){
      std::cout << "wrong size: " << sz << " | rapidjson size: " << rsz << std::endl;
      return 1;
    }
  }


  try{
    int kj_array_items = 0;
    int rj_array_items = 0;
    int rj_object_items = 0;
    int kj_object_items = 0;
    int rj_null_items = 0;
    int kj_null_items = 0;



    auto doc = json::parse(filedata);
    auto kj_array  = doc["data"];
    auto kj_object = doc["data"][0].as_object();


    auto rapidjs_doc = rapidjson::Document();
    rapidjs_doc.Parse<0>(filedata);
    auto rj_array  = rapidjs_doc["data"].GetArray();
    auto rj_object = rapidjs_doc["data"][0].GetObject();


    time_compare( 50000, "Json object iteration",

      func("ohmyjson object", [&]{
        for(auto const &el: kj_object){
          kj_object_items++;
          kj_null_items += el.value.is_array();
        }
      }),

      func("rapidjson object", [&]{
        for(auto const &el: rj_object){
          rj_object_items++;
          rj_null_items += el.value.IsArray();
        }
      })
    );

    time_compare( 50000, "Json array iteration",
      func("ohmyjson array", [&]{
        for(auto const &el: kj_array){
          kj_array_items++;
          kj_null_items += el.is_null();
        }
      }),

      func("rapidjson array", [&]{
        for(auto const &el: rj_array){
          rj_array_items++;
          rj_null_items += el.IsNull();

        }
      })
    );

    printf("rapidjson objects: %d, arrays: %d, nulls: %d\nohmyjson objects: %d, arrays: %d, nulls: %d\n",
      rj_object_items, rj_array_items,rj_null_items,  kj_object_items, kj_array_items, kj_null_items);


    int nElements = (int)kj_array.size();
    double kj_double_sum = 0;
    double rj_double_sum = 0;


    time_compare( 500,
      "Json double access",

      func("ohmyjson double", [&]{
        for(int el = 0; el<nElements; el++)
          kj_double_sum += kj_array[el]["longitude"].as_double();
      }),

      func("rapidjson double", [&]{
        for(int el=0; el < nElements; el++)
          rj_double_sum += rj_array[el]["longitude"].GetDouble();
      })
    );


    printf("rapidjson double sum=%f \nohmyjson double sum=%f \n", rj_double_sum, kj_double_sum);


    int kj_int_sum = 0;
    int rj_int_sum = 0;


    for(int xx = 0; xx<3; ++xx)
      time_compare( 500,
        "Json int access",

        func("ohmyjson int", [&]{
          for(int el=0; el < nElements; el++)
            kj_int_sum += kj_array[el]["age"].as_int();
        }),

        func("rapidjson int", [&]{
          for(int el=0; el < nElements; el++)
            rj_int_sum += rj_array[el]["age"].GetInt();
        })
      );

    printf("rapidjson int sum=%d \nohmyjson int sum=%d \n", rj_int_sum, kj_int_sum);

    int kj_bool_sum = 0;
    int rj_bool_sum = 0;


    for(int xx = 0; xx<3; ++xx)
      time_compare( 500,
        "Json bool access",

        func("ohmyjson bool", [&]{
          for(int el=0; el < nElements; el++)
            kj_bool_sum += kj_array[el]["isActive"].as_bool();
        }),

        func("rapidjson bool", [&]{
          for(int el=0; el < nElements; el++)
            rj_bool_sum += rj_array[el]["isActive"].GetBool();
        })
      );

    printf("rapidjson bool sum=%d \nohmyjson bool sum=%d \n", rj_bool_sum, kj_bool_sum);

  } catch (bad_data_exception &de){
    printf("exception: %s\n",de.what());
    return 1;
  }

  return 0;
}

