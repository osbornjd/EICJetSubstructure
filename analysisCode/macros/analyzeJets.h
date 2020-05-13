

#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

#include <iostream>


using TLorentzVectorVec = std::vector<TLorentzVector>;
/// A JetConstPair is a pair of a jet 4 vector and vector of jet constituents
using JetConstPair = std::pair<TLorentzVector, std::vector<TLorentzVector>>;
using JetConstVec = std::vector<JetConstPair>;

void setupTree();

TLorentzVector truthExchangeBoson;
JetConstVec truthJets, recoJets, recoSDJets;
std::vector<std::vector<JetConstPair>> matchedJets, matchedSDJets;
double recx, recy, recq2, truey, truex, trueq2;
