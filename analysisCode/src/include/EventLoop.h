/// Fastjet includes
#include <fastjet/ClusterSequence.hh>
#include <fastjet/Selector.hh>
#include <fastjet/contrib/SoftDrop.hh>
#include <fastjet/PseudoJet.hh>

/// Root includes
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TRint.h>
#include <TROOT.h>

/// EIC smear includes
#include <eicsmear/erhic/EventPythia.h>
#include <eicsmear/smear/EventS.h>

/// Local includes
#include "TruthEvent.h"
#include "SmearedEvent.h"
#include "JetDef.h"
#include "SoftDropJetDef.h"

#include <iostream>
#include <vector>

using PseudoJetVec = std::vector<fastjet::PseudoJet>;
using TLorentzVectorVec = std::vector<TLorentzVector>;
/// A JetConstPAir is a pair of a jet 4 vector and vector of jet constituents
using JetConstPair = std::pair<TLorentzVector, std::vector<TLorentzVector>>;
using JetConstVec = std::vector<JetConstPair>;

PseudoJetVec getTruthJets(fastjet::ClusterSequence *truthcs, 
			  erhic::EventPythia *truthEvent, 
			  JetDef jetdef);

void setupJetTree(TTree *tree);

JetConstVec convertToTLorentzVectors(PseudoJetVec pseudoJets);
std::vector<std::vector<JetConstPair>> convertMatchedJetVec(std::vector<PseudoJetVec>);

JetConstVec truthR1Jets, recoR1Jets, recoR1SDJets;
double truex, truey, trueq2;
double recx, recy, recq2;
TLorentzVector exchangeBoson;

/// This structure is a vector of vector of matched truth-reco jets. 
/// Each entry of std::vector<JetConstPair> is length 2 - first the truth
/// jet, then the reco jet
std::vector<std::vector<JetConstPair>> matchedR1Jets, matchedR1SDJets;

fastjet::ClusterSequence *cs, *truthcs;

TTree *jetTree;
