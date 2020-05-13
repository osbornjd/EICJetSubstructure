
#include "analyzeJets.h"


void analyzeJets()
{

  gROOT->ProcessLine(".L ../src/fastJetLinker.C+");
  setupTree();





}



void setupTree()
{
  infile = TFile::Open("pE275_eE18_minqsq9.root");

  jettree = (TTree*)file->Get("jettree");
  jettree->SetBranchAddress("recx", &recx);
  jettree->SetBranchAddress("recq2", &recq2);
  jettree->SetBranchAddress("recy", &recy);
  jettree->SetBranchAddress("truex", &truex);
  jettree->SetBranchAddress("truey", &truey);
  jettree->SetBranchAddress("trueq2", &trueq2);
  jettree->SetBranchAddress("truthR1Jets", &truthJets);
  jettree->SetBranchAddress("recoR1Jets", &recoJets);
  jettree->SetBranchAddress("recoR1SDJets", &recoSDJets);
  jettree->SetBranchAddress("matchedR1Jets", &matchedJets);
  jettree->SetBranchAddress("matchedR1SDJets", &matchedSDJets);
  jettree->SetBranchAddress("exchangeBoson", &truthExchangeBoson);


}
