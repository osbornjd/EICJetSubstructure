#ifndef TRUTHEVENT_H
#define TRUTHEVENT_H

#include <utility>
#include <tuple>

#include <eicsmear/erhic/EventPythia.h>
#include <eicsmear/smear/EventS.h>

#include <TLorentzVector.h>

#include "JetDef.h"
#include "SoftDropJetDef.h"
#include "BreitFrame.h"

using PseudoJetVec = std::vector<fastjet::PseudoJet>;
using namespace fastjet;
using namespace std;

class TruthEvent {
  
 public:

  TruthEvent() {}
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

  void useBreitFrame(bool yesorno) { m_breitFrame = yesorno; }
  void setMinQ2(double q2) { m_minq2 = q2; }
  void setMinY(double y) {m_minY = y; }
  void setMaxY(double y) {m_maxY = y; }
  void setMinX(double x) {m_minX = x; }
  bool passCuts();

 private:

  double m_minq2;
  double m_minY;
  double m_maxY;
  double m_minX;
  bool m_breitFrame;

  erhic::EventPythia *m_truthEvent;

  const erhic::ParticleMC *m_scatLepton;

  int m_verbosity = 0;

  PseudoJetVec m_particles;

  void setScatteredLepton();
  void setTruthParticles();
};


#endif
