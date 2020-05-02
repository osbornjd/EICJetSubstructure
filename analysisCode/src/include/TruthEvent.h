#ifndef TRUTHEVENT_H
#define TRUTHEVENT_H

#include <eicsmear/erhic/EventPythia.h>
#include <eicsmear/smear/EventS.h>

#include <fastjet/ClusterSequence.hh>
#include <fastjet/Selector.hh>
#include <fastjet/contrib/SoftDrop.hh>
#include <vector>

#include "JetDef.h"
#include "SoftDropJetDef.h"
#include "BreitFrame.h"

using PseudoJetVec = std::vector<fastjet::PseudoJet>;
using namespace fastjet;
using namespace std;

class TruthEvent {
  
 public:

  TruthEvent();
  TruthEvent(erhic::EventPythia &truthEvent)
   : m_truthEvent(&truthEvent)
  {}

  ~TruthEvent(){}

  // member functions //
  void processEvent( );
  void setVerbosity(int verb) { m_verbosity = verb; }

  PseudoJetVec getTruthJets(fastjet::ClusterSequence *cs,
			   JetDef jetDef);
  PseudoJetVec getTruthSoftDropJets(PseudoJetVec recoJets, 
				   SoftDropJetDef sdJetDef);

  //void setTruthEvent(erhic::EventPythia* ev);

  //void particleLoop(erhic::EventPythia* ev); 
  //void jetLoop(JetDef def, contrib::SoftDrop sd);
  
  //void printEvent();
  //void PrintJet();
  //void PrintGroomedJet();
  
 private:
  erhic::EventPythia *m_truthEvent;

  const erhic::ParticleMC *m_scatLepton;

  int m_verbosity = 0;

  PseudoJetVec m_particles;

  void setScatteredLepton();
  void setTruthParticles();
};


#endif
