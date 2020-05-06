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
using JetConstPair = std::pair<TLorentzVector, std::vector<TLorentzVector>>;
using JetConstVec = std::vector<JetConstPair>;

PseudoJetVec getTruthJets(fastjet::ClusterSequence *truthcs, 
			  erhic::EventPythia *truthEvent, 
			  JetDef jetdef);

void setupJetTree(TTree *tree);

JetConstVec convertToTLorentzVectors(PseudoJetVec pseudoJets);

JetConstVec truthR1Jets, recoR1Jets, recoR1SDJets;

std::vector<std::vector<JetConstPair>> matchedR1Jets;

fastjet::ClusterSequence *cs, *truthcs;

TTree *jetTree;
