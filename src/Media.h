#ifndef _MEDIA_H
#define _MEDIA_H

#include <string>
#include <tr1/memory>
#include <ostream>
#include <vector>
#include "Compound.h"

//! Media compound in biochemistry domain.

class MediaCompound
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	MediaCompound(Json::Value val);

	//! ?? what units
	double concentration;

	//! ??
	double maxFlux;

	//! ??
	double minFlux;

	//! Universally unique identifier to compound.
	string compoundUuid;

	//! Pointer to the biochemistry compound.
	CompoundPtr compound;
};

//! Smart pointer for MediaCompound class.
typedef std::tr1::shared_ptr<MediaCompound> MediaCompoundPtr;

//! Formatted output operator for MediaCompound class.
std::ostream& operator<<(std::ostream& out, MediaCompound& obj);

//! Media in biochemistry domain.

class Media
{
public:

	//! \brief Constructor.
	//! \param[in] val Object represented in JSON format.

	Media(Json::Value val);

	//! Descriptive name of media.
	string name;

	//! Type of media.
	string type;

	//! True if this media condition is defined (all components explicitly known).
	bool isDefined;

	//! Last modification date.
	string modDate;

	//! ??
	bool isMinimal;

	//! List of media compounds.
	vector<MediaCompoundPtr> mediaCompoundList;

	//! ??
	string id;

	//! Universally unique identifier.
	string uuid;

};

//! Smart pointer for Media class.
typedef std::tr1::shared_ptr<Media> MediaPtr;

//! Formatted output operator for Media class.
std::ostream& operator<<(std::ostream& out, Media& obj);

#endif
