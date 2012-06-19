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
/*
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
  cout<<"ORIG: "<<ProblemStoich.getCell(4,20)<<"\tCHANGED: ";
  ProblemStoich.setCell(4,20,6.334);
  cout<<ProblemStoich.getCell(4,20)<<"\tCHANGED: ";
  ProblemStoich.setCell(4,20) = 9.3;
  cout<<ProblemStoich.getCell(4,20)<<endl;
*/
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




//  vector<double> objfn = pstoichmat.getObjFromRxnID(RXNID(2142));

  IloEnv env;

  SOLVER sol(env, ProblemSpace); 

  
  sol.writeProblem("incomplete.lp");

  sol.solve();

  sol.writeSolution("complete.sol");
  METID mid(23);
  RXNID rid(508);
//  cout<<"Coef at("<<int(mid)<<","<<int(rid)<<"): "<<sol.getCoef(mid,rid)<<endl;
//  if(!sol.setCoef(mid,rid, 333.225))
//    cout<<"Yeah, girl, back that drive up!"<<endl; 
//  cout<<"Coef at("<<int(mid)<<","<<int(rid)<<"): "<<sol.getCoef(mid,rid)<<endl;
  map<RXNID, double> objMap;
  cout<<"Objective"<<endl;
  cout<<"ID: 99999\tCoeff: "<<sol.getObjCoef(RXNID(99999))<<endl;
  cout<<"ID: 508\tCoeff: "<<sol.getObjCoef(RXNID(508))<<endl;
  cout<<"ID: 848484\tCoeff: "<<sol.getObjCoef(RXNID(848484))<<endl;
  sol.changeObjective(rid, 2.34);
  cout<<"NEWID: 508\tCoeff: "<<sol.getObjCoef(rid)<<endl;
  sol.getObjFn(objMap);
  map<RXNID, double>::iterator it = objMap.begin();
  for(;it != objMap.end(); ++it)
  {
    cout<<"ID: "<<it->first<<"\tCoeff: "<<it->second<<endl;
  }
    
  objMap[RXNID(1996)] = -0.234;

  cout<<"NEW OBJECTIVE FUNCTION"<<endl;

  it = objMap.begin();
  for(;it != objMap.end(); ++it)
  {
    cout<<"ID: "<<it->first<<"\tCoeff: "<<it->second<<endl;
  }
  
  cout<<"1NOW?: "<<sol.hasRXNID(RXNID(848484))<<" AND "
      <<sol.hasMETID(METID(678912))<<endl;
  sol.addEmptyRxn(RXNID(848484), -IloInfinity, IloInfinity);
  sol.addEmptyMet(METID(678912));
  
  cout<<"2NOW?: "<<sol.hasRXNID(RXNID(848484))<<" AND "
      <<sol.hasMETID(METID(678912))<<endl;
 
  sol.changeObjective(objMap);
  cout<<"THIS: "<<double(-IloInfinity)<<endl;
  sol.solve();
  sol.writeProblem("incomplete2.lp");
  cout<<"GROWTH RATE: "<<sol.getObjVal()<<endl;
  map<RXNID, double> RxnIDs;
  sol.getRxnValues(RxnIDs);


  for(map<RXNID, double>::iterator it=RxnIDs.begin(); it!=RxnIDs.end(); it++)
  {
    cout<<"ID : "<<it->first<<"\tRate: "<<it->second<<endl;
  }


  env.end();

  return 0;

}


