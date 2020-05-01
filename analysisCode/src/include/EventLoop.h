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

PseudoJetVec getTruthJets(fastjet::ClusterSequence *truthcs, 
			  erhic::EventPythia *truthEvent, 
			  JetDef jetdef);

void setupTree(TTree *tree);

std::vector<std::pair<TLorentzVector, std::vector<TLorentzVector>>>
  convertToTLorentzVectors(std::vector<fastjet::PseudoJet> pseudoJets);

std::vector<std::pair<TLorentzVector, std::vector<TLorentzVector>>> 
  truthR1Jets, recoR1Jets, recoR1SDJets;

std::vector<PseudoJetVec> matchedR1Jets;

fastjet::ClusterSequence *cs, *truthcs;

TTree *jetTree;
