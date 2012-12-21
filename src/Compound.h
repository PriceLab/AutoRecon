#ifndef _COMPOUND_H
#define _COMPOUND_H

#include "AliasSet.h"
#include <string>
#include <tr1/memory>
#include <ostream>
#include <map>
#include <vector>
#include <jsoncpp/json/json.h>
#include <cstdlib>

using namespace std;

//! Type string for Compound objects.
static const string CompoundType = "Compound";

//! Compound in biochemistry domain.

class Compound
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	Compound(Json::Value val);

	//! \brief Build a string that represents the object for database table.
	//! \param[in] aliasSet Pointer to aliasSet for looking up id.
	//! \return String that represents object.

	string toDBString(AliasSetPtr aliasSet);

	//! \brief Get Gibbs free energy error bound as a number.
	//! \return Gibbs free energy error bound value.

	double deltaGErrValue(void) { return atof(deltaGErr.c_str()); }

	//! \brief Get Gibbs free energy as a number.
	//! \return Gibbs free energy value.

	double deltaGValue(void) { return atof(deltaG.c_str()); }

	//! \brief Get mass as a number.
	//! \return Mass value.

	double massValue(void) { return atof(mass.c_str()); }

	//! Error bound on Gibbs free energy computation for compound (kcal/mol).
	string deltaGErr;

	//! Hash of cue uuids with cue coefficients as values.
	map<string,int> cueList;

	//! Primary name of compound, for use in displaying reactions.
	string name;

	//! Hash of pKa values with atom numbers as values.
	map< string,vector<double> > pkaList;

	//! Last modification date.
	string modDate;

	//! Hash of pKb values with atom numbers as values.
	map< string,vector<double> > pkbList;

	//! Abbreviation for compound.
	string abbreviation;

	//! Atomic mass of the compound at pH 7.
	string mass;

	//! True if universal cofactor (e.g. water/H+).
	bool isCofactor;

	//! Not currently implemented.
	string cksum;

	//! Computed Gibbs free energy of formation for compound at pH 7 (kcal/mol).
	string deltaG;

	//! Formula for the compound at pH 7.
	string formula;

	//! Formula for compound if it does not have a ionic charge.
	string unchargedFormula; // Unused

	//! Array of associated molecular structures.
	vector<string> structureUuidList; // What is this a reference to?

	//! Computed charge for compound at pH 7.
	int defaultCharge;

	//! Universally unique identifier.
	string uuid;

};

//! Smart pointer for Compound class.
typedef std::tr1::shared_ptr<Compound> CompoundPtr;

//! Formatted output operator for Compound class.
std::ostream& operator<<(std::ostream& out, Compound& obj);

#endif
