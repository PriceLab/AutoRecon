#include "Models.h"
#include <stdlib.h>

AliasSet::AliasSet(Json::Value val)
{
	attribute = val["attribute"].asString();
	source = val["source"].asString();
	name = val["name"].asString();
	modDate = val["modDate"].asString();
	className = val["class"].asString();
	uuid = val["uuid"].asString();
	Json::Value aliases = val["aliases"];
	Json::Value::Members members = aliases.getMemberNames();
	for (Json::Value::Members::iterator iter = members.begin(); iter != members.end(); iter++)
	{
		vector<string> uuidList;
		Json::Value alias = aliases[*iter];
		for (int index = 0; index < alias.size(); index++)
		{
			uuidList.push_back(alias[index].asString());
		}
		aliasList.insert(pair<string,vector<string> >(*iter, uuidList));
	}
}

std::ostream& operator<<(std::ostream& out, AliasSet& obj)
{
	out << "-AliasSet-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; attribute: " << obj.attribute << "; source: " << obj.source;
	out << "; class: " << obj.className << "; aliasList size: " << obj.aliasList.size();
	return out;
}

Cue::Cue(Json::Value val)
{
	priority = val["priority"].asInt();
	defaultCharge = atoi(val["defaultCharge"].asString().c_str());
	deltaG = atof(val["deltaG"].asString().c_str());
	name = val["name"].asString();
	cksum = val["cksum"].asString();
	uuid = val["uuid"].asString();
	smallMolecule = atoi(val["smallMolecule"].asString().c_str());
	formula = val["formula"].asString();
	modDate = val["modDate"].asString();
	abbreviation = val["abbreviation"].asString();
	unchargedFormula = val["unchargedFormula"].asString();
	deltaGErr = atof(val["deltaGErr"].asString().c_str());
}

std::ostream& operator<<(std::ostream& out, Cue& obj)
{
	out << "-Cue-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; abbreviation: " << obj.abbreviation << "; defaultCharge: " << obj.defaultCharge;
	out << "; smallMolecule: " << obj.smallMolecule << "; deltaG: " << obj.deltaG << "; deltaGErr: " << obj.deltaGErr;
	out << "; formula: " << obj.formula << "; unchargedFormula: " << obj.unchargedFormula;
	out << "; priority: " << obj.priority << "; cksum: " << obj.cksum;
	return out;
}

Compartment::Compartment(Json::Value val)
{
	name = val["name"].asString();
	modDate = val["modDate"].asString();
	id = val["id"].asString();
	uuid = val["uuid"].asString();
	hierarchy = val["hierarchy"].asInt();
}

std::ostream& operator<<(std::ostream& out, Compartment& obj)
{
	out << "-Compartment-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; id: " << obj.id << "; hierarchy: " << obj.hierarchy;
	return out;
}

Compound::Compound(Json::Value val)
{
	defaultCharge = atoi(val["defaultCharge"].asString().c_str());
	isCofactor = atoi(val["isCofactor"].asString().c_str());
	deltaG = atof(val["deltaG"].asString().c_str());
	name = val["name"].asString();
	cksum = val["cksum"].asString();
	uuid = val["uuid"].asString();
	formula = val["formula"].asString();
	mass = atof(val["mass"].asString().c_str());
	modDate = val["modDate"].asString();
	abbreviation = val["abbreviation"].asString();
	unchargedFormula = val["unchargedFormula"].asString();
	deltaGErr = atof(val["deltaGErr"].asString().c_str());
	Json::Value cues = val["cues"];
	Json::Value::Members cueMembers = cues.getMemberNames();
	for (Json::Value::Members::iterator iter = cueMembers.begin(); iter != cueMembers.end(); iter++)
	{
		try
		{
			cueList.insert(pair<string,int>(*iter,cues[*iter].asInt()));
		}
		catch (std::exception& e)
		{
			string str = cues[*iter].asString();
			cueList.insert(pair<string,int>(*iter,atoi(str.c_str())));
		}
	}
	Json::Value pkas = val["pkas"];
	Json::Value::Members pkaMembers = pkas.getMemberNames();
	for (Json::Value::Members::iterator iter = pkaMembers.begin(); iter != pkaMembers.end(); iter++)
	{
		Json::Value pka = pkas[*iter];
		vector<double> pkaValues;
		for (int index = 0; index < pka.size(); index++)
		{
			pkaValues.push_back(atof(pka[index].asString().c_str()));
		}
		pkaList.insert(pair< string,vector<double> >(*iter,pkaValues));
	}
	Json::Value pkbs = val["pkbs"];
	Json::Value::Members pkbMembers = pkbs.getMemberNames();
	for (Json::Value::Members::iterator iter = pkbMembers.begin(); iter != pkbMembers.end(); iter++)
	{
		Json::Value pkb = pkbs[*iter];
		vector<double> pkbValues;
		for (int index = 0; index < pkb.size(); index++)
		{
			pkbValues.push_back(atof(pkb[index].asString().c_str()));
		}
		pkbList.insert(pair< string,vector<double> >(*iter,pkbValues));
	}

	Json::Value structureUuids = val["structure_uuids"];
	for (int index = 0; index < structureUuids.size(); index++)
	{
		structureUuidList.push_back(structureUuids[index].asString());
	}
}

std::ostream& operator<<(std::ostream& out, Compound& obj)
{
	out << "-Compound-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; abbreviation: " << obj.abbreviation << "; defaultCharge: " << obj.defaultCharge;
	out << "; isCofactor: " << obj.isCofactor << "; deltaG: " << obj.deltaG << "; deltaGErr: " << obj.deltaGErr;
	out << "; formula: " << obj.formula << "; unchargedFormula: " << obj.unchargedFormula;
	out << "; mass: " << obj.mass << "; cksum: " << obj.cksum;
	out << "; cueList size: " << obj.cueList.size() << "; structureUuidList size: " << obj.structureUuidList.size();
	out << "; pkaList size: " << obj.pkaList.size() << "; pkbList size: " << obj.pkbList.size();
	return out;
}

MediaCompound::MediaCompound(Json::Value val)
{
	compoundUuid = val["compound_uuid"].asString();
	concentration = atof(val["concentration"].asString().c_str());
	maxFlux = atof(val["maxFlux"].asString().c_str());
	minFlux = atof(val["minFlux"].asString().c_str());
}

std::ostream& operator<<(std::ostream& out, MediaCompound& obj)
{
	out << "-MediaCompound-" << endl;
	out << "compoundUuid: " << obj.compoundUuid << "; concentration: " << obj.concentration;
	out << "; minFlux: " << obj.minFlux << "; maxFlux: " << obj.maxFlux;
	return out;
}

Media::Media(Json::Value val)
{
	isMinimal = atoi(val["isMinimal"].asString().c_str());
	name = val["name"].asString();
	modDate = val["modDate"].asString();
	id = val["id"].asString();
	type = val["type"].asString();
	uuid = val["uuid"].asString();
	isDefined = atoi(val["isDefined"].asString().c_str());
	Json::Value mediaCompounds = val["mediacompounds"];
	for (Json::ValueIterator iter = mediaCompounds.begin(); iter != mediaCompounds.end(); iter++)
	{
		MediaCompoundPtr mc(new MediaCompound(*iter));
		mediaCompoundList.push_back(mc);
	}
}

std::ostream& operator<<(std::ostream& out, Media& obj)
{
	out << "-Media-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; id: " << obj.id << "; type: " << obj.type;
	out << "; isMinimal: " << obj.isMinimal << "; isDefined: " << obj.isDefined;
	out << "; mediaCompoundList size: " << obj.mediaCompoundList.size();
	return out;
}

Reagent::Reagent(Json::Value val)
{
	isCofactor = atoi(val["isCofactor"].asString().c_str());
	compoundUuid = val["compound_uuid"].asString();
	compartmentUuid = val["compartment_uuid"].asString();
	coefficient = val["coefficient"].asInt();
}

std::ostream& operator<<(std::ostream& out, Reagent& obj)
{
	out << "-Reagent-" << endl;
	out << "compoundUuid: " << obj.compoundUuid << "; compartmentUuid: " << obj.compartmentUuid;
	out << "; isCofactor: " << obj.isCofactor << "; coefficient: " << obj.coefficient;
	return out;
}

Reaction::Reaction(Json::Value val)
{
	status = val["status"].asString();
	direction = val["direction"].asString();
	deltaGErr = atof(val["deltaGErr"].asString().c_str());
	Json::Value cues = val["cues"];
	Json::Value::Members members = cues.getMemberNames();
	try
	{
		for (Json::Value::Members::iterator mbrIter = members.begin(); mbrIter != members.end(); mbrIter++)
		{
			cueList.insert(pair<string,string>(*mbrIter,cues[*mbrIter].asString()));
		}
	}
	catch (std::exception& e)
	{
		cerr << "invalid cue for reaction " << name << " with uuid " << uuid << endl;
	}
	name = val["name"].asString();
	try
	{
		probability = val["probability"].asDouble();
	}
	catch (std::exception& e)
	{
		probability = atof(val["probability"].asString().c_str());
	}
	modDate = val["modDate"].asString();
	abbreviation = val["abbreviation"].asString();
	cksum = val["cksum"].asString();
	deltaG = atof(val["deltaG"].asString().c_str());
	defaultProtons = atoi(val["defaultProtons"].asString().c_str());
	Json::Value reagents = val["reagents"];
	for (Json::ValueIterator iter = reagents.begin(); iter != reagents.end(); iter++)
	{
		ReagentPtr reagent(new Reagent(*iter));
		reagentList.push_back(reagent);
	}
	complexInfo = val["complexinfo"].asString();
	thermoReversibility = val["thermoReversibility"].asString();
	GPR = val["GPR"].asString();
	uuid = val["uuid"].asString();
}

std::ostream& operator<<(std::ostream& out, Reaction& obj)
{
	out << "-Reaction-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; abbreviation: " << obj.abbreviation << "; status: " << obj.status;
	out << "; deltaG: " << obj.deltaG << "; deltaGErr: " << obj.deltaGErr;
	out << "; direction: " << obj.direction << "; thermoReversibility: " << obj.thermoReversibility;
	out << "; defaultProtons: " << obj.defaultProtons << "; probability: " << obj.probability;
	out << "; cksum: " << obj.cksum << "; complexInfo: " << obj.complexInfo << "; GPR: " << obj.GPR;
	out << "; reagentList size: " << obj.reagentList.size() << "; cueList size: " << obj.cueList.size();
	return out;
}

Biochemistry::Biochemistry()
{
	version = 0;
}

std::ostream& operator<<(std::ostream& out, Biochemistry& obj)
{
	out << "-Biochemistry-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; version: " << obj.version << "; defaultNameSpace: " << obj.defaultNameSpace;
	out << "; biochemistryStructures_uuid: " << obj.biochemistryStructures_uuid;
	return out;
}

BiomassCompound::BiomassCompound(Json::Value val)
{
	modelCompoundUuid = val["modelcompound_uuid"].asString();
	coefficient = val["coefficient"].asDouble();
}

Biomass::Biomass(Json::Value val)
{
	dna = atof(val["dna"].asString().c_str());
	rna = atof(val["rna"].asString().c_str());
	cofactor = atof(val["cofactor"].asString().c_str());
	energy = val["energy"].asInt();
	locked = atoi(val["locked"].asString().c_str());
	name = val["name"].asString();
	cellwall = atof(val["cellwall"].asString().c_str());
	uuid = val["uuid"].asString();
	protein = atof(val["protein"].asString().c_str());
	modDate = val["modDate"].asString();
	lipid = atof(val["lipid"].asString().c_str());
	Json::Value biomasscompounds = val["biomasscompounds"];
	for (Json::ValueIterator iter = biomasscompounds.begin(); iter != biomasscompounds.end(); iter++)
	{
		BiomassCompoundPtr biomassCompound(new BiomassCompound(*iter));
		biomassCompoundList.push_back(biomassCompound);
	}
}

std::ostream& operator<<(std::ostream& out, Biomass& obj)
{
	out << "-Biomass-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; dna: " << obj.dna << "; rna: " << obj.rna << "; cofactor: " << obj.cofactor;
	out << "; energy: " << obj.energy << "; locked: " << obj.locked << "; protein: " << obj.protein;
	out << "; lipid: " << obj.lipid << "; biomassCompoundList size: " << obj.biomassCompoundList.size();
	return out;
}

ModelCompound::ModelCompound(Json::Value val)
{
	modelCompartmentUuid = val["modelcompartment_uuid"].asString();
	compoundUuid = val["compound_uuid"].asString();
	charge = atoi(val["charge"].asString().c_str());
	formula = val["formula"].asString();
	modDate = val["modDate"].asString();
	uuid = val["uuid"].asString();
	isTransporter = false;
}

std::ostream& operator<<(std::ostream& out, ModelCompound& obj)
{
	out << "-ModelCompound-" << endl;
	out << "uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; compoundUuid: " << obj.compoundUuid << "; modelCompartmentUuid: " << obj.modelCompartmentUuid;
	out << "; charge: " << obj.charge << "; formula: " << obj.formula;
	return out;
}

ModelCompartment::ModelCompartment(Json::Value val)
{
	pH = val["pH"].asDouble();
	compartmentIndex = val["compartmentIndex"].asInt();
	potential = atof(val["potential"].asString().c_str());
	modDate = val["modDate"].asString();
	compartmentUuid = val["compartment_uuid"].asString();
	label = val["label"].asString();
	uuid = val["uuid"].asString();
}

std::ostream& operator<<(std::ostream& out, ModelCompartment& obj)
{
	out << "-ModelCompartment-" << endl;
	out << "uuid: " << obj.uuid << "; modDate: " << obj.modDate << "; label: " << obj.label;
	out << "; pH: " << obj.pH << "; potential: " << obj.potential;
	out << "; compartmentUuid: " << obj.compartmentUuid << "; compartmentIndex: " << obj.compartmentIndex;
	return out;
}

ModelReactionReagent::ModelReactionReagent(Json::Value val)
{
	modelCompoundUuid = val["modelcompound_uuid"].asString();
	coefficient = val["coefficient"].asDouble();
}

ModelReactionProteinSubunit::ModelReactionProteinSubunit(Json::Value val)
{
	roleUuid = val["role_uuid"].asString();
	triggering = atoi(val["triggering"].asString().c_str());
	optional = atoi(val["optional"].asString().c_str());
	Json::Value genes = val["modelReactionProteinSubunitGenes"];
	for (Json::ValueIterator iter = genes.begin(); iter != genes.end(); iter++)
	{
		Json::Value gene = *iter;
		geneList.push_back(gene["feature_uuid"].asString());
	}
}

ModelReactionProtein::ModelReactionProtein(Json::Value val)
{
	complexUuid = val["complex_uuid"].asString();
	note = val["note"].asString();
	Json::Value subunits = val["modelReactionProteinSubunits"];
	for (Json::ValueIterator iter = subunits.begin(); iter != subunits.end(); iter++)
	{
		ModelReactionProteinSubunitPtr subunit(new ModelReactionProteinSubunit(*iter));
		reactionProteinSubunitList.push_back(subunit);
	}
}

ModelReaction::ModelReaction(Json::Value val)
{
	direction = val["direction"].asString();
	modelCompartmentUuid = val["modelcompartment_uuid"].asString();
	modDate = val["modDate"].asString();
	uuid = val["uuid"].asString();
	protons = atoi(val["protons"].asString().c_str());
	reactionUuid = val["reaction_uuid"].asString();
	Json::Value reagents = val["modelReactionReagents"];
	for (Json::ValueIterator iter = reagents.begin(); iter != reagents.end(); iter++)
	{
		ModelReactionReagentPtr reagent(new ModelReactionReagent(*iter));
		modelReactionReagentList.push_back(reagent);
	}
	Json::Value proteins = val["modelReactionProteins"];
	for (Json::ValueIterator iter = proteins.begin(); iter != proteins.end(); iter++)
	{
		ModelReactionProteinPtr protein(new ModelReactionProtein(*iter));
		modelReactionProteinList.push_back(protein);
	}
}

std::ostream& operator<<(std::ostream& out, ModelReaction& obj)
{
	out << "-ModelReaction-" << endl;
	out << "uuid: " << obj.uuid << "; modDate: " << obj.modDate << "; direction: " << obj.direction;
	out << "; modelCompartmentUuid: " << obj.modelCompartmentUuid << "; reactionUuid: " << obj.reactionUuid;
	out << "; protons: " << obj.protons << "; reagentList size: " << obj.modelReactionReagentList.size();
	out << "; proteinList size: " << obj.modelReactionProteinList.size();
	return out;
}

std::ostream& operator<<(std::ostream& out, MetabolicModel& obj)
{
	out << "-MetabolicModel-" << endl;
	out << "name: " << obj.name << "; uuid: " << obj.uuid << "; modDate: " << obj.modDate;
	out << "; version: " << obj.version << "; id: " << obj.id << "; status: " << obj.status;
	out << "; type: " << obj.type << "; defaultNameSpace: " << obj.defaultNameSpace;
	out << "; current: " << obj.current << "; growth: " << obj.growth;
	out << "; annotationUuid; " << obj.annotationUuid;
	out << "; biochemistryUuid: " << obj.biochemistryUuid << "; mappingUuid: " << obj.mappingUuid;
	out << "; biomassList size: " << obj.biomassList.size() << "; modelCompartmentList size: " << obj.modelCompartmentList.size();
	out << "; modelCompoundList size: " << obj.modelCompoundList.size() << "; modelReactionList size: " << obj.modelReactionList.size();
	return out;
}
