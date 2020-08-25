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

#include <sstream>
#include <iostream>
#include <vector>

using PseudoJetVec = std::vector<fastjet::PseudoJet>;
using TLorentzVectorVec = std::vector<TLorentzVector>;
using TLorentzPair = std::pair<TLorentzVector, TLorentzVector>;
using TLorentzPairVec = std::vector<TLorentzPair>;
/// A JetConstPair is a pair of a jet 4 vector and vector of jet constituents
using JetConstPair = std::pair<TLorentzVector, std::vector<TLorentzVector>>;
using JetConstVec = std::vector<JetConstPair>;

/// Functions to set branch addresses for trees
void setupJetTree();
void setupRunTree();

/// Takes a PseudoJetVec and converts it to a JetConstVec. SDJet boolean
/// flag notifies if the first two entries in the constituent vector
/// are the subjets
JetConstVec convertToTLorentzVectors(PseudoJetVec pseudoJets, bool SDJet);

/// Function that calls convertToTLorentzVectors for the matched jet data structure
std::vector<std::vector<JetConstPair>> 
     convertMatchedJetVec(std::vector<PseudoJetVec> vec, bool SDJet);

/// Note - for SDJets, the first two entries in the constituent vector, i.e.
/// first two entries in the second of the pair list are the two subjets
JetConstVec truthR1Jets, recoR1Jets, recoR1SDJets, truthR1SDJets;

double truex, truey, trueq2, truenu;
double recx, recy, recq2, recnu;
int processId;
float nEventsTried, totalCrossSection, nEventsGen;
float integratedLumi;
TLorentzVector exchangeBoson, smearExchangeBoson, scatteredE, smearScatteredE;

/// This structure is a vector of vector of matched truth-reco jets. 
/// Each entry of std::vector<JetConstPair> is length 2 - first the truth
/// jet, then the reco jet
std::vector<std::vector<JetConstPair>> matchedR1Jets, matchedR1SDJets;
TLorentzPairVec matchedParticles;
fastjet::ClusterSequence *cs, *truthcs;

TTree *jetTree, *runTree;


float maxPPID[14] = {0,0,0,0,0,20,20,30,30,30,50,20,20,45};
