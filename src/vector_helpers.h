#ifndef _VECTOR_HELPERS_H_
#define _VECTOR_HELPERS_H_

#include <string>
#include <vector>
using std::vector;
using std::string;

string join(vector<string>, string);
string join(string s, string glue, unsigned int n);
string repeat(string s, unsigned int n);
#endif
