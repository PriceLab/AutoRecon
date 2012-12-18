#ifndef _COMPOUND_H
#define _COMPOUND_H

#include <string>
#include <tr1/memory>
#include <ostream>
#include <map>
#include <vector>
#include <jsoncpp/json/json.h>

using namespace std;

//! Compound in biochemistry domain.

class Compound
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	Compound(Json::Value val);

	//! Error bound on Gibbs free energy computation for compound (kcal/mol).
	double deltaGErr;

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
	double mass;

	//! True if universal cofactor (e.g. water/H+).
	bool isCofactor;

	//! Not currently implemented.
	string cksum;

	//! Computed Gibbs free energy of formation for compound at pH 7 (kcal/mol).
	double deltaG;

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
