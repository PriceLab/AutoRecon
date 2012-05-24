#include <iostream>
#include <vector>

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
