#ifndef _MODELCOMPOUND_H
#define _MODELCOMPOUND_H

#include <string>
#include <tr1/memory>
#include <ostream>
#include "ModelCompartment.h"
#include "Compound.h"

class ModelCompound; // Forward reference

//! Smart pointer for ModelCompound class.
typedef std::tr1::shared_ptr<ModelCompound> ModelCompoundPtr;

//! Compound in metabolic model domain.

class ModelCompound
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	ModelCompound(Json::Value val);

	// Universally unique identifier to model compartment.
	string modelCompartmentUuid;

	//! Universally unique identifier to biochemistry compound.
	string compoundUuid;

	//! ??
	int charge;

	//! ??
	string formula;

	//! Last modification date.
	string modDate;

	//! Universally unique identifier.
	string uuid;

	//! Pointer to model compartment.
	ModelCompartmentPtr modelCompartment;

	//! Pointer to the biochemistry compound.
	CompoundPtr compound;

	//! True when the compound is a transporter between compartments.
	bool isTransporter;

	//! Pointer to paired model compound when the compound is a transporter between compartments.
	ModelCompoundPtr transporterCompound;

};

//! Formatted output operator for ModelCompound class.
std::ostream& operator<<(std::ostream& out, ModelCompound& obj);

#endif
