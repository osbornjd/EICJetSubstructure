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

  double getSmearedX() {return m_x;}
  double getSmearedQ2() {return m_q2;}
  double getSmearedY() {return m_y;}
  double getSmearedNu() {return m_nu;}

  void setScatteredLepton();
  void setSmearedParticles();
  
  PseudoJetVec getRecoJets(fastjet::ClusterSequence *cs,
			   JetDef jetDef);
  PseudoJetVec getRecoSoftDropJets(PseudoJetVec recoJets, 
				   SoftDropJetDef sdJetDef);

  std::vector<PseudoJetVec> matchTruthRecoJets(PseudoJetVec truthjets, 
					       PseudoJetVec recojets);

  void useBreitFrame(bool yesorno) { m_breitFrame = yesorno; }
 private:
  /// Need truth event for identifying only final state particles
  erhic::EventPythia *m_truthEvent;
  Smear::Event *m_smearEvent;

  const Smear::ParticleMCS *m_scatLepton;
  bool m_breitFrame;
  std::vector<PseudoJetVec> m_matchedJets;
  
  PseudoJetVec m_particles;
  double m_x, m_y, m_q2, m_nu;
  int m_verbosity = 0;

 

};


#endif
