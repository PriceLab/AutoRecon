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
  METID mid(20003);
  RXNID rid(50800);
//  cout<<"Coef at("<<int(mid)<<","<<int(rid)<<"): "<<sol.getCoef(mid,rid)<<endl;
//  if(!sol.setCoef(mid,rid, 333.225))
//    cout<<"Yeah, girl, back that drive up!"<<endl; 
//  cout<<"Coef at("<<int(mid)<<","<<int(rid)<<"): "<<sol.getCoef(mid,rid)<<endl;
  map<RXNID, double> testMap;
  map<METID, double> metMap;

  sol.getUB(testMap);
  testMap[rid] = 12.78;
  testMap[RXNID(50007)] = 333.212;
  cout<<"RID: "<<rid<<"\tLB: "<<sol.getLB(rid)<<"\tUB: "<<sol.getUB(rid)<<endl;
  cout<<"MID: "<<mid<<"\tLB: "<<sol.getLB(mid)<<"\tUB: "<<sol.getUB(mid)<<endl;

  cout<<  "RID UB: "<<sol.setUB(testMap);
  sol.getLB(testMap);
  testMap[rid] = -8.23;
  testMap[RXNID(50007)] = -321.2;
  cout<<"\tLB: "<<sol.setLB(testMap);
  sol.getLB(metMap);
  metMap[mid] = -32.3;
  metMap[METID(23000)] = -7.424;
  cout<<"\tMID LB: "<<sol.setLB(metMap);
  sol.getUB(metMap);
  metMap[mid] = 9.3;
  metMap[METID(23000)] = 48.3;
  cout<<"\tUB: "<<sol.setUB(metMap)<<endl;

  cout<<"RID: "<<rid<<"\tLB: "<<sol.getLB(rid)<<"\tUB: "<<sol.getUB(rid)<<endl;
  cout<<"MID: "<<mid<<"\tLB: "<<sol.getLB(mid)<<"\tUB: "<<sol.getUB(mid)<<endl;

  cout<<"RID LB: "<<sol.setLB(rid, -88.22)
    <<"\tUB: "<<sol.setUB(rid, 12.78)
    <<"\tMID LB: "<<sol.setLB(mid, -32.3)
    <<"\tUB: "<<sol.setUB(mid, 9.3)<<endl;

  cout<<"RID: "<<rid<<"\tLB: "<<sol.getLB(rid)<<"\tUB: "<<sol.getUB(rid)<<endl;
  cout<<"MID: "<<mid<<"\tLB: "<<sol.getLB(mid)<<"\tUB: "<<sol.getUB(mid)<<endl;

  map<RXNID, double>::iterator it = testMap.begin();
  for(;it != testMap.end(); ++it)
  {
    cout<<"RID: "<<it->first<<"\tUB: "<<it->second<<endl;
  }

  sol.getLB(testMap);

  it = testMap.begin();
  for(;it != testMap.end(); ++it)
  {
    cout<<"RID: "<<it->first<<"\tLB: "<<it->second<<endl;
  }

  sol.getUB(metMap);


  map<METID, double>::iterator it2 = metMap.begin();
  for(;it2 != metMap.end(); ++it2)
  {
    cout<<"MID: "<<it2->first<<"\tUB: "<<it2->second<<endl;
  }

  sol.getLB(metMap);

  it2 = metMap.begin();
  for(;it2 != metMap.end(); ++it2)
  {
    cout<<"MID: "<<it2->first<<"\tLB: "<<it2->second<<endl;
  }

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


