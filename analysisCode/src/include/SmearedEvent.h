#ifndef SMEAREDEVENT_H
#define SMEAREDEVENT_H

#include <utility>
#include <tuple>

#include <eicsmear/smear/EventS.h>
#include <eicsmear/erhic/EventPythia.h>

#include <TLorentzVector.h>

#include "JetDef.h"
#include "SoftDropJetDef.h"
#include "BreitFrame.h"

using PseudoJetVec = std::vector<fastjet::PseudoJet>;
using TLorentzVectorVec = std::vector<TLorentzVector>;
using JetConstPair = std::pair<TLorentzVector, std::vector<TLorentzVector>>;
using JetConstVec = std::vector<JetConstPair>;
using TLorentzPair = std::pair<TLorentzVector, TLorentzVector>;
using TLorentzPairVec = std::vector<TLorentzPair>;

/**
 * This class is the analagous class to TruthEvent but for smeared particles.
 * The functionality is largely the same as TruthEvent, but with some additional
 * considerations for smeared particles from EICSmear.
 */
class SmearedEvent {
  
 public:
  SmearedEvent(){}
  SmearedEvent(erhic::EventPythia &truthEvent, Smear::Event &smearEvent)
   : m_truthEvent(&truthEvent)
   , m_smearEvent(&smearEvent)
  {}

  ~SmearedEvent(){}

  /// Main workhorse function, which is called from event loop
  void processEvent();
  void setVerbosity(int verb) { m_verbosity = verb; }
  TLorentzVector getExchangeBoson();
  TLorentzVector getScatteredLepton(){return m_scatLepton->Get4Vector();}

  /// Set some event level criteria
  void setMaxPartEta(double eta){m_maxPartEta = eta;}
  void setMinPartPt(double pt){m_minPartPt = pt;}
  void setScatteredLepton();
  void setSmearedParticles();

  /// Returns a TLorentzPairVec matching truth and smeared particles used
  // in the jet clustering. Useful for identifying e.g. response matrices
  TLorentzPairVec getMatchedParticles();

  PseudoJetVec getRecoJets(fastjet::ClusterSequence *cs,
			   JetDef jetDef);
  PseudoJetVec getRecoSoftDropJets(PseudoJetVec recoJets, 
				   SoftDropJetDef sdJetDef);

  /// Match truth and reco jets via their deltaR separation
  std::vector<PseudoJetVec> matchTruthRecoJets(PseudoJetVec truthjets, 
					       PseudoJetVec recojets);

  void useBreitFrame(bool yesorno) { m_breitFrame = yesorno; }

 private:
  /// Need truth event for identifying only final state particles
  erhic::EventPythia *m_truthEvent;
  Smear::Event *m_smearEvent;

  double m_maxPartEta;
  double m_minPartPt;

  const Smear::ParticleMCS *m_scatLepton;
  bool m_breitFrame;
  std::vector<PseudoJetVec> m_matchedJets;

  /// Vectors of particles to be kept
  PseudoJetVec m_particles;
  PseudoJetVec m_truthParticles;
  int m_verbosity = 0;

};


#endif
