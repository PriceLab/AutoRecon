#ifndef _MODELCOMPARTMENT_H
#define _MODELCOMPARTMENT_H

#include <string>
#include <tr1/memory>
#include <ostream>
#include "Compartment.h"

//! Compartment in metabolic model domain.

class ModelCompartment
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	ModelCompartment(Json::Value val);

	//! ?? pH value
	double pH;

	//! ??
	int compartmentIndex;

	//! ??
	double potential;

	//! Last modification date.
	string modDate;

	//! Universally unique identifier to biochemistry domain compartment.
	string compartmentUuid;

	//! ??
	string label;

	//! Universally unique identifier.
	string uuid;

	//! Pointer to biochemistry domain compartment.
	CompartmentPtr compartment;

};

//! Smart pointer for ModelCompartment class.
typedef std::tr1::shared_ptr<ModelCompartment> ModelCompartmentPtr;

//! Formatted output operator for ModelCompartment class.
std::ostream& operator<<(std::ostream& out, ModelCompartment& obj);

#endif
