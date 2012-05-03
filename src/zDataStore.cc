#include "DataStructures.h"
#include "ETC.h"
#include "Grow.h"
#include "MyConstants.h"
#include "pathUtils.h"
#include "Paths2Model.h"
#include "Printers.h"
#include "shortestPath.h"
#include "RunK.h"
#include "visual01.h"
#include "XML_loader.h"
#include "Annotations.h"
#include "mysql_table.h"
#include "mysql_link_table.h"

#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

using std::vector;
using std::map;
using std::string;

#include "mysql.h"
void init_mysql();
void insert_mets(const PROBLEM& ProblemSpace);
void insert_rxns(const PROBLEM& ProblemSpace);
void insert_stoich(const PROBLEM& ProblemSpace);
void insert_paths(const PROBLEM& ProblemSpace);

int main(int argc, char *argv[]) {
  printf("\n\n");

  init_mysql();

  /* Main problem structure containing all the metabolites and reactions
     (in fullrxns), their synonym versions (synrxns), and a reversed synonym version
     useful for finding paths despite reversibility bottlenecks (synrxnsR) 
  
     All three sets of reactions share the same metabolites (metabolites) */
  PROBLEM ProblemSpace;

  /* Number of K-shortest paths (forward if possible, backward otherwise) */
  int K = _db.INITIAL_K;

  /* psum[i][j][k]: Growth condition i, output j, k'th shortest path */
  vector<vector<vector<PATHSUMMARY> > > psum;
  
  printf("Setting up problem inputs...\n");fflush(stdout);
  InputSetup(argc,argv,ProblemSpace);
  printf("...done \n");fflush(stdout);

  //  insert_mets(ProblemSpace);
  insert_rxns(ProblemSpace);
  //  insert_paths(ProblemSpace);
  printf("No seg faults!\n");
  return 0;

}

void init_mysql() {
  const char* server="localhost";
  const char* user="root";
  const char* password="";
  const char* database="autorecon";


  MYSQL* gConnection=MysqlTable::local_connect(server, user, password, database);
  if (!gConnection) {
    cout << "Database connection error" << endl;
    exit(1);
  }
  MysqlTable::_init_class(gConnection);
  METABOLITE::_init_class();
  REACTION::_init_class();
  STOICH::_init_class();

  
  MysqlLinkTable* srTable=new MysqlLinkTable(STOICH::_table, REACTION::_table);
  srTable->init();
  if (!srTable->exists()) {
    cerr << "creating srTable" << endl;
    srTable->create();
  } else {
    cerr << "sr link table already exists" << endl;
  }
}

void insert_mets(const PROBLEM& ProblemSpace) {
  string delete_metabolites="DELETE FROM " + METABOLITE::_table->tablename;
  METABOLITE::_table->do_sql(delete_metabolites.c_str());

  vector<METABOLITE> mets=ProblemSpace.metabolites.mets;
  vector<METABOLITE>::iterator met_it=mets.begin();
  cerr << mets.size() << " regular metabolites" << endl;
  while (met_it!=mets.end()) {
    METABOLITE m=*met_it++;
    cerr << m.dump() << endl;
    m.insert();
  }

  mets=ProblemSpace.secondaryLones.mets;
  met_it=mets.begin();
  cerr << mets.size() << " secondary lone metabolites" << endl;
  while (met_it!=mets.end()) {
    METABOLITE m=*met_it++;
    cerr << m.dump() << endl;
  }

  mets=ProblemSpace.cofactors.mets;
  met_it=mets.begin();
  cerr << mets.size() << " cofactor metabolites" << endl;
  while (met_it!=mets.end()) {
    METABOLITE m=*met_it++;
    cerr << m.dump() << endl;
  }
}  
  
void insert_rxns(const PROBLEM& ProblemSpace) {
  string delete_reactions="DELETE FROM " + REACTION::_table->tablename;
  REACTION::_table->do_sql(delete_reactions.c_str());

  string delete_stoichs="DELETE FROM " + STOICH::_table->tablename;
  STOICH::_table->do_sql(delete_stoichs.c_str());

  MysqlLinkTable* srTable=REACTION::_table->links[STOICH::_table->tablename];
  cerr << "got srTable " << srTable->tablename << endl;

  // fullrxns:
  vector<REACTION> rxns=ProblemSpace.fullrxns.rxns;
  vector<REACTION>::iterator rxn_it=rxns.begin();
  cerr << rxns.size() << " full reactions" << endl;
  while (rxn_it!=rxns.end()) {
    REACTION rxn=*rxn_it++;
    rxn.insert();
    cerr << rxn.dump() << endl;

    // Insert stoich vector to reaction-stoich link table:
    vector<STOICH>::iterator it=rxn.stoich.begin();
    while (it != rxn.stoich.end()) {
      STOICH s=*it++;
      s.insert();
      cerr << "attempting to insert to stoich-reaction: rxn.id=" << rxn.id << 
	", stoich=" << s.dump() << endl;
      srTable->insert(rxn.id, s.id);
    }
  }
 
  // synrxns:
  rxns=ProblemSpace.synrxns.rxns;
  rxn_it=rxns.begin();
  cerr << rxns.size() << " syn reactions" << endl;
  while (rxn_it!=rxns.end()) {
    REACTION rxn=*rxn_it++;
    rxn.insert();
  }

  // synrxnsR:
  rxns=ProblemSpace.synrxnsR.rxns;
  rxn_it=rxns.begin();
  cerr << rxns.size() << " syn reverse reactions" << endl;
  while (rxn_it!=rxns.end()) {
    REACTION rxn=*rxn_it++;
    rxn.insert();
  }

  // exchanges:
  rxns=ProblemSpace.exchanges.rxns;
  rxn_it=rxns.begin();
  cerr << rxns.size() << " exchange reactions" << endl;
  while (rxn_it!=rxns.end()) {
    REACTION rxn=*rxn_it++;
    rxn.insert();
  }

}


void insert_paths(const PROBLEM& ProblemSpace) {}

