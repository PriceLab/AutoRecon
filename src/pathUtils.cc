#include "pathUtils.h"

void runMFAToolkit(BiochemistryPtr biochem)
{
	// Here's what ModelSeed does.
	// Biochemistry->printDBFiles() method puts compounds.tbl and reactions.tbl files in /tmp/fbafiles if they don't exist (unless forceprint was specified).

	biochem->printDBFiles("/tmp/fbafile", true);

	// Would be nice to figure out a way to leverage ModelSEED code for this.

	return;
}
