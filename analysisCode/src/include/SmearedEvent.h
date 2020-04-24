#ifndef SMEAREDEVENT_H
#define SMEAREDEVENT_H

#include <eicsmear/smear/EventS.h>
#include <eicsmear/erhic/EventPythia.h>

#include <TLorentzVector.h>

#include "JetDef.h"
#include "SoftDropJetDef.h"

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

  std::vector<fastjet::PseudoJet> getRecoJets(JetDef jetDef);
  std::vector<fastjet::PseudoJet> getRecoSoftDropJets(std::vector<fastjet::PseudoJet> recoJets, SoftDropJetDef sdJetDef);

  std::vector<std::vector<fastjet::PseudoJet>> matchTruthRecoJets(std::vector<fastjet::PseudoJet> truthjets, std::vector<fastjet::PseudoJet> recojets);

 private:
  /// Need truth event for identifying only final state particles
  erhic::EventPythia *m_truthEvent;
  Smear::Event *m_smearEvent;

  const Smear::ParticleMCS *m_scatLepton;

  std::vector<std::vector<fastjet::PseudoJet>> m_matchedJets;
  std::vector<fastjet::PseudoJet> m_particles;

  fastjet::ClusterSequence *cs;

  int m_verbosity = 0;

  void setScatteredLepton();
  void setSmearedParticles();
  

};


#endif
