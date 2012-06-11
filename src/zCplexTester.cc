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
#include "cplexScript.h"

#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

using std::vector;
using std::map;
using std::string;

int main(int argc, char *argv[]) {
  printf("\n\n");

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

  std::cout<<"Hello World, growth media: "<<ProblemSpace.growth[0].media.size()
           <<", byproducts: "<<ProblemSpace.growth[0].byproduct.size()
           <<", biomass: "<<ProblemSpace.growth[0].biomass.size()
           <<", mutation: "<<ProblemSpace.growth[0].mutation.size()
           <<", growRate: "<<ProblemSpace.growth[0].growth_rate<<std::endl
           <<"GROWTH: "<<std::endl
           <<ProblemSpace.growth[0]<<std::endl<<std::endl;

  STOICHMATRIX ProblemStoich(ProblemSpace.fullrxns);
  
  std::cout<<"STOICHMATRIX"<<std::endl
           <<ProblemStoich<<std::endl;

  //if(toCplexLP(ProblemStoich, ProblemStoich[20], "Maximize"))
  //  cout<<"YAAAAAAAAY!"<<endl;
/* 
  vector<double> obj;
  obj.push_back(1.0);
  obj.push_back(2.0);
  obj.push_back(3.0);

  vector<vector<STOICH> > pstoich;
  vector<STOICH> temp;
  temp.push_back(STOICH(METID(1), -1.0));
  temp.push_back(STOICH(METID(2), 1.0));
  pstoich.push_back(temp);
  temp.clear();
  temp.push_back(STOICH(METID(1), 1.0));
  temp.push_back(STOICH(METID(2), -3.0));
  pstoich.push_back(temp);
  temp.clear();
  temp.push_back(STOICH(METID(1), 1.0));
  temp.push_back(STOICH(METID(2), 1.0));
  pstoich.push_back(temp);

  STOICHMATRIX pstoichmat(pstoich);

  vector<int> leg;
  leg.push_back(-1);
  leg.push_back(-1);

  vector<double> rhs;
  rhs.push_back(20);
  rhs.push_back(30);
*/

/*  lb.push_back(0);
  lb.push_back(0);
  lb.push_back(0);
*/ 

//  ub.push_back(40);
//  ub.push_back(1000);
//  ub.push_back(1000);


  STOICHMATRIX pstoichmat(ProblemSpace.fullrxns); 

//  vector<double> objfn = pstoichmat.getObjFromRxnID(RXNID(2142));

  IloEnv env;

  Solver sol(env, pstoichmat, pstoichmat.getObjFromRxnID(RXNID(2142)),
             "Maximize", ProblemSpace.fullrxns.lbVector(),
              ProblemSpace.fullrxns.ubVector() );

  sol.writeProblem("incomplete.lp");

  //sol.obj.setLinearCoef(sol.rxns[1], 5.4);

  sol.solve();

  sol.writeSolution("complete.sol");
  cout<<"GROWTH RATE: "<<sol.getObjVal()<<endl;
  vector<int> RxnIDs;
  sol.getRxnIDs(RxnIDs);
  vector<double> Rates;
  sol.getRxnRates(Rates);
  for(int i=0; i<sol.rxnsize; i++)
  {
    cout<<"ID : "<<RxnIDs[i]<<"\tRate: "<<Rates[i]<<endl;
  }


  env.end();

  return 0;

}


