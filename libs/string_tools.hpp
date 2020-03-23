#ifndef REPLACE_ALL_H
#define REPLACE_ALL_H
#include <string>

void replace_all(std::string* s, std::string to_replace, std::string by);

#endif

#ifndef SPLIT_LEN_H
#define SPLIT_LEN_H
#include <vector>
#include <string>

std::vector<std::string> split_len(std::string s, int len);

#endif
