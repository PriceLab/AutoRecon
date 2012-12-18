#ifndef _ALIASSET_H
#define _ALIASSET_H

#include <string>
#include <tr1/memory>
#include <ostream>
#include <map>
#include <jsoncpp/json/json.h>

using namespace std;

//! Alias set (do not understand this yet).

class AliasSet
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	AliasSet(Json::Value val);

	//! Name of alias set.
	string name;

	//! ??
	string attribute;

	//! Universally unique identifier.
	string uuid;

	//! ??
	string source;

	//! Last modification date.
	string modDate;

	//! ??
	string className;

	//! ??
	map<string,vector<string> > aliasList;

};

//! Smart pointer for AliasSet class.
typedef std::tr1::shared_ptr<AliasSet> AliasSetPtr;

//! Formatted output operator for AliasSet class.
std::ostream& operator<<(std::ostream& out, AliasSet& obj);

#endif
