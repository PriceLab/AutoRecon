#ifndef _BIOMASS_H
#define _BIOMASS_H

#include <string>
#include <tr1/memory>
#include <ostream>
#include <vector>
#include <jsoncpp/json/json.h>
#include "ModelCompound.h"

using namespace std;

//! Biomass compound in metabolic model domain.

class BiomassCompound
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	BiomassCompound(Json::Value val);

	//! Universally unique identifier to model compound.
	string modelCompoundUuid;

	//! ?? what units
	double coefficient;

	//! Pointer to model compound.
	ModelCompoundPtr modelCompound;

};

//! Smart pointer for BiomassCompound class.
typedef std::tr1::shared_ptr<BiomassCompound> BiomassCompoundPtr;

//! Biomass in metabolic model domain.

class Biomass
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	Biomass(Json::Value val);

	//! Portion of a gram of this biomass that is DNA (expressed as a fraction of 1.0).
	double dna;

	//! Portion of a gram of this biomass that is RNA (expressed as a fraction of 1.0).
	double rna;

	//! Portion of a gram of this biomass that functions as cofactors (expressed as a fraction of 1.0).
	double cofactor;

	//! Number of ATP molecules hydrolized per gram of this biomass.
	int energy;

	//! ??
	bool locked; // bool?

	//! Descriptive name of biomass.
	string name;

	//! Portion of a gram of this biomass that is cell wall (expressed as a fraction of 1.0).
	double cellwall;

	//! Universally unique identifier.
	string uuid;

	//! Portion of a gram of this biomass that is protein (expressed as a fraction of 1.0).
	double protein;

	//! List of compounds in biomass.
	vector<BiomassCompoundPtr> biomassCompoundList; //! \todo Should this be a map keyed by model compound uuid?

	//! Last modification date.
	string modDate;

	//! Portion of a gram of this biomass that is lipid but not part of the cell wall (expressed as a fraction of 1.0).
	double lipid;

};

//! Smart pointer for Biomass class.
typedef std::tr1::shared_ptr<Biomass> BiomassPtr;

//! Formatted output operator for Biomass class.
std::ostream& operator<<(std::ostream& out, Biomass& obj);

#endif
