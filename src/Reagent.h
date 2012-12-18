#ifndef _REAGENT_H
#define _REAGENT_H

#include <string>
#include <tr1/memory>
#include <ostream>
#include "Compound.h"
#include "Compartment.h"

//! Reagent in biochemistry domain.

class Reagent
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	Reagent(Json::Value val);

	//! ?? what units.
	int coefficient;

	//! ??
	bool isCofactor;

	//! Universally unique identifier to compound.
	string compoundUuid;

	//! Universally unique identifier to compartment.
	string compartmentUuid;

	//! Pointer to biochemistry compound.
	CompoundPtr compound;

	//! Pointer to biochemistry compartment.
	CompartmentPtr compartment;

};

//! Smart pointer for Reagent class.
typedef std::tr1::shared_ptr<Reagent> ReagentPtr;

//! Formatted output operator for Reagent class.
std::ostream& operator<<(std::ostream& out, Reagent& obj);

#endif
