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

  void setSmearedParticles();
  
  std::vector<fastjet::PseudoJet> getReconstructedJets(JetDef jetDef);
  
 private:
  /// Need truth event for identifying only final state particles
  erhic::EventPythia *m_truthEvent;
  Smear::Event *m_smearEvent;

  std::vector<fastjet::PseudoJet> m_particles;
};


#endif
