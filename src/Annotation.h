#ifndef __ANNOTATION_H__
#define __ANNOTATION_H__

#include <string>
using namespace std;

struct ANNOTATION{
  double probability;
  string genename;
  /* THis compares > because we want to go in opposite order.. when sorting these. */
  bool operator<(const ANNOTATION &rhs) const;
};

#endif
