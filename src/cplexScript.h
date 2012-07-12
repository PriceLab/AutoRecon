/*  cplexScript.h

    SOLVER - CPLEX WRAPPER
    IloInfinity = infinity in double format (seriously)
   -IloInfinity = negative infinity in double format

    If you include cplexScript.h in your header, you too can use these
    amazing infinity values. They are GREAT for bounds.
*/
#ifndef cplexScript_H
#define cplexScript_H


#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlreader.h>
#include <ilcplex/ilocplex.h>
#include <ilcplex/cplex.h>
#include <ilconcert/ilolinear.h>


#include "DataStructures.h"



ILOSTLBEGIN

using std::cout;
using std::map;
using std::vector;
using std::endl; 

// This class is a ghost of a discarded CPLEX solver method
class XMLinfo
{
public:
    double obVal;
    int size;
    double * values;
    string * varnames;

    XMLinfo(): obVal(0), size(0), values(NULL), varnames(NULL) {}
    XMLinfo(double ov, double * vals, string * names, int s);
    XMLinfo(XMLinfo const & orig);
    ~XMLinfo();

    XMLinfo& operator=(const XMLinfo & rhs);

    friend ostream& operator<<(ostream &out, XMLinfo & orig);
};


// CPLEX handler class
// NOTE: To start using the SOLVER, you must first declare an IloEnv, and pass
//       it to the constructor. When you are done using CPLEX and this solver,
//       you must end the IloEnv object with IloEnv::end().
//       Example:
//       IloEnv env;
//       SOLVER sol(env);
//       ...
//       env.end();
//       at this point, the solver and all CPLEX stuff should be freed up.
// 
// TODO: finish testing: addRxn-full- and addMet-full- functions
//       add functions:
class SOLVER
{
private:
  int metsize;
  int rxnsize;

  RXNSPACE rSpace;

  vector<RXNID> rxnIDs;
  vector<METID> metIDs;
  map<RXNID, int> rxnIdx;
  map<METID, int> metIdx;

public:
  IloModel model;
  IloCplex cplex;
  IloNumVarArray rxns;
  IloRangeArray mets;
  IloObjective obj;
  
  // Default constructor - still needs an IloEnv input 
  SOLVER(IloEnv & env) : model(env), rxns(env), mets(env), cplex(model),
                         metsize(0), rxnsize(0) {}

  // Other constructors
  SOLVER(IloEnv &env, STOICHMATRIX & stoich, vector<double> & objfn,
          char * objm, vector<int> & leg, vector<double> & rhs,
          vector<double> & lb,  vector<double> & ub ) : 
          model(env), rxns(env), mets(env), cplex(model)
          { settings();
            if(!initialize(stoich, objfn, objm, leg, rhs, lb, ub))
              cout<<"SOLVER initialization failed."<<endl; }

  SOLVER(IloEnv &env, STOICHMATRIX & stoich, vector<double> & objfn,
        char * objm, vector<double> & lb, vector<double> & ub ) :
        model(env), rxns(env), mets(env), cplex(model)
        { settings();
          if(!initialize(stoich, objfn, objm, lb, ub))
            cout<<"SOLVER initialization failed."<<endl; } 
  // Main constructor - Takes ANSWER, default objective reaction
  SOLVER(IloEnv &env, ANSWER &a, char * objm="Maximize") : model(env),
         rxns(env), mets(env), cplex(model), rSpace(a.reactions)
	 { settings() ; if(!initialize( RXNID(_db.BIOMASS), objm))
           cout<<"SOLVER initialization failed."<<endl; }
  // Main constructor, specific objective reaction - Takes ANSWER
  SOLVER(IloEnv &env, ANSWER &a, RXNID r, char * objm="Maximize") : 
         model(env), rxns(env), mets(env), cplex(model), rSpace(a.reactions)
	 { settings() ; if(!initialize(r, objm))
           cout<<"SOLVER initialization failed."<<endl; }
  // Main constructor - Takes PROBLEM, default objective reaction
  SOLVER(IloEnv &env, PROBLEM &p, char * objm="Maximize") : model(env),
         rxns(env), mets(env), cplex(model), rSpace(p.fullrxns)
	 { settings() ; if(!initialize(RXNID(_db.BIOMASS), objm))
           cout<<"SOLVER initialization failed."<<endl; }
  // Main constructor, specific objective reaction - Takes PROBLEM
  SOLVER(IloEnv &env, PROBLEM &p, RXNID r, char * objm="Maximize") : 
         model(env), rxns(env), mets(env), cplex(model), rSpace(p.fullrxns)
	 { settings() ; if(!initialize(r, objm))
           cout<<"SOLVER initialization failed."<<endl; }
  // Main constructor, takes RXNSPACE, specific objective reaction
  SOLVER(IloEnv &env, RXNSPACE &rspace, RXNID r, char * objm="Maximize") : 
         model(env), rxns(env), mets(env), cplex(model), rSpace(rspace)
	 { settings() ; if(!initialize(r, objm))
           cout<<"SOLVER initialization failed."<<endl; }

  bool initialize(STOICHMATRIX &stoich, vector<double> &objfn, char * objm,
                   vector<int> &leg, vector<double> &rhs, vector<double> &lb,
                   vector<double> &ub );
  bool initialize(STOICHMATRIX &stoich, vector<double> &objfn, char * objm,
                   vector<double> &lb, vector<double> &ub);
  // Main initialization function
  bool initialize(RXNID rid, char * objm);

  // SOLVER solve function - returns 0 if fail, i think, 1 if success
  // RUN BEFORE ACCESSING SOLUTION - duh.
  bool solve();
  // cplex settings - keep in header for organization purposes
  void settings() { cplex.setParam(IloCplex::PreInd, false); }

  // SOLVER access functions
  // get size function
  int metSize() const { return metsize; }
  int rxnSize() const { return rxnsize; }
  // get coefficient correlating to the input METID and RXNID
  double getCoef(METID mid, RXNID rid);
  // set coefficient correlating to the input METID and RXNID
  bool setCoef(METID mid, RXNID rid, double coef);
  // ID/index conversion functions - shouldn't need them.
  //   If you do need one of these conversion functions, use them, then email:
  //   colin.hoffman48@gmail.com - or try to fix it
  // Returns -1 if rxnID is not in SOLVER
  int rxnIDtoIDX(RXNID id);
  // Returns -1 if metID is not in SOLVER
  int metIDtoIDX(METID id);
  // Returns -1 if rxn index is not in SOLVER
  RXNID rxnIDXtoID(int idx) const;
  // Returns -1 if met index is not in SOLVER
  METID metIDXtoID(int idx) const;
  // Check for ID functions
  // Returns 1 if RXNID is in SOLVER, 0 otherwise
  bool hasRXNID(RXNID id);
  // Returns 1 if METID is in SOLVER, 0 otherwise
  bool hasMETID(METID id);
  // Returns Upper/Lower bound as double if successful, 0 is returned if error
  double getUB(RXNID id);
  double getLB(RXNID id);
  // Stores Upper/Lower Bounds of Reactions in map of RXNIDs and
  //    their correlating values.
  void getUB(map<RXNID, double> &ubMap);
  void getLB(map<RXNID, double> &lbMap);
  // Returns Upper/Lower bound of metabolite as double if successful,
  //    0.0 returned if error
  double getUB(METID id);
  double getLB(METID id);
  // Stores Upper/Lower Bounds of Metabolites in map of METIDs and
  //    their correlating values.
  void getUB(map<METID, double> &ubMap);
  void getLB(map<METID, double> &lbMap);
  // Returns objective coefficient by RXNID, 0 is returned if error
  double getObjCoef(RXNID id);
  // Stores the objective coefficients as a map of RXNID and doubles
  void getObjFn(map<RXNID, double> & objfn);
  // Returns reaction
  REACTION getRxn(RXNID id);

  // USE NEXT THREE FUNCTIONS ONLY AFTER SOLVING!
  // Returns the objective value
  double getObjVal() { return cplex.getObjValue(); }
  // Solution RXNIDs
  void getRxnIDs(vector<RXNID> & rxnIDs);
  // Solution values by RXNID
  void getRxnRates(vector<double> & rxnRates);
  // Provides both RXNIDs and solution values in a map format
  void getRxnValues(map<RXNID, double> & rxnVals);

  // Use to alter the input problem
  // REACTION BOUNDS SET
  // Change Bounds - set bounds to 0 to "remove" the reaction
  // changes both Upper and Lower bound
  bool setBounds(RXNID id, double lb, double ub);
  // changes Lower Bound
  bool setLB(RXNID id, double lb);
  // changes Lower Bounds by paired RXNIDs
  bool setLB(map<RXNID, double> &bounds);
  // changes Upper Bound
  bool setUB(RXNID id, double ub);
  // changes Uppoer Bounds by paired RXNIDs
  bool setUB(map<RXNID, double> &bounds);
  // METABOLITE BOUNDS SET
  // Change Bounds
  // changes both Upper and Lower bound
  bool setBounds(METID id, double lb=0.0, double ub=0.0);
  // changes Lower Bound
  bool setLB(METID id, double lb);
  // changes Lower Bounds by paired RXNIDs
  bool setLB(map<METID, double> &bounds);
  // changes Upper Bound
  bool setUB(METID id, double ub);
  // changes Uppoer Bounds by paired RXNIDs
  bool setUB(map<METID, double> &bounds);
  // Change Objective
  // Change objective by a map matching IDs to values. Any incorrect IDs
  //    will be skipped, and a 0 returned. 1 returned if successful.
  bool changeObjective(map<RXNID, double> & objvals);
  // Change objective by a vector. Vector must be as big as the set of rxns
  bool changeObjective(vector<double> & objvals);
  // Change objective by a single reaction. Takes a RXNID and a value
  bool changeObjective(RXNID rid, double value);
  // Clear old objective and replaces it with new objective
  //   singe RXNID - coefficient defaults to 1.0
  bool setObjective(RXNID id, double coeff=1.0);
  //   multiple RXNID
  bool setObjective(map<RXNID, double> &objset);
  // Add Reaction
  bool addRxn(REACTION &r); //needs test
  // adds stoich-empty reaction. RXN only has id, LB, UB, and name(def to ID).
  bool addEmptyRxn(RXNID id, double lb, double ub, char name[] = ""); 
                                                   //needs bounds test
  // Add Metabolite
  bool addEmptyMet(METID id, double lb=0, double ub=0); //needs bounds test
  // Adds metabolite to the SOLVER with corresponding reactions and values
  // Set lb and ub to 0.0 if FBA.
  // Can be used to set bounds for a set of reactions.
  // Must contain reactions already in the input problem. Will return 0
  // if reaction is not in problem or if metabolite is already in problem.
  bool addMet(METID id, vector<RXNID> &reacts, vector<double> &vals,
              double lb=0, double ub=0); //needs test

  // Can write a problem in all of the major formats. Not txt, though, I think
  void writeProblem(char * filename) { cplex.exportModel(filename); }
  // Writes the solution. End filename as .sol
  void writeSolution(char * filename) { cplex.writeSolution(filename); }

};
    

void processNode(xmlTextReaderPtr reader, XMLinfo & sol,
                   vector<string> & varname, vector<double> & val);

XMLinfo parseSol(const char * filename);

ostream& operator<<(ostream &out, XMLinfo & orig);

// This is the main solver
// Parameter: char* - the lp filename that is to be solved
// Output: XMLinfo - the data gathered from the CPLEX solution
XMLinfo cplexSolve(char * filename);

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
//int toCplexLP(STOICHMATRIX & stoich, vector<double> & objfn,
//              vector<int> & leg, vector<double> & legval, char * mm);


#endif // cplexScript_H


    
