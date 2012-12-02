#include "FileLoader.h"
#include "Models.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << "Usage: ./a.out [biochem_json] [model_json] " << endl;
    exit(EXIT_FAILURE);
  }
  //! Parse JSON files.
  Problem problem;
  loadBiochemistryFile(argv[1], problem.biochem);
  loadModelFile(argv[2], problem.model);
  validateProblem(problem);

  //! Identify synonymous reactions
  map<ReactionPtr, vector<ReactionPtr> > = computeSynonyms(problem);

  return 0;
}
