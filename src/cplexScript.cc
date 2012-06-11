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


bool Solver::initialize(STOICHMATRIX &stoich, vector<double> &objfn,
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



bool Solver::initialize(STOICHMATRIX &stoich, vector<double> &objfn,
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

void Solver::getRxnIDs(vector<int> & rxnIDs)
{
  rxnIDs.clear();
  char * temp;
  int id;
  for(int i=0; i<rxnsize; i++)
  {
    sscanf(rxns[i].getName(), "Rxn%d", &id);
    rxnIDs.push_back(id);
  }
}

void Solver::getRxnRates(vector<double> & rxnRates)
{
  rxnRates.clear();
  for(int i=0; i<rxnsize; i++)
  {
    rxnRates.push_back(cplex.getValue(rxns[i]));
  }
}

    






