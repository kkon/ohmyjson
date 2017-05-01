#include <cstdio>

#include "ohmy/json.hpp"
#include "read_file.h"


int run_pass_tests();
int run_fail_tests();

int main(){
  int pass_errors = run_pass_tests();
  int fail_errors = run_fail_tests();

  return pass_errors + fail_errors;
}




using namespace ohmy::json;
using namespace ohmy::test;
using namespace std;

bool try_pass_doc(int &errors, int file_num){
  string data, fname = "./data/json-org/pass" + to_string(file_num) + ".json";
  read_file(fname, data);

  try{
    auto jdoc = parse(data.c_str());
    if( jdoc.is(t_empty) ) errors++;
    cout << "ok result: " << file_num << endl;
    return true;
  }
  catch (parse_error &pe){
    cout << "Error while parsing " << fname << ": " << endl
         << pe.what() << endl
         << "Pass test " << fname << " failed" << endl;
    return false;
  }
}


int run_pass_tests(){
  int errors = 0;

  for(auto i = 1; i < 4; i++){
    if(!try_pass_doc(errors, i))
      cerr << "Parsing of document \"pass" << i << ".json\" is wrong" << endl;
  }
  cout << "\n\nPass tests are " << (errors ? "FAILED" : "OK") << "\n\n\n"<< endl;
  return errors;
}


bool try_fail_doc(int &errors, int file_num){
  switch(file_num){

    // final comma is allowed
    case 4:
    case 9:

    // string escapes are not checked
    case 15:
    case 17:

    // no limit for deep objects
    case 18:

    // string escapes are not checked
    case 25:
    case 26:
    case 27:
    case 28:
      return true;


    default:
      string data, fname = "./data/json-org/fail" + to_string(file_num) + ".json";
      read_file(fname, data);

      try{
        auto jdoc = parse(data.c_str());
        if( !jdoc.is(t_empty) ) errors++;
        cerr << "No errors while parsing " << fname << ": " << "ko!" << endl;
        return false;
      }
      catch (parse_error &pe){
        cout << "Expected error while parsing " << fname << ": " << "ok!" << endl;
        return true;
      }
  }
}

int run_fail_tests(){
  int errors = 0;
  for(auto i = 1; i < 33; i++){
    if(!try_fail_doc(errors, i))
      cerr << "Parsing of document \"fail" << i << ".json\" is wrong" << endl;
  }
  cout << "\n\nFail tests are " << (errors ? "FAILED" : "OK") << "\n\n\n"<< endl;

  return errors;
}

