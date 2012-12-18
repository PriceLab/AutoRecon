#ifndef _PROBLEM_H
#define _PROBLEM_H

#include "Biochemistry.h"
#include "Models.h"

class Problem
{
public:

	//! \brief Default constructor.

	Problem();

	//! \brief Validate the biochemistry and model.
	//! \return True if validation was successful.

	bool validate(void);

	//! \brief Check if a model compound is an external compound.
	//! \param[in] Pointer to model compound to check.
	//! \return True if external compound.

	bool isExternalCompound(ModelCompoundPtr compound);

	MetabolicModelPtr findPaths(SynonymMap synonymList, MediaPtr media, int numSolutions);

	BiochemistryPtr biochem;

	MetabolicModelPtr model;

};

#endif
