#include <iostream>
#include <vector>
#include <map>
#include <cstring>

#include "cplexScript.h"

using namespace std;

// constructor
XMLinfo::XMLinfo(double ov, double * vals, string * names, int s)
{
    obVal = ov;
    values = new double[s];
    varnames = new string[s];
    size = s;
    for(int i=0; i<size; i++){
        values[i] = vals[i];
        varnames[i] = names[i];
    }
}

// copy constructor
XMLinfo::XMLinfo(XMLinfo const & orig){
    obVal = orig.obVal;
    size = orig.size;
    if(size > 0){
        values = new double[size];
        varnames = new string[size];
        for(int i=0; i<size; i++){
            values[i] = orig.values[i];
            varnames[i] = orig.varnames[i];
        }
    }
    else{
        values = NULL;
        varnames = NULL;
    }
}

// destructor
XMLinfo::~XMLinfo(){
    delete[] values;
    delete[] varnames;
    values = NULL;
    varnames = NULL;
}

// overloaded assignment operator
XMLinfo& XMLinfo::operator=(const XMLinfo & rhs){
  if (&rhs != this) {
    obVal = rhs.obVal;
    size = rhs.size;
    /* Why doesn't this fail if *this doesn't have anything in it yet? */
    // ^ NULLification ^ deleting a ptr set to NULL does nothing
    delete[] values;
    delete[] varnames;
    if(size > 0){
        values = new double[size];
        varnames = new string[size];    
        for(int i=0; i<size; i++){
            values[i] = rhs.values[i];
            varnames[i] = rhs.varnames[i];
        }
    }
    else{
        values = NULL;
        varnames = NULL;
    }
  } 

  return *this;
}

// gathers data from the XML node
void processNode(xmlTextReaderPtr reader, XMLinfo & sol,
                   vector<string> & varname, vector<double> & val)
{
    const xmlChar *name;

    name = xmlTextReaderConstName(reader);
    if(name==NULL)
        name = BAD_CAST "--";
    if(string(((char*)name)) == "header")
        sol.obVal = atof((char*)xmlTextReaderGetAttribute(reader,
                        xmlCharStrdup("objectiveValue")));
    if(string(((char*)name)) == "variable"){
        varname.push_back(string((char*)xmlTextReaderGetAttribute(reader,
                                                xmlCharStrdup("name"))));
        val.push_back(atof((char*)xmlTextReaderGetAttribute(reader,
                                                 xmlCharStrdup("value"))));
        sol.size++;
    }

}

// parses the xml into nodes to gather data
// reads in the output solution XML from CPLEX
// outputs the XMLinfo
XMLinfo parseSol(const char * filename)
{
    xmlTextReaderPtr reader;
    int ret;
    XMLinfo output;
    vector<double> vals;
    vector<string> varnames;
    

    reader = xmlReaderForFile(filename, NULL, 0);
    if(reader == NULL){
        fprintf(stderr, "Failed to parse %s\n", filename);
        return XMLinfo();
    }

    ret = xmlTextReaderRead(reader);
    while(ret == 1){
        processNode(reader, output, varnames, vals);
        ret = xmlTextReaderRead(reader);
    }
    xmlFreeTextReader(reader);
    if(ret != 0)
        fprintf(stderr, "%s :failed to parse\n", filename);

    xmlCleanupParser();
    xmlMemoryDump();

    if(output.size > 0){
        output.varnames = new string[output.size];
        output.values = new double[output.size];
        for(int i=0; i<output.size; i++){
            output.varnames[i] = varnames[i];
            output.values[i] = vals[i];
        }
    }

    return output;
}

// overloaded output operator for XMLinfo
ostream& operator<<(ostream &out, XMLinfo &orig)
{
    out<<"Objective Value: "<<orig.obVal<<endl;
    if(orig.size > 0){
        out<<"--Variables--"<<endl;
        for(int i=0; i<orig.size; i++)
            out<<orig.varnames[i]<<"\t"<<orig.values[i]<<endl;
    }
    return out;
}

void cleanupTemporaryCplexFiles() {
  system("rm cplex.log");
  system("rm clone1.log");
  system("rm cplexScript.sh");
  // I'm not entirely sure if we want to throw this away or not.
  //system("rm cplexSol.sol");
}
    
// This is the main solver
// Parameter: char* - the lp filename that is to be solved
// Output: XMLinfo - the data gathered from the CPLEX solution
XMLinfo cplexSolve(char * filename)
{
    ofstream scriptFILE;
    scriptFILE.open("cplexScript.sh");
    scriptFILE<<"read "<<filename<<" lp"<<endl
              <<"optimize"<<endl
              <<"write cplexSol.sol"<<endl
              <<"y"<<endl
              <<"quit"<<endl;
    scriptFILE.close();

    system("cplex < cplexScript.sh");

    XMLinfo temp;

    temp = parseSol("cplexSol.sol");
    cleanupTemporaryCplexFiles();

    return temp;
}

// toCplexLP function
// Creates an LP file for CPLEX to solve
// Parameters: stoich - a stoich matrix of mets x rxns
//             objfn  - a vector of the values for each rxn in the objective
//                      function (must be length of rxns)
//             leg    - a vector of {-1,0,1} for less, equal, greater than for
//                      the bounds for each metabolite (must be length of mets)
//             legval - a vector of the values for the bounds for each
//                      metabolite (must be length of mets)
//             mm     - a char string of {"Maximize","Minimize"} to maximize
//                      or minimize the objective function
// Returns: 1 for success, 0 for failure
/*int toCplexLP(STOICHMATRIX & stoich, vector<double> & objfn,
              vector<int> & leg, vector<double> & legval, char * mm)
{
  if(stoich.sizeX <= 0 || stoich.sizeY <= 0){
    cout<<"stoich matrix is empty or incompatible"<<endl;
    return 0;
  }
  if(strcmp(mm, "Minimize") && strcmp(mm, "Maximize")){
    cout<<"Must designate Maximize or Minimize for linear problem"<<endl;
    return 0;
  }
  if(objfn.size() != stoich.sizeY){
    cout<<"objfn length must match stoich.reactions vector length"<<endl;
    return 0;
  }
  if(leg.size() != stoich.sizeX || legval.size() != stoich.sizeX){
    cout<<"leg and legval length must match stoich.metabolites vector length"
        <<endl;
    return 0;
  }
  ofstream lpFILE;
  lpFILE.open("cplexProblem.lp");
  lpFILE<<mm<<endl
        <<"\tvalue: ";
  for(int i=0; i<objfn.size(); i++){
    if(i > 0){
      if( objfn[i] >= 0)
        lpFILE<<" + ";
      else
        lpFILE<<" ";
    }
    lpFILE<<objfn[i]<<" Rxn"<<i;
  }
  lpFILE<<endl<<endl<<"Subject To"<<endl;
  for(int i=0; i<stoich.sizeX; i++){
    lpFILE<<"\tMet"<<i<<": ";
    for(it=stoich[i].begin(); it!=stoich[i].end(); it++){
      if(it!=stoich[i].begin()){
        if((it->second) >= 0)
          lpFILE<<" + ";
        else
          lpFILE<<" ";
      }
      lpFILE<<(it->second)<<" Rxn"<<(it->first);
    }
    lpFILE<<endl;
  }
  lpFILE<<endl<<"Bounds"<<endl;
  for(int i=0; i<stoich.reactions.rxns.size(); i++)
    lpFILE<<"\t"<<stoich.reactions.rxns[i].lb<<" <= "
          <<"Rxn"<<i<<" <= "<<stoich.reactions.rxns[i].ub<<endl;
  lpFILE<<endl<<"End";
  return 1;
}
  
  */


bool SOLVER::initialize(STOICHMATRIX &stoich, vector<double> &objfn,
                 char * objm, vector<int> &leg, vector<double> &rhs,
                 vector<double> &lb, vector<double> &ub)
{
  // check that the input sizes are consistent
  if(stoich.sizeY != objfn.size())
  {
    cout<<"size of objective function vector does not match stoich size"<<endl;
    cout<<"obj fn size: "<<objfn.size()<<"\trequired size: "<<stoich.sizeY
        <<endl;
    return 0;
  }
  if(stoich.sizeX != leg.size() )
  {
    cout<<"size of LEG vector does not match stoich size"<<endl;
    cout<<"LEG size: "<<leg.size()<<"\trequired size: "<<stoich.sizeX
        <<endl;
    return 0;
  }
  if(stoich.sizeX != rhs.size() )
  {
    cout<<"size of RHS vector does not match stoich size"<<endl;
    cout<<"rhs size: "<<rhs.size()<<"\trequired size: "<<stoich.sizeX
        <<endl;
    return 0;
  }
  if(stoich.sizeY != lb.size())
  {
    cout<<"size of lower bound vector does not match stoich size"<<endl;
    cout<<"lb size: "<<lb.size()<<"\trequired size: "<<stoich.sizeY
        <<endl;
    return 0;
  }
  if(stoich.sizeY != ub.size())
  {
    cout<<"size of upper bound vector does not match stoich size"<<endl;
    cout<<"ub size: "<<ub.size()<<"\trequired size: "<<stoich.sizeY
        <<endl;
    return 0;
  }

  // grab environment from model
  IloEnv env = model.getEnv();

  // check for max, min, or error from objm
  if(!strcmp(objm, "Maximize") || !strcmp(objm, "max") ||
     !strcmp(objm, "maximize") )
  {
    obj = IloMaximize(env);
  }
  else if(!strcmp(objm,"Minimize") || !strcmp(objm,"min") ||
          !strcmp(objm, "minimize")  )
  {
    obj = IloMinimize(env);
  }
  else{
    cout<<"did not specify maximize or minimize"<<endl;
    return 0;
  }
  
  // store size of problem
  metsize = stoich.sizeX;
  rxnsize = stoich.sizeY;
  char temp[24];
  
  // set up metabolites and right hand size metabolite value ranges
  for(int i=0; i<metsize; i++)
  {
    sprintf(temp, "Met%d", int(stoich.xIDs[i]));
    if(leg[i] > 0)
    {
      if(leg[i] > 1)
        cout<<"minor error: leg value ["<<i<<"] greater than 1"<<endl;
      mets.add(IloRange(env, -IloInfinity, rhs[i], temp) );
    }
    else if(leg[i] < 0)
    {
      if(leg[i] < -1)
        cout<<"minor error: leg value ["<<i<<"] less than -1"<<endl;
      mets.add(IloRange(env, rhs[i], IloInfinity, temp) );
    }
    else
    {
      mets.add(IloRange(env, rhs[i], rhs[i], temp) );
    }
  }
  // set up reactions and ranges
  for(int i=0; i<rxnsize; i++)
  {
    sprintf(temp, "Rxn%d", int(stoich.yIDs[i])); 
    rxns.add(IloNumVar(env, lb[i], ub[i], temp) );
  }

  // set up objective function with reactions
  for(int i=0; i<rxnsize; i++)
  {
    obj.setLinearCoef(rxns[i], objfn[i]);
  }

  // set up lp matrix with stoich matrix values
  map<int, double>::iterator it;
  for(int i=0; i<metsize; i++)
  {
    for(it = stoich[i].begin(); it != stoich[i].end(); it++)
    {
     mets[i].setLinearCoef(rxns[it->first], it->second);
    }
  }

  model.add(obj);
  model.add(mets);

  return 1; // SUCCESS!
}



bool SOLVER::initialize(STOICHMATRIX &stoich, vector<double> &objfn,
                 char * objm, vector<double> &lb, vector<double> &ub)
{
  // check that the input sizes are consistent
  if(stoich.sizeY != objfn.size())
  {
    cout<<"size of objective function vector does not match stoich size"<<endl;
    cout<<"obj fn size: "<<objfn.size()<<"\trequired size: "<<stoich.sizeY
        <<endl;
    return 0;
  }
  if(stoich.sizeY != lb.size())
  {
    cout<<"size of lower bound vector does not match stoich size"<<endl;
    cout<<"lb size: "<<lb.size()<<"\trequired size: "<<stoich.sizeY
        <<endl;
    return 0;
  }
  if(stoich.sizeY != ub.size())
  {
    cout<<"size of upper bound vector does not match stoich size"<<endl;
    cout<<"ub size: "<<ub.size()<<"\trequired size: "<<stoich.sizeY
        <<endl;
    return 0;
  }

  // grab environment from model
  IloEnv env = model.getEnv();

  // check for max, min, or error from objm
  if(!strcmp(objm, "Maximize") || !strcmp(objm, "max") ||
     !strcmp(objm, "maximize") )
  {
    obj = IloMaximize(env);
  }
  else if(!strcmp(objm,"Minimize") || !strcmp(objm,"min") ||
          !strcmp(objm, "minimize")  )
  {
    obj = IloMinimize(env);
  }
  else{
    cout<<"did not specify maximize or minimize"<<endl;
    return 0;
  }
  
  // store size of problem
  metsize = stoich.sizeX;
  rxnsize = stoich.sizeY;
  char temp[24];
  
  // set up metabolites and right hand size metabolite value ranges
  for(int i=0; i<metsize; i++)
  {
    sprintf(temp, "Met%d", int(stoich.xIDs[i]));
      mets.add(IloRange(env, 0, 0, temp) );
  }
  // set up reactions and ranges
  for(int i=0; i<rxnsize; i++)
  {
    sprintf(temp, "Rxn%d", int(stoich.yIDs[i])); 
    rxns.add(IloNumVar(env, lb[i], ub[i], temp) );
  }

  // set up objective function with reactions
  for(int i=0; i<rxnsize; i++)
  {
    obj.setLinearCoef(rxns[i], objfn[i]);
  }

  // set up lp matrix with stoich matrix values
  map<int, double>::iterator it;
  for(int i=0; i<metsize; i++)
  {
    for(it = stoich[i].begin(); it != stoich[i].end(); it++)
    {
     mets[i].setLinearCoef(rxns[it->first], it->second);
    }
  }

  model.add(obj);
  model.add(mets);

  return 1; // SUCCESS!
}


bool SOLVER::initialize(RXNSPACE &rSpace, RXNID rid, char * objm)
{
  // check that the input sizes are consistent
  if(!rSpace.idIn(rid))
  {
    cout<<"Reaction ID not in RXNSPACE"<<endl;
    cout<<"ID: "<<int(rid)<<endl;
    return 0;
  }

  // grab environment from model
  IloEnv env = model.getEnv();

  // check for max, min, or error from objm
  if(!strcmp(objm, "Maximize") || !strcmp(objm, "max") ||
     !strcmp(objm, "maximize") )
  {
    obj = IloMaximize(env);
  }
  else if(!strcmp(objm,"Minimize") || !strcmp(objm,"min") ||
          !strcmp(objm, "minimize")  )
  {
    obj = IloMinimize(env);
  }
  else{
    cout<<"did not specify maximize or minimize"<<endl;
    return 0;
  }
  
  rxnsize = rSpace.rxns.size();
  char temp[24];
  
  int rxncount=0;
  metsize=0;
  // set up reactions and ranges
  for(vector<REACTION>::iterator it=rSpace.rxns.begin(); rxncount<rxnsize; it++)
  {
    sprintf(temp, "Rxn%d", int(it->id)); 
    rxns.add(IloNumVar(env, it->lb, it->ub, temp) );
    rxnIDs.push_back(it->id);
    // set up metabolites and coefficients
    for(vector<STOICH>::iterator it2=it->stoich.begin();
        it2 != it->stoich.end(); it2++)
    {
      // check if met ID is new, if so, add to mets
      if(!metIdx.count(it2->met_id))
      {
        sprintf(temp, "Met%d", int(it2->met_id));
        mets.add(IloRange(env, 0, 0, temp) );
        metIDs.push_back(it2->met_id);
        metIdx[it2->met_id] = metsize++;
      }
      mets[metIdx[it2->met_id] ].setLinearCoef(rxns[rxncount], it2->rxn_coeff);
    }
    rxnIdx[it->id] = rxncount++;
  }

  // set up objective function with reaction
  obj.setLinearCoef(rxns[rxnIdx[rid] ], 1.0);

  model.add(obj);
  model.add(mets);

  return 1; // SUCCESS!
}

// solve - SOLVER
// solves the constructed LP, returns 1 if success, 0 if fail
bool SOLVER::solve()
{
  return cplex.solve();
}

// getCoef - SOLVER
// returns the coefficient corresponding to METID and RXNID
double SOLVER::getCoef(METID mid, RXNID rid)
{
  if(!rxnIdx.count(rid)){
    cout<<"RXNID: "<<int(rid)<<" not inSOLVER"<<endl;
    return 0.0;
  }
  if(!metIdx.count(mid)){
    cout<<"METID: "<<int(mid)<<" not in SOLVER"<<endl;
    return 0.0;
  }

  int ridx = rxnIdx[rid];
  int midx = metIdx[mid];
  IloExpr e = mets[midx].getExpr();
  for(IloExpr::LinearIterator it = e.getLinearIterator(); it.ok(); ++it)
  {
    if((it.getVar()).getName() == rxns[ridx].getName())
      return it.getCoef();
  }
  return 0.0;
}

// setCoef - SOLVER
// sets the coefficient corresponding to METID(mid) and RXNID(rid) to coef
// returns 0 if error, 1 if success
bool SOLVER::setCoef(METID mid, RXNID rid, double coef)
{
  if(!rxnIdx.count(rid)){
    cout<<"RXNID: "<<int(rid)<<" not in SOLVER"<<endl;
    return 0;
  }
  if(!metIdx.count(mid)){
    cout<<"METID: "<<int(mid)<<" not in SOLVER"<<endl;
    return 0;
  }

  mets[ metIdx[mid] ].setLinearCoef(rxns[ rxnIdx[rid] ], coef);

  return 1;
}

// rxnIDtoIDX - SOLVER
// Returns corresponding index if ID is in SOLVER, -1 otherwise
int SOLVER::rxnIDtoIDX(RXNID id)
{
  return ((rxnIdx.count(id)) ? (rxnIdx[id]) : -1);
}

// metIDtoIDX - SOLVER
// Returns corresponding index if ID is in SOLVER, -1 otherwise
int SOLVER::metIDtoIDX(METID id)
{
  return (metIdx.count(id) ? metIdx[id] : -1);
}

// rxnIDXtoID - SOLVER
// Returns corresponding RXNID if index is within bounds, -1 otherwise
RXNID SOLVER::rxnIDXtoID(int idx) const
{
  return ((idx>-1 && idx<rxnsize) ? rxnIDs[idx] : RXNID(-1));
}

// metIDXtoID - SOLVER
// Returns corresponding METID if index is within bounds, -1 otherwise
METID SOLVER::metIDXtoID(int idx) const
{
  return ((idx>-1 && idx<metsize) ? metIDs[idx] : METID(-1));
}

// hasRXNID - SOLVER
// Returns 1 if RXNID is in SOLVER, 0 otherwise
bool SOLVER::hasRXNID(RXNID id)
{
  return rxnIdx.count(id);
}

// hasMETID - SOLVER
// Returns 1 if METID is in SOLVER, 0 otherwise
bool SOLVER::hasMETID(METID id)
{
  return metIdx.count(id);
}

// getUB - SOLVER
// Returns Upper Bound of Reaction RXNID, if error: returns 0.0
double SOLVER::getUB(RXNID id)
{
  if(!rxnIdx.count(id) ){
    cout<<"RXN: "<<int(id)<<" not in SOLVER"<<endl;
    return 0.0;
  }
  return rxns[ rxnIdx[id] ].getUB();
}

// getLB - SOLVER
// Returns Lower Bound of Reaction RXNID, if error: returns 0.0
double SOLVER::getLB(RXNID id)
{
  if(!rxnIdx.count(id) ){
    cout<<"RXN: "<<int(id)<<" not in SOLVER"<<endl;
    return 0.0;
  }
  return rxns[ rxnIdx[id] ].getLB();
}

// getUB - SOLVER - mass get
// Stores Upper Bounds for reactions in map of RXNID and corresponding values
void SOLVER::getUB(map<RXNID, double> &ubMap)
{
  ubMap.clear();
  for(int i=0; i<rxnIDs.size(); i++)
  {
    ubMap[ rxnIDs[i] ] = rxns[i].getUB();
  }
  return;
}

// getLB - SOLVER - mass get
// Stores Lower Bounds for reactions in map of RXNID and corresponding values
void SOLVER::getLB(map<RXNID, double> &lbMap)
{
  lbMap.clear();
  for(int i=0; i<rxnIDs.size(); i++)
  {
    lbMap[ rxnIDs[i] ] = rxns[i].getLB();
  }
  return;
}


// getUB - SOLVER
// Returns Upper Bound of Metabolite METID, if error: returns 0.0
double SOLVER::getUB(METID id)
{
  if(!metIdx.count(id) ){
    cout<<"MET: "<<int(id)<<" not in SOLVER"<<endl;
    return 0.0;
  }
  return mets[ metIdx[id] ].getUB();
}

// getLB -SOLVER
// Returns Lower Bound of Metabolite METID, if error: returns 0.0
double SOLVER::getLB(METID id)
{
  if(!metIdx.count(id) ){
    cout<<"MET: "<<int(id)<<" not in SOLVER"<<endl;
    return 0.0;
  }
  return mets[ metIdx[id] ].getLB();
}

// getUB - SOLVER - mass get
// Stores Upper Bounds for metabolites in map of METID and corresponding values
void SOLVER::getUB(map<METID, double> &ubMap)
{
  ubMap.clear();
  for(int i=0; i<metIDs.size(); i++)
  {
    ubMap[ metIDs[i] ] = mets[i].getUB();
  }
  return;
}

// getLB - SOLVER - mass get
// Stores Lower Bounds for metabolites in map of METID and corresponding values
void SOLVER::getLB(map<METID, double> &lbMap)
{
  lbMap.clear();
  for(int i=0; i<metIDs.size(); i++)
  {
    lbMap[ metIDs[i] ] = mets[i].getLB();
  }
  return;
}

// getObjCoef - SOLVER
// Returns objective coefficient by RXNID, 0.0 is returned if error
double SOLVER::getObjCoef(RXNID id)
{
  if(!rxnIdx.count(id)){
    cout<<"RXNID: "<<int(id)<<" not in SOLVER"<<endl;
    return 0.0;
  }
  
  IloExpr e = obj.getExpr();
  for(IloExpr::LinearIterator it = e.getLinearIterator(); it.ok(); ++it)
  {
    if((it.getVar()).getName() == rxns[ rxnIdx[id] ].getName() )
      return it.getCoef();
  }
  return 0.0;
}

// getObjFn - SOLVER
// Stores the objective coefficients as a map of RXNID and doubles
// Must pass the map to fill as the input parameter.
void SOLVER::getObjFn(map<RXNID, double> & rxnVals)
{
  rxnVals.clear();
  int id;
  IloExpr e = obj.getExpr();
  for(IloExpr::LinearIterator it = e.getLinearIterator(); it.ok(); ++it)
  {
    if( sscanf((it.getVar()).getName(), "Rxn%d", &id) )
    {
      rxnVals[RXNID(id)] = it.getCoef();
    }
  }
}

// getRxnIDs - SOLVER
// Input vector will be filled with the RXNIDs corresponding to solution values
void SOLVER::getRxnIDs(vector<RXNID> & rxnIDs)
{
  rxnIDs.clear();
  int id;
  for(int i=0; i<rxnsize; i++)
  {
    sscanf(rxns[i].getName(), "Rxn%d", &id);
    rxnIDs.push_back( RXNID(id) );
  }
}

// getRxnRates - SOLVER
// Input vector will be filled with reaction values from the solution
void SOLVER::getRxnRates(vector<double> & rxnRates)
{
  rxnRates.clear();
  for(int i=0; i<rxnsize; i++)
  {
    rxnRates.push_back(cplex.getValue(rxns[i]));
  }
}

// getRxnValues - SOLVER
// Input map will be filled with RXNIDs and corresponding solution values
void SOLVER::getRxnValues(map<RXNID, double> & rxnVals)
{
  rxnVals.clear();
  int id;
  for(int i=0; i<rxnsize; i++)
  {
    if( sscanf(rxns[i].getName(), "Rxn%d", &id) )
    {
      rxnVals[RXNID(id)] = cplex.getValue(rxns[i]);
    }
  }
  return;
}

// setBounds - SOLVER
// sets both upper and lower bounds for reaction RXNID
// Returns 1 if successful, 0 if error
bool SOLVER::setBounds(RXNID id, double lb, double ub)
{
  if(!rxnIdx.count(id)){
    cout<<"changeBounds error: Reaction "<<id<<" not in SOLVER"<<endl;
    return 0;
  }
  rxns[rxnIdx[id] ].setBounds(lb, ub);
  return 1;  
}

// setLB - SOLVER
// sets lower bound of reaction RXNID to lb
// Returns 1 if successful, 0 if error
bool SOLVER::setLB(RXNID id, double lb)
{
  if(!rxnIdx.count(id)){
    cout<<"changeLB error: Reaction "<<id<<" not in SOLVER"<<endl;
    return 0;
  }
  rxns[rxnIdx[id] ].setLB(lb);
  return 1;  
}

// setLB - SOLVER - mass set
// sets both lower bound for reactions in map of RXNIDs and values
// Returns 1 if successful, 0 if error. Skips invalid reactions
bool SOLVER::setLB(map<RXNID, double> &bounds)
{
  bool success = true;
  map<RXNID, double>::iterator it=bounds.begin();
  for(; it != bounds.end(); ++it)
  {
    if(rxnIdx.count(it->first))
    {
      rxns[ rxnIdx[it->first] ].setLB(it->second);
    }
    else
    {
      cout<<"changeBounds error: Reaction "<<it->first<<" not in SOLVER"<<endl;
      success = false;
    }
  }
  return success;  
}

// setUB - SOLVER
// sets upper bound for reaction RXNID
// Returns 1 if successful, 0 if error
bool SOLVER::setUB(RXNID id, double ub)
{
  if(!rxnIdx.count(id)){
    cout<<"changeUB error: Reaction "<<id<<" not in SOLVER"<<endl;
    return 0;
  }
  rxns[rxnIdx[id] ].setUB(ub);
  return 1;  
}

// setUB - SOLVER - mass set
// sets both upper bound for reactions in map of RXNIDs and values
// Returns 1 if successful, 0 if error. Skips invalid reactions
bool SOLVER::setUB(map<RXNID, double> &bounds)
{
  bool success = true;
  map<RXNID, double>::iterator it=bounds.begin();
  for(; it != bounds.end(); ++it)
  {
    if(rxnIdx.count(it->first))
    {
      rxns[ rxnIdx[it->first] ].setUB(it->second);
    }
    else
    {
      cout<<"changeBounds error: Reaction "<<it->first<<" not in SOLVER"<<endl;
      success = false;
    }
  }
  return success;  
}

// setBounds - SOLVER
// sets both upper and lower bounds for metabolite METID
// Returns 1 if successful, 0 if error
bool SOLVER::setBounds(METID id, double lb, double ub)
{
  if(!metIdx.count(id)){
    cout<<"changeBounds error: Metabolite "<<id<<" not in SOLVER"<<endl;
    return 0;
  }
  mets[ metIdx[id] ].setBounds(lb, ub);
  return 1;  
}

// setLB - SOLVER
// sets lower bound of metabolite METID to lb
// Returns 1 if successful, 0 if error
bool SOLVER::setLB(METID id, double lb)
{
  if(!metIdx.count(id)){
    cout<<"changeLB error: Metabolite "<<id<<" not in SOLVER"<<endl;
    return 0;
  }
  mets[metIdx[id] ].setLB(lb);
  return 1;  
}

// setLB - SOLVER - mass set
// sets lower bound for metabolites in map of METIDs and values
// Returns 1 if successful, 0 if error. Skips invalid reactions
bool SOLVER::setLB(map<METID, double> &bounds)
{
  bool success = true;
  map<METID, double>::iterator it=bounds.begin();
  for(; it != bounds.end(); ++it)
  {
    if(metIdx.count(it->first))
    {
      mets[ metIdx[it->first] ].setLB(it->second);
    }
    else
    {
      cout<<"changeLB error: Metabolite "<<it->first<<" not in SOLVER"<<endl;
      success = false;
    }
  }
  return success;  
}

// setUB - SOLVER
// sets upper bound of metabolite METID to lb
// Returns 1 if successful, 0 if error
bool SOLVER::setUB(METID id, double ub)
{
  if(!metIdx.count(id)){
    cout<<"changeLB error: Metabolite "<<id<<" not in SOLVER"<<endl;
    return 0;
  }
  mets[metIdx[id] ].setUB(ub);
  return 1;  
}

// setUB - SOLVER - mass set
// sets upper bound for metabolites in map of METIDs and values
// Returns 1 if successful, 0 if error. Skips invalid reactions
bool SOLVER::setUB(map<METID, double> &bounds)
{
  bool success = true;
  map<METID, double>::iterator it=bounds.begin();
  for(; it != bounds.end(); ++it)
  {
    if(metIdx.count(it->first))
    {
      mets[ metIdx[it->first] ].setUB(it->second);
    }
    else
    {
      cout<<"changeLB error: Metabolite "<<it->first<<" not in SOLVER"<<endl;
      success = false;
    }
  }
  return success;  
}

// changeObjective - SOLVER
// Changes the objective function by map of RXNIDs and corresponding values
// Paramteter: objvals - map of reaction IDs and coefficients for each reaction
// Returns 1 if successful, 0 if error
bool SOLVER::changeObjective(map<RXNID, double> & objvals)
{
  bool success = true;
  map<RXNID, double>::iterator it = objvals.begin();
  for(; it != objvals.end(); ++it)
  {
    if(rxnIdx.count(it->first) ){
      obj.setLinearCoef(rxns[ rxnIdx[it->first] ], it->second);
    }
    else{
      cout<<"RXNID: "<<int(it->first)<<" not in SOLVER"<<endl;
      success = false;
    }
  }
  return success;
}

// changeObjective - SOLVER
// Changes the objective function by all reactions
// Paramteter: objvals - vector of coefficients for each reaction
// Returns 1 if successful, 0 if error
bool SOLVER::changeObjective(vector<double> & objvals)
{
  if(objvals.size() != rxnsize){
    cout<<"objective vector does not match size of reaction vector"<<endl;
    return 0;
  }
  for(int i=0; i<rxnsize; i++)
  {
    obj.setLinearCoef(rxns[i], objvals[i]);
  }
  return 1;
}

// changeObjective - SOLVER
// Changes the objective function by a single reaction
// Parameters: rid - Reaction ID, value - coefficient in front of the reaction
// Returns: 1 if successful, 0 if error
bool SOLVER::changeObjective(RXNID rid, double value)
{
  // check if reaction is within range
  if(!rxnIdx.count(rid) ) {
    cout<<"changeObjective error: Reaction is not within bounds of lp"<<endl;
    return 0;
  }
  obj.setLinearCoef(rxns[ rxnIdx[rid] ], value);
  return 1;
}

bool SOLVER::addRxn(REACTION &r)
{
  // checks if reaction is new
  if(rxnIdx.count(r.id)){
    cout<<"Reaction already in SOLVER"<<endl;
    return 0;
  }
  // sets up reaction
  char temp[24];
  IloEnv env = model.getEnv();
  sprintf(temp, "Rxn%d", int(r.id));
  rxns.add(IloNumVar(env, r.lb, r.ub, temp) );
  rxnIDs.push_back(r.id);
  // fills in metabolite/reaction coefficients
  for(vector<STOICH>::iterator it=r.stoich.begin(); it!=r.stoich.end(); it++)
  {
    // creates new metabolite if metabolite is new
    if(!metIdx.count(it->met_id))
    {
      sprintf(temp, "Met%d", int(it->met_id) );
      mets.add(IloRange(env, 0, 0, temp) );
      metIDs.push_back(it->met_id);
      metIdx[it->met_id] = metsize++;
    }
    mets[metIdx[it->met_id] ].setLinearCoef(rxns[rxnsize], it->rxn_coeff);
  }
  // finishes up setting up reaction
  rxnIdx[r.id] = rxnsize++; // increments rxnsize

  return 1; // SUCCESS
}

bool SOLVER::addEmptyRxn(RXNID id, double lb, double ub)
{
  // checks if reaction is new
  if(rxnIdx.count(id) ){
    cout<<"Reaction already in SOLVER"<<endl;
    return 0;
  }
  // sets up reaction
  char temp[24];
  IloEnv env = model.getEnv();
  sprintf(temp, "Rxn%d", int(id) );
  rxns.add(IloNumVar(env, lb, ub, temp));
  rxnIDs.push_back(id);
  rxnIdx[id] = rxnsize++; // increments rxnsize
  return 1; // SUCCESS
}

// addEmptyMet - SOLVER
// Adds a metabolite with no reactions to the SOLVER
// Returns 1 if successful, 0 if error
bool SOLVER::addEmptyMet(METID id, double lb, double ub)
{
  // check if metabolite is new
  if(metIdx.count(id) ){
    cout<<"addEmptyMet error: Metabolite already in SOLVER"<<endl;
    return 0;
  }
  // seting up metabolite
  char temp[24];
  IloEnv env = model.getEnv();
  sprintf(temp, "Met%d", int(id) );
  mets.add(IloRange(env, lb, ub, temp) );
  metIDs.push_back(id);
  metIdx[id] = metsize++; // increments metsize
  return 1; // SUCCESS
}

// addMet - SOLVER
// Adds metabolite to the SOLVER with corresponding reactions and values
// Set lb and ub to 0.0 if FBA.
// Can be used to set bounds for a set of reactions.  
// Must contain reactions already in the input problem. Will return 0
//   if reaction is not in problem or if metabolite is already in problem.
//   Returns 1 if successful.
bool SOLVER::addMet(METID id, vector<RXNID> &reacts, vector<double> &vals,
            double lb, double ub)
{
  // ERROR CHECKERS
  // check if metabolite is new
  if(metIdx.count(id) ){
    cout<<"addMet error: Metabolite already in SOLVER"<<endl;
    return 0;
  }
  // check if vector sizes match
  if(reacts.size() != vals.size()){
    cout<<"addMet error: Reaction vector and coefficient vector sizes did not "
        <<"match."<<endl;
    return 0;
  }
  // checks if reactions in vector are in the solver
  for(int i=0; i<reacts.size(); i++){
    if(!rxnIdx.count(reacts[i]) ) {
      cout<<"addMet error: Reaction ID: "<<reacts[i]<<" not in SOLVER"<<endl;
      return 0;
    }
  }
  
  // set up new metabolite
  char temp[24];
  IloEnv env = model.getEnv();
  sprintf(temp, "Met%d", int(id) );
  mets.add(IloRange(env, lb, ub, temp) );
  metIDs.push_back(id);
  
  // fill with reactions and correlating coefficients
  for(int i=0; i<reacts.size(); i++)
  {
    mets[metsize].setLinearCoef(rxns[rxnIdx[reacts[i] ] ], vals[i]);
  }
  
  // finish setting up metabolite, and increment metsize
  metIdx[id] = metsize++;
  
  return 1; // SUCCESS
}






