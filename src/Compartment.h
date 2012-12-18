#ifndef _COMPARTMENT_H
#define _COMPARTMENT_H

#include <string>
#include <tr1/memory>
#include <ostream>
#include <jsoncpp/json/json.h>

using namespace std;

//! Cell compartment in biochemistry domain.

class Compartment
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	Compartment(Json::Value val);

	//! Single character identifier for the compartment.
	string id;

	//! Relative position in hierarchy of cell compartments where extracellular is 0. A compartment
	//! contained within another compartment has an index that is +1 over the outer compartment.
	int hierarchy;

	//! Universally unique identifier.
	string uuid;

	//! Name of compartment.
	string name;

	//! Last modification date.
	string modDate;

};

//! Smart pointer for Compartment class.
typedef std::tr1::shared_ptr<Compartment> CompartmentPtr;

//! Formatted output operator for Compartment class.
std::ostream& operator<<(std::ostream& out, Compartment& obj);

#endif
