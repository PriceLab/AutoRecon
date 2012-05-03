#include "vector_helpers.h"

#include <string>
#include <vector>
#include <map>
using std::vector;
using std::map;
using std::string;

using namespace std;

string join(vector<string> v, string glue) {
  string answer="";
  vector<string>::iterator it=v.begin();
  while (it != v.end()) {
    answer+=*it;
    it++;
    if (it != v.end()) answer+=glue;
  }
  return answer;
}

string join(string s, string glue, unsigned int n) {
  if (n==0) return string("");
  string answer=repeat(s+glue, n-1);
  answer+=s;
  return answer;
}

string repeat(string s, unsigned int n) {
  string answer="";
  while (n--) {
    answer+=s;
  }
  return answer;
}
