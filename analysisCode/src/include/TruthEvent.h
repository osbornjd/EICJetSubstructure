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

/**
 * This class stores truth information, like final state particles, and also
 * defines various truth level cuts on those particles
 */
class TruthEvent {
  
 public:

  TruthEvent() {}
  TruthEvent(erhic::EventPythia &truthEvent)
   : m_truthEvent(&truthEvent)
  {}

  ~TruthEvent(){}

  /// Processes event by getting all final state particles 
  void processEvent();
  void setVerbosity(int verb) { m_verbosity = verb; }

  /// Get truth jets from list of final state particles
  PseudoJetVec getTruthJets(fastjet::ClusterSequence *cs,
			   JetDef jetDef);
  PseudoJetVec getTruthSoftDropJets(PseudoJetVec recoJets, 
				   SoftDropJetDef sdJetDef);

  /// Setters for various cuts
  void useBreitFrame(bool yesorno) { m_breitFrame = yesorno; }
  void setMinQ2(double q2) { m_minq2 = q2; }
  void setMinY(double y) {m_minY = y; }
  void setMaxY(double y) {m_maxY = y; }
  void setMinX(double x) {m_minX = x; }
  void setProcessId(int id) {m_processId = id;}
  void setMinPartPt(double pt) {m_minPartPt = pt;}
  void setMaxPartEta(double eta) {m_maxPartEta = eta;}
  /// Return boolean whether or not event passes truth event cuts
  bool passCuts();
  TLorentzVector getExchangeBoson();
  TLorentzVector getScatteredLepton(){return m_scatLepton->Get4Vector();}

 private:
  /// Member variables defining the truth level cuts
  int m_processId = -1;
  double m_minq2;
  double m_minY;
  double m_maxY;
  double m_minX;
  double m_minPartPt;
  double m_maxPartEta;
  bool m_breitFrame;

  erhic::EventPythia *m_truthEvent;

  const erhic::ParticleMC *m_scatLepton;

  int m_verbosity = 0;

  PseudoJetVec m_particles;

  void setScatteredLepton();
  void setTruthParticles();
};


#endif
