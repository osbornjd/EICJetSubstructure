#ifndef SMEAREDEVENT_H
#define SMEAREDEVENT_H

#include <utility>
#include <tuple>

#include <eicsmear/smear/EventS.h>
#include <eicsmear/erhic/EventPythia.h>

#include <TLorentzVector.h>
#include <TF1.h>

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
   , m_smearHCal(false)
  {
    m_HCalRes = new TF1("hcalres","sqrt(0.1*0.1+0.5*0.5/x)",0.1,275);
  }

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

  void useBreitFrame(bool breit) { m_breitFrame = breit; }
  void smearHCal(bool smear) { m_smearHCal = smear;}
  void setMaxPPID(float maxPPID[14]){for(int i=0; i<14; i++)m_maxPPID[i] = maxPPID[i];}

 private:
  /// Need truth event for identifying only final state particles
  erhic::EventPythia *m_truthEvent;
  Smear::Event *m_smearEvent;

  /// set eta bins for pid requirements
  const static int m_nEtaBins = 14;
  float m_etaBins[m_nEtaBins+1] = {-3.5,-3.,-2.5,-2,-1.5,-1,-0.5,0,0.5,1,1.5,2,2.5,3,3.5};
  float m_maxPPID[m_nEtaBins];


  double m_maxPartEta;
  double m_minPartPt;

  const Smear::ParticleMCS *m_scatLepton;
  bool m_breitFrame;
  std::vector<PseudoJetVec> m_matchedJets;

  TF1 *m_HCalRes;
  bool m_smearHCal;

  /// Vectors of particles to be kept
  PseudoJetVec m_particles;
  PseudoJetVec m_truthParticles;
  int m_verbosity = 0;

};


#endif
