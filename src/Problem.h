#ifndef _PROBLEM_H
#define _PROBLEM_H

#include "Biochemistry.h"
#include "Models.h"

//! Description of problem to solve.

class Problem
{
public:

	//! \brief Default constructor.

	Problem();

	//! \brief Validate the biochemistry and model.
	//! \return True if validation was successful.

	bool validate(void);

	//! \brief Check if a model compound is an external compound.
	//! \param[in] compound Pointer to model compound to check.
	//! \return True if external compound.

	bool isExternalCompound(ModelCompoundPtr compound);

	//! \brief Run path finding algorithm.
	//! \param[in] synonymList List of reactions and all of the reactions that are synonyms.
	//! \param[in] media Pointer to media.
	//! \param[in] numSolutions Number of solutions to find.
	//! \return Pointer to model describing solution.

	MetabolicModelPtr findPaths(SynonymMap synonymList, MediaPtr media, int numSolutions);

	//! Biochemistry domain data.
	BiochemistryPtr biochem;

	//! Metabolic model domain data.
	MetabolicModelPtr model;

};

#endif
