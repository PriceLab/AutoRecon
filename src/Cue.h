#ifndef _CUE_H
#define _CUE_H

#include <string>
#include <tr1/memory>
#include <ostream>
#include <jsoncpp/json/json.h>

using namespace std;

//! Structural cue for parts of compound structures (do not understand this yet).

class Cue
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	Cue(Json::Value val);

	//! Abbreviation for cue.
	string abbreviation;

	//! ??
	double deltaGErr;

	//! Universally unique identifier.
	string uuid;

	//! ??
	bool smallMolecule;

	//! Last modification date.
	string modDate;

	//! ??
	int priority;

	//! Checksum (unused).
	string cksum;

	//! ??
	double deltaG;

	//! ??
	string formula;

	//! ??
	string unchargedFormula;

	//! ??
	int defaultCharge;

	//! Name of cue.
	string name;

};

//! Smart pointer for Cue class.
typedef std::tr1::shared_ptr<Cue> CuePtr;

//! Formatted output operator for Cue class.
std::ostream& operator<<(std::ostream& out, Cue& obj);

#endif
