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

// This class is the output for the whole CPLEX scripting program
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

class Solver
{
public:
    IloModel model;
    IloCplex cplex;
    IloNumVarArray rxns;
    IloRangeArray mets;
    IloObjective obj;
    int metsize;
    int rxnsize;
    
    Solver(IloEnv & env) : model(env), rxns(env), mets(env), cplex(model),
                           metsize(0), rxnsize(0) {}

    Solver(IloEnv &env, STOICHMATRIX & stoich, vector<double> & objfn,
            char * objm, vector<int> & leg, vector<double> & rhs,
            vector<double> & lb,  vector<double> & ub ) : 
            model(env), rxns(env), mets(env), cplex(model)
            { settings();
              if(!initialize(stoich, objfn, objm, leg, rhs, lb, ub))
                cout<<"Solver initialization failed."<<endl; }

    Solver(IloEnv &env, STOICHMATRIX & stoich, vector<double> & objfn,
            char * objm, vector<double> & lb, vector<double> & ub ) :
          model(env), rxns(env), mets(env), cplex(model)
          { settings();
            if(!initialize(stoich, objfn, objm, lb, ub))
              cout<<"Solver initialization failed."<<endl; } 

    bool initialize(STOICHMATRIX &stoich, vector<double> &objfn, char * objm,
                     vector<int> &leg, vector<double> &rhs, vector<double> &lb,
                     vector<double> &ub );
    bool initialize(STOICHMATRIX &stoich, vector<double> &objfn, char * objm,
                     vector<double> &lb, vector<double> &ub);

    bool solve() { return cplex.solve(); }
    void settings() { cplex.setParam(IloCplex::PreInd, false); }

    double getObjVal() { return cplex.getObjValue(); }
    void getRxnIDs(vector<int> & rxnIDs);
    void getRxnRates(vector<double> & rxnRates);

    void writeProblem(char * filename) { cplex.exportModel(filename); }
    void writeSolution(char * filename) { cplex.writeSolution(filename); }
//    changeobjective(vector<double> & objvals);

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


    
