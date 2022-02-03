#pragma once

#include <vector>
#include <string>
#include <sstream>

class StringUtils {
public:
  static std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> result;
    std::stringstream sstream(str);
    std::string s;
    while (std::getline(sstream, s, delim)) {
      result.push_back(s);
    }

    return result;
  }
};