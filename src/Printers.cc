#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "DataStructures.h"
#include "Printers.h"

using namespace std;

/**************** Generic printers **************/

void printIntMap(map<int, int> intMap) {
  map<int, int>::const_iterator i;
  if(intMap.empty()) {printf("EMPTY\n"); return; }
  for(i=intMap.begin(); i != intMap.end(); ++i) {
    printf("%d\t%d\n", i->first, i->second);
  }
  return;
}

void printIntVector(vector<int> intVector) {
  if(intVector.empty()) {printf("EMPTY\n"); return;}
  for(int i=0;i<intVector.size();i++)  { printf("%d ", (int) intVector[i]);  }
  printf("\n");
  return;
}

void printVector(vector<RXNID> intVector) {
  if(intVector.empty()) {printf("EMPTY\n"); return;}
  for(int i=0;i<intVector.size();i++)  { printf("%d ", (int) intVector[i]);  }
  printf("\n");
  return;
}

void printVector(vector<REV> intVector) {
  if(intVector.empty()) {printf("EMPTY\n"); return;}
  for(int i=0;i<intVector.size();i++)  { printf("%d ", (int) intVector[i]);  }
  printf("\n");
  return;
}

void printVector(vector<RXNDIRID> intVector) {
  if(intVector.empty()) {printf("EMPTY\n"); return;}
  for(int i=0;i<intVector.size();i++)  { printf("%d ", (int) intVector[i]);  }
  printf("\n");
  return;
}

void printVector(vector<METID> intVector) {
  if(intVector.empty()) {printf("EMPTY\n"); return;}
  for(int i=0;i<intVector.size();i++)  { printf("%d ", (int)intVector[i]);  }
  printf("\n");
  return;
}

void printDoubleVector(vector<double> doubleVector) {
  if(doubleVector.empty()) {printf("EMPTY\n"); return;}
  for(int i=0;i<doubleVector.size();i++)    {
    printf("%4.3f ", doubleVector[i]);
  }
  printf("\n");
  return;
}

void printIntSet(const set<int> &mySet) {
  if(mySet.empty()) { printf("EMPTY\n"); return; }
  for(set<int>::iterator it=mySet.begin(); it!=mySet.end(); it++) {
    printf("%d ", *it);
  }
  printf("\n");
  return;
}

/***************** Metablite / reaction printers ****************/

void printRxnFormula(const REACTION &rxn, string rxnString, bool printStoichPart) {
  vector<STOICH> st;
  if(printStoichPart) {
    st = rxn.stoich_part;
  } else {
    st = rxn.stoich;
  }
  sort(st.begin(), st.end());
  bool arrowDone = false; /* Needed to make sure we always get an arrow including on exchanges */
  for(int i=0; i < st.size(); i++) {
    double rxnCoeff = st[i].rxn_coeff;
    if(rxnCoeff < 0.0f) {
      rxnCoeff *= -1;
    }
    string oneReactantString;
    stringstream out;
    out << setprecision(5) << rxnCoeff << " " << st[i].met_name;
    rxnString += oneReactantString;

    if(i != st.size() - 1) {
      if(st[i+1].rxn_coeff * st[i].rxn_coeff < 0.0f) {
        if(rxn.net_reversible == -1) {
          rxnString += " <-- ";
        } else if(rxn.net_reversible == 0) {
          rxnString += " <=> ";
        } else {
          rxnString += " --> ";
        }
        arrowDone = true;
      } else {
        rxnString += " + ";
      }
    }
  }
  if(!arrowDone) {
    if(rxn.net_reversible == -1) {
      rxnString += " <-- ";
    } else if(rxn.net_reversible == 0) {
      rxnString += " <=> ";
    } else {
      rxnString += " --> ";
    }
  }
  return;
}


void printMETABOLITEinputs(const METABOLITE &metabolite){
  printf("\tid: %05d\n",(int)metabolite.id);
  cout << "\tname: " << metabolite.name << endl;;
  printf("\tinput: %d  ",metabolite.input);
  if(metabolite.input==0){ printf("(NO)\n");} else{printf("(YES)\n");}
  printf("\toutput: %d  ",metabolite.output);
  if(metabolite.output==0){ printf("(NO)\n");} else{printf("(YES)\n");}
  printf("\tbiomass: %d  ",metabolite.biomass);
  if(metabolite.biomass==0){ printf("(NO)\n");} else{printf("(YES)\n");}
  printf("\tsecondary_lone: %d  ",metabolite.secondary_lone);
  if(metabolite.secondary_lone==0){ printf("(NO)\n");} else{printf("(YES)\n");}
  if(metabolite.secondary_pair.size()>0){
    for(int j=0;j<metabolite.secondary_pair.size();j++){
      printf("\tsecondary_pair: %d  ",(int)metabolite.secondary_pair[j]);
    }
  }
  else{printf("(YES - PARTNER ID# GIVEN)\n");}
  printf("\n");
}

void printGROWTHinputs(const GROWTH &growth){
  int i;
  printf("GROWTH MEDIA:\n");
  for(i=0;i<growth.media.size();i++){
    printf("\t%05d\t%1.3f\n",(int)growth.media[i].id,growth.media[i].rate);
  }
  printf("\nBYPRODUCTS:\n");
  for(i=0;i<growth.byproduct.size();i++){
    printf("\t%05d\t%1.3f\n",(int)growth.byproduct[i].id,
           growth.byproduct[i].rate);
  }
  printf("\nMUTATIONS:\n");
  for(i=0;i<growth.mutation.size();i++){
    printf("\t%s\t%1.3f\n",growth.mutation[i].genename.c_str(),
           growth.mutation[i].act_coef);
  }
  printf("\tnum_of_metabolites: %d\n",(int)growth.biomass.size());
  if(growth.biomass.size() > 0){
    printf("\n BIOMASS EQUATION:\n\n");
    printSTOICHIOMETRY_by_id(growth.biomass,growth.biomass.size(),1);}
  printf("\nGROWTH RATE: %f\n\n",growth.growth_rate);
}

void printSynRxns(const RXNSPACE &synrxns, const RXNSPACE &fullrxns) {
  for(int i=0; i<synrxns.rxns.size(); i++) {
    cout << "Reaction " << synrxns.rxns[i].name << " (rev=" << synrxns.rxns[i].net_reversible << ") synrxns: ";
    for(int j=0; j<synrxns.rxns[i].syn.size(); j++) {
      cout << fullrxns.rxnFromId(synrxns.rxns[i].syn[j]).name 
	   << " rev = " << (int)fullrxns.rxnFromId(synrxns.rxns[i].syn[j]).net_reversible;
    }
    printf("\n");
  }
}

void printREACTIONintermediates(const REACTION &reaction, int print_type){
  printf("\tid: %05d\n",(int)reaction.id);
  cout << "\tname: " << reaction.name << endl;
  printf("\treversible: %d  ",(int)reaction.net_reversible);
  if(reaction.net_reversible==0){ printf("(YES)\n");}
  if(reaction.net_reversible==-1){ printf("(NO - BACKWARDS)\n");}
  if(reaction.net_reversible==1){ printf("(NO - FORWARDS)\n");}

  printf("\t [lb: %f; ub: %f]\n", reaction.lb, reaction.ub);
  printf("\ttransporter: %d  ",reaction.transporter);
  if(reaction.transporter==0){ printf("(NO)\n");} else{printf("(YES)\n");}
  printf("\tlikelihood_init: %f\n",reaction.init_likelihood);
  printf("\tlikelihood_curr: %f\n",reaction.current_likelihood);
  printf("\tnum_of_metabolites: %d\n",(int)reaction.stoich.size());
  if(reaction.stoich.size() > 0){
    if(print_type==1){
      printf("\n STOICHIOMETRY:\n\n");
      printSTOICHIOMETRY_by_id(reaction.stoich,reaction.stoich.size(),
                               reaction.init_reversible);}
  }
  printf("\n");
  printf("\tnum_of_non_secondary_metabolites: %d\n", (int)reaction.stoich_part.size());
  if(reaction.stoich_part.size() > 0){
    if(print_type==1){
      printf("\n STOICHIOMETRY:\n\n");
      printSTOICHIOMETRY_by_id(reaction.stoich_part,
                               reaction.stoich_part.size(),
                               reaction.init_reversible);}
  }
  printf("\n");
  if(reaction.syn.size()>1){
    printf("\tSYNONYMS: ");
    for(int i=0;i<reaction.syn.size();i++){
      printf("%05d, ",(int)reaction.syn[i]);
    }
    printf("\n\n--------------------\n");
  }
  return;
}


void printSTOICHIOMETRY_by_id(const vector<STOICH> &stoich, int num_met, int rev){
  unsigned int i;
  STOICH temps;
  vector<STOICH> tempv = stoich;

  /* Quick little sort to make sure negatives are before positives */
  for(i=1;i<tempv.size();){
    if(tempv[i-1].rxn_coeff <= tempv[i].rxn_coeff){ i++; continue; }
    if(tempv[i-1].rxn_coeff > tempv[i].rxn_coeff){
      temps = tempv[i];
      tempv[i] = tempv[i-1];
      tempv[i-1] = temps;
      if(i>1){i--;}
      continue;
    }
  }

  /* Print reaction */
  i=0;
  while(tempv[i].rxn_coeff<0){
    printf("%2.2f %s ",tempv[i].rxn_coeff,tempv[i].met_name.c_str());
    if(tempv[i+1].rxn_coeff<0){ printf("+ ");}
    i++;}
  if(rev==-1){ printf(" <--  ");}
  if(rev==0) { printf(" <--> ");}
  if(rev==1) { printf("  --> ");}
  while(tempv[i].rxn_coeff>0 && i<tempv.size()){
    printf("%2.2f %s ",tempv[i].rxn_coeff,tempv[i].met_name.c_str());
    if(i!=(tempv.size()-1)){ printf("+ ");}
    i++;}
  printf("\n");
}

void printREACTIONvector(const vector<REACTION> &reaction, int print_type) {
  for(unsigned int i=0; i<reaction.size(); i++) { printREACTIONinputs(reaction[i], print_type);}
}

void printREACTIONinputs(const REACTION &reaction, int print_type){
  printf("\tid: %05d\n",(int)reaction.id);
  cout << "\tname: " << reaction.name << endl;
  printf("\tinit_reversible: %d  ",(int)reaction.init_reversible);
  if(reaction.init_reversible==0){ printf("(YES)\n");}
  if(reaction.init_reversible==-1){ printf("(NO - BACKWARDS)\n");}
  if(reaction.init_reversible==1){ printf("(NO - FORWARDS)\n");}

  printf("\tFree make: %d ", reaction.freeMakeFlag);
  if(reaction.freeMakeFlag == 1) { printf("(YES)\n"); } else { printf("(NO)\n"); }

  printf("\tnet_reversible: %d \n", (int)reaction.net_reversible);
  printf("\tLower bound: %f\n", reaction.lb);
  printf("\tUpper bound: %f\n", reaction.ub);
  printf("\ttransporter: %d  ",reaction.transporter);
  if(reaction.transporter==0){ printf("(NO)\n");} else{printf("(YES)\n");}
  printf("\tlikelihood: %f",reaction.init_likelihood);
  if(reaction.init_likelihood < 0.0f){
    printf("(SPECIAL FLAG - ");
    if(reaction.init_likelihood > -1.1f && reaction.init_likelihood < -0.9f){
      printf("DO NOT INCLUDE IN NETWORK)\n");}
    if(reaction.init_likelihood > -2.1f && reaction.init_likelihood < -1.9f){
      printf("USER DEFINED INCLUDE IN NETWORK)\n");}
    if(reaction.init_likelihood > -3.1f && reaction.init_likelihood < -2.9f){
      printf("BLACK MAGIC)\n");}
    if(reaction.init_likelihood > -4.1f && reaction.init_likelihood < -3.9f){
      printf("SPONTANEOUS REACTION)\n");}
    if(reaction.init_likelihood > -5.1f && reaction.init_likelihood < -4.9f){
      printf("NOT FOUND)\n");}
  }
  else{ printf("\n");}
  printf("\tnum_of_metabolites: %d\n",(int)reaction.stoich.size());
  if(reaction.stoich.size() > 0){
    if(print_type==1){
      printf("\n STOICHIOMETRY:\n\n");
      printSTOICHIOMETRY_by_id(reaction.stoich,reaction.stoich.size(),
                               reaction.init_reversible);}
  }
  printf("\n");
  printf("\tnum_of_non_secondary_metabolites: %d\n", (int)reaction.stoich_part.size());
  if(reaction.stoich_part.size() > 0){
    if(print_type==1){
      printf("\n STOICHIOMETRY:\n\n");
      printSTOICHIOMETRY_by_id(reaction.stoich_part,
                               reaction.stoich_part.size(),
                               reaction.init_reversible);}
  }
  printf("\n");
}


void printRxnsFromIntVector(const vector<RXNID> &intVector, const RXNSPACE &rxnspace) {
  if(intVector.empty()) {printf("EMPTY\n"); return;}
  cout.precision(8);
  for(int i=0;i<intVector.size();i++){
    cout << rxnspace.rxnFromId(intVector[i]).name << "(" << setw(8) 
	 << rxnspace.rxnFromId(intVector[i]).init_likelihood << ")" << endl;
  }
  printf("\n");
}

void printRxnsFromIntVector(const vector<RXNDIRID> &intVector, const RXNSPACE &rxnspace) {
  if(intVector.empty()) {printf("EMPTY\n"); return;}
  for(int i=0;i<intVector.size();i++){
    printf("%s(%4.3f) ", rxnspace.rxnFromId((RXNID) abs(intVector[i])).name.c_str(), 
	   rxnspace.rxnFromId((RXNID) abs(intVector[i])).init_likelihood);
  }
  printf("\n");
}

void printRxnsFromIntSet(const set<RXNID> &intSet, const RXNSPACE &rxnspace) {
  if(intSet.empty()) { printf("EMPTY\n"); return; }
  for(set<RXNID>::iterator it=intSet.begin(); it!=intSet.end(); it++) {
    cout << rxnspace.rxnFromId(*it).name << "(" << rxnspace.rxnFromId(*it).init_likelihood << ") ";
  }
  printf("\n");
  return;
}

//change name to printMets
void printMetsFromIntVector(const vector<METID> &Vector, const PROBLEM &ProblemSpace) {
  if(Vector.empty()) {printf("EMPTY\n"); return;}
  for(int i=0;i<Vector.size();i++){
    cout << ProblemSpace.metabolites[Vector[i]].name << " ";
  }
  printf("\n");
}

/* The metabolites in RXNSPACE should have the same order as the doubles in doubleVec
   Prints out the reaction names and then the double values in a nice format (omits zero values) */
void printDoubleVector_rxns(const RXNSPACE &rxnspace, const vector<double> &doubleVec) {
  if(rxnspace.rxns.size() != doubleVec.size()) {
    printf("WARNING: Attempt to call printDoubleVector_rxns with a RXNSPACE and double vector of different sizes\n");
    return;
  }
  for(int i=0; i<doubleVec.size(); i++) {
    if(doubleVec[i] > 0.0001 || doubleVec[i] < -0.0001) {
      printf("%s\t%1.3f\n", rxnspace.rxns[i].name.c_str(), doubleVec[i]);
    }
  }
  return;
}

/* The metabolites in METSPACE should have the same order as the doubles in doubleVec
   Prints out the metbaolite names and then the double values in a nice format */
void printDoubleVector_mets(const METSPACE &metspace, const vector<double> &doubleVec) {
  if(metspace.mets.size() != doubleVec.size()) {
    printf("WARNING: Attempt to call printDoubleVector_mets with a METSPACE and double vector of different sizes\n");
    return;
  }
  for(int i=0; i<doubleVec.size(); i++) {
    if(doubleVec[i] > 0.0001 || doubleVec[i] < -0.0001) {
      printf("%s\t%1.3f\n", metspace.mets[i].name.c_str(), doubleVec[i]);
    }
  }
  return;
}


void printMetsFromStoich(vector<STOICH> a){
  int i;
  for(i=0;i<a.size();i++){
    cout << a[i].met_name << " ";
  }
  printf("\n");
  return;
}


/************* ETC printers ***********************/

void printNetReactionVector(const vector<NETREACTION> &netReactions, const PROBLEM &problemSpace) {
  for(int i=0; i<netReactions.size(); i++) {
    printf("ETC %d: ", i);
    for(int j=0; j<netReactions[i].rxnDirIds.size(); j++) {
      REACTION tmpRxn = problemSpace.fullrxns.rxnFromId((RXNID)abs(netReactions[i].rxnDirIds[j]));
      printf("%s(%4.3f)\t", tmpRxn.name.c_str(), tmpRxn.init_likelihood);
    }
    printf("\n");
  }
  return;
}


/*************** Path printers ******************/

void PrintPathSummary_verbose(const PATHSUMMARY &psum) {
  printf("----------------------------------------\n");
  printf("RxnDirIds:");
  printVector(psum.rxnDirIds);
  printf("deadEndIds:");
  printVector(psum.deadEndIds);
  printf("AllMetsConsumed:");
  printVector(psum.metsConsumed);
  printf("Output ID: %d\n", (int)psum.outputId);
  printf("Growth Index: %d\n", psum.growthIdx[0]);
  printf("K number: %d\n", psum.k_number);
}

void PrintPathSummary(const vector<vector<vector<PATHSUMMARY> > > &psum){
  /* Standard Dummies */
  unsigned int i,j,k;
  for(i=0;i<psum.size();i++){
    for(j=0;j<psum[i].size();j++){
      for(k=0;k<psum[i][j].size();k++){
        printf("-%d-%d-%d- ",i,j,k);
        printVector(psum[i][j][k].rxnDirIds);
      }
    }
  }
  return;
}

void printPathResults(const vector<PATH> &path) {
  for(unsigned int i=0;i<path.size();i++) {
    printf("Path number: %d corresponding to output %d...\n", i, (int)path[i].outputId);
    printf("Inputs required to reach output: ");
    printVector(path[i].inputIds);
    printf("Number of reactions: ");
    printf("%d\n", (int)path[i].rxnIds.size());
    printf("Reactions required: ");
    printVector(path[i].rxnIds);
    printf("Reaction directionality: ");
    printVector(path[i].rxnDirection);
    printf("Total likelihood: %4.3f \n\n", path[i].totalLikelihood);
    printf("Dead ends: ");
    printVector(path[i].deadEndIds);
    printf("---------------------------------------------------------------------\n\n");
  }
}

/* Prints the metabolite names in addition to just the IDs */
void printPathResults(const vector<PATH> &path, PROBLEM &ProblemSpace) {
  for(unsigned int i=0;i<path.size();i++) {
    printf("Path number: %d corresponding to output %d...\n", i, (int)path[i].outputId);
    printf("Inputs required to reach output: "); 
    printMetsFromIntVector(path[i].inputIds,ProblemSpace);
    printf("Number of reactions: ");
    printf("%d\n", (int)path[i].rxnIds.size());
    printf("Reactions required: ");
    printRxnsFromIntVector(path[i].rxnIds,ProblemSpace.synrxns);
    printf("Reaction directionality: ");
    printVector(path[i].rxnDirection);
    printf("Total likelihood: %4.3f \n\n", path[i].totalLikelihood);
    printf("Dead ends: ");
    printMetsFromIntVector(path[i].deadEndIds,ProblemSpace);
    printf("---------------------------------------------------------------------\n\n");
  }
  return;
}

/* ALso prints reaction likelihoods from rxnspace (why do we need this if the rxnspace is also found in PROBLEMSPACE?) */
void printPathResults(const vector<PATH> &path, PROBLEM &ProblemSpace, RXNSPACE &rxnspace) {
  for(unsigned int i=0;i<path.size();i++) {
    cout << "Path number: " << i << " corresponding to output " << ProblemSpace.metabolites[path[i].outputId].name
	 << " ..." << endl;
    printf("Inputs required to reach output: "); 
    printMetsFromIntVector(path[i].inputIds,ProblemSpace);
    printf("Number of reactions: ");
    printf("%d\n", (int)path[i].rxnIds.size());
    printf("Reactions required: ");
    printRxnsFromIntVector(path[i].rxnIds,rxnspace);
    printf("Reaction likelihoods: ");
    for(int j=0;j<path[i].rxnIds.size();j++){
      printf("%1.2f  ",rxnspace[path[i].rxnIds[j]].current_likelihood);}
    printf("\n");
    printf("Reaction directionality: ");
    printVector(path[i].rxnDirection);
    printf("Total likelihood: %4.3f \n\n", path[i].totalLikelihood);
    printf("Dead ends: ");
    printMetsFromIntVector(path[i].deadEndIds,ProblemSpace);
    printf("---------------------------------------------------------------------\n\n");
  }
  return;
}

void PrintPathSummary2(const vector<vector<vector<PATHSUMMARY> > > &psum, RXNSPACE &rxnspace){
  for(int i=0;i<psum.size();i++){
    for(int j=0;j<psum[i].size();j++){
      for(int k=0;k<psum[i][j].size();k++){
	printf("--------\n");
        printf("-%d-%d-%d- ",i,j,k);
        printRxnsFromIntVector(psum[i][j][k].rxnDirIds,rxnspace);
      }
    }
  }
  return;
}

void PrintPathSummary2(const vector<vector<vector<PATHSUMMARY> > > &psum, PROBLEM &ProblemSpace){
  PrintPathSummary2(psum,ProblemSpace.synrxns);
  return;
}

void PrintGapfillResult(const vector<GAPFILLRESULT> &res, const PROBLEM &problemSpace, const vector<int> &kToPrint) {
  for(int j=0; j<res.size(); j++) {
    if(res[j].deadEndSolutions.size() == 0) { continue; }
    printf("%s (k=%d) --> ", problemSpace.metabolites[res[j].deadMetId].name.c_str(), kToPrint[j]);
    for(int n=0; n<res[j].deadEndSolutions[kToPrint[j]].size(); n++) {
      REACTION tmp = problemSpace.fullrxns.rxnFromId(res[j].deadEndSolutions[kToPrint[j]][n]);
      printf("%s(%4.3f)\t", tmp.name.c_str(), tmp.init_likelihood);
    }
    printf("\n");
  }
}

/******************** Output files ***************/

/* Output reactions in InRxns to a text file (including name and direction) */
void MATLAB_out(const string fileName, const vector<REACTION> &InRxns){
  unsigned int i,j,k;
  STOICH temps;
  vector<STOICH> stoich;
  FILE* output;
  output = fopen(fileName.c_str(), "w");

  for(i=0;i<InRxns.size();i++){

    stoich = InRxns[i].stoich;
    /* Sort stoichs by coefficient */
    for(j=1;j<stoich.size();){
      if(stoich[j-1].rxn_coeff <= stoich[j].rxn_coeff){ j++; continue; }
      if(stoich[j-1].rxn_coeff > stoich[j].rxn_coeff){
        temps = stoich[j];
        stoich[j] = stoich[j-1];
        stoich[j-1] = temps;
        if(j>1){j--;}
        continue;
      }
    }

    bool allPositive(true);
    for(j=0; j<stoich.size(); j++) {
      if(stoich[j].rxn_coeff < 0.0f) {
        allPositive = false;
        break;
      }
    }

    /* 1st column: Print out name */
    fprintf(output,"%s\t",InRxns[i].name.c_str());

    /* 2nd column: Print out reaction */
    for(j=0,k=0;j<stoich.size();j++){
      if(stoich[j].rxn_coeff < 0.0f){
        fprintf(output,"%1.8f %s",-stoich[j].rxn_coeff,stoich[j].met_name.c_str());
        if((j+1) < stoich.size() && stoich[j+1].rxn_coeff < 0.0f){ fprintf(output," + ");}
      }
      /* If either 1) the sign changes between the current and next STOICH, or
         2) there's only positive coefficients [and thus we need to put an arrow --> before the first one] */
      if( ((j+1) < stoich.size() && stoich[j].rxn_coeff < 0.0f && stoich[j+1].rxn_coeff > 0.0f ) ||
          ( j==0 && allPositive) ){
        k++;
        if(InRxns[i].net_reversible==0){fprintf(output," <--> ");}
        if(InRxns[i].net_reversible==1){fprintf(output," --> ");}
        if(InRxns[i].net_reversible==-1){fprintf(output," <-- ");}
      }
      if(stoich[j].rxn_coeff > 0.0f){
        fprintf(output,"%1.8f %s",stoich[j].rxn_coeff,stoich[j].met_name.c_str());
        if((j+1) < stoich.size()){ fprintf(output," + ");}
      }
    }
    if(k==0){
      if(InRxns[i].net_reversible==0){fprintf(output," <--> ");}
      if(InRxns[i].net_reversible==1){fprintf(output," --> ");}
      if(InRxns[i].net_reversible==-1){fprintf(output," <-- ");}
    }
    fprintf(output, "\t");

    /* 3rd\4th column: Print out the flux bound */
    fprintf(output, "%1.5f\t%1.5f\t", InRxns[i].lb, InRxns[i].ub);

    /* 6th/6th column: Print out original and final reversibility */
    fprintf(output, "%d\t%d\t", (int)InRxns[i].init_reversible, (int)InRxns[i].net_reversible);

    /* 7th/8th column: Print out initial and current likelihood */
    fprintf(output, "%1.5f\t%1.5f", InRxns[i].init_likelihood, InRxns[i].current_likelihood);

    /* End of reaction entry */
    fprintf(output,"\n");
  }

  fflush(output);
  fclose(output);
  return;
}

/* Report reactions involved in psum (AFTER unsynonymizing - if you try this before unsyn it will die a horrible death) */
void PATHS_rxns_out(const string fileName, const vector<PATHSUMMARY> &psum, const PROBLEM &problem) {

  const RXNSPACE &rxnspace = problem.fullrxns;
  const METSPACE &metspace = problem.metabolites;

  /* Reactions */
  FILE* output = fopen(fileName.c_str(), "w");

  fprintf(output, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
	  "PSUM ID", "GROWTH CONDITION", "TARGET METABOLITE", "K (#th shortest)", "REACTION", "RXNDIRID", "LIKELIHOOD");

  for(int i=0; i<psum.size(); i++) {
    for(int j=0; j<psum[i].rxnDirIds.size(); j++) {
      fprintf(output, "%ld\t%d\t%s\t%d\t%s\t%d\t%4.3f\n",
	      (long int)psum[i].id, psum[i].growthIdx[0], metspace[psum[i].outputId].name.c_str(), psum[i].k_number, 
	      rxnspace.rxnFromId((RXNID)abs(psum[i].rxnDirIds[j])).name.c_str(), (int)psum[i].rxnDirIds[j], 
	      rxnspace.rxnFromId((RXNID)abs(psum[i].rxnDirIds[j])).init_likelihood);
    }
  }

  fflush(output);
  fclose(output);
}

/* After un-synonymizing: Output an adjacency-list for the data in the PSUM:
   Reaction   metabolite  direction  edge weight [default: current_likelihood] 
direction is 0 if it is a reversible edge, -1 means metabolite --> reaction, and +1 means reaction --> metabolite */
void ONEPATH_adj_list(const string fileName, const PATHSUMMARY &psum, const PROBLEM &problem) {
  FILE* output = fopen(fileName.c_str(), "w");
  fprintf(output, "%s\t%s\t%s\t%s\n", "REACTION", "METABOLITE", "DIRECTION", "EDGEWEIGHT");
  for(int i=0; i<psum.rxnDirIds.size(); i++) {
    REACTION tmprxn = problem.fullrxns.rxnFromId((RXNID)abs(psum.rxnDirIds[i]));
    for(int j=0; j<tmprxn.stoich.size(); j++) {
      int dir;
      if(tmprxn.net_reversible == 0) {
	dir = 0;
      } else if (tmprxn.net_reversible * tmprxn.stoich[j].rxn_coeff > 0.0f) { 
	/* A --> B: Stoich coeff for B is 1 and net_reversible = 1, we want the arrow pointing towards B so dir = 1*/
	/* B <-- A: Stoich coeff for B is -1 but net_reversible = -1, we want thearrow pointing towards B so dir = 1 */
	dir = 1;
      } else { dir = -1; }
      fprintf(output, "%s\t%s\t%d\t%1.4f\n", 
	      tmprxn.name.c_str(), tmprxn.stoich[j].met_name.c_str(), dir, tmprxn.current_likelihood);
    }
  }
  fflush(output);
  fclose(output);
}

/* Report list of metabolites in each PATH. The reaction names in psum
 must correspond to the reactions in rxnspace (i.e. for un-synnonymized RXNSPACE, the RXNSPACE should be fullrxns,
 and for the synonymized version from Dijkstras it should be synrxns, etc.

This format can be easily sorted and checked e.g. for what reactions are foudn in what paths */
void PATHS_mets_out(const string fileName, const vector<PATHSUMMARY> &psum, const PROBLEM &problem) {
  const RXNSPACE &rxnspace = problem.fullrxns;
  const METSPACE &metspace = problem.metabolites;
  /* Reactions */
  FILE* output = fopen(fileName.c_str(), "w");

  fprintf(output, "%s\t%s\t%s\t%s\t%s\n",
	  "PSUM ID", "GROWTH CONDITION", "TARGET METABOLITE", "K (#th shortest)", "METABOLITE", "REACTION containing metabolite");

  for(int i=0; i<psum.size(); i++) {
    for(int j=0; j<psum[i].rxnDirIds.size(); j++) {
      REACTION tmp = rxnspace.rxnFromId((RXNID)abs(psum[i].rxnDirIds[j]));
      for(int k=0; k<tmp.stoich.size(); k++) {
	fprintf(output, "%ld\t%d\t%s\t%d\t%s\t%s\n",
		psum[i].id, psum[i].growthIdx[0], metspace[psum[i].outputId].name.c_str(), psum[i].k_number, 
		metspace[tmp.stoich[k].met_id].name.c_str(), tmp.name.c_str());
      }
    }
  }

  fflush(output);
  fclose(output);
}


void ANNOTATIONS_out(const string filename, const vector<REACTION> &annotated_reaction_list) {
  FILE* output = fopen(filename.c_str(), "w");
  fprintf(output, "%s\t%s\t%s\n", "REACTION", "ANNOTATION", "GENE_PROBABILITY");
  for(int i=0; i<annotated_reaction_list.size(); i++) {
    for(int j=0; j<annotated_reaction_list[i].annote.size(); j++) {
      fprintf(output, "%s\t%s\t%4.3f\n", annotated_reaction_list[i].name.c_str(), 
	      annotated_reaction_list[i].annote[j].genename.c_str(), 
	      annotated_reaction_list[i].annote[j].probability);
    }
  }
  fclose(output);
}
