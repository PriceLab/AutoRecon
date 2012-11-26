#ifndef _FILELOADER_H
#define _FILELOADER_H

#include "Models.h"

bool loadModelFile(const char* filename, MetabolicModel& model);

bool loadBiochemistryFile(const char* filename, Biochemistry& biochem);

bool validateProblem(Problem& problem);

#endif // _FILELOADER_H
