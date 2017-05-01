#ifndef OHMYJSON_READ_FILE_H
#define OHMYJSON_READ_FILE_H

#include <fstream>
#include <string>

using namespace std;

namespace ohmy { namespace test {

  int read_file(const string &path, string &data){
    std::ifstream file;
    file.open(path.c_str(), std::ios::in | std::ios::binary);

    if(!file) return -1;

    file.seekg(0, std::ios::end);
    auto size = (unsigned long)file.tellg();
    data.reserve(size);

    file.seekg(0, std::ios::beg);
    data.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();
    return (int)size;
  }

}}

#endif
