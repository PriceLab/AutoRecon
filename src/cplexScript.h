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

using namespace std;

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

void processNode(xmlTextReaderPtr reader, XMLinfo & sol,
                   vector<string> & varname, vector<double> & val);

XMLinfo parseSol(const char * filename);

ostream& operator<<(ostream &out, XMLinfo & orig);

// This is the main solver
// Parameter: char* - the lp filename that is to be solved
// Output: XMLinfo - the data gathered from the CPLEX solution
XMLinfo cplexSolve(char * filename);

#endif // cplexScript_H


    
