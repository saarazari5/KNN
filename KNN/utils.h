
#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <sstream>
#include <vector>

namespace utils {

template <typename T> std::vector<T>
split(const std::string &str, char delim, bool (*pred)(const std::string)) {
    std::vector<T> tokens;
    std::string token;
    std::stringstream ss(str);
    while (std::getline(ss, token, delim)) {
        if (pred != nullptr && !pred(token)) { continue; }
        T value;
        std :: stringstream conv(token);
        conv >> value;
        tokens.push_back(value);
    }
    return tokens;
}


template <typename T> std::vector<T>
until_where( const std::vector<T>& vector, bool (*pred)(const T)) {
    std::vector<T> tokens;
    for (const T& x : vector)  {
        if (pred(x)) {
            tokens.push_back(x);
        } else{
            return tokens;
        }
    }
    return tokens;
}
}

#endif /* utils_h */
