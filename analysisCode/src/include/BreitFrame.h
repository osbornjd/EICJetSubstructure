#ifndef BREITFRAME_H
#define BREITFRAME_H

#include <eicsmear/erhic/EventPythia.h>
#include <eicsmear/smear/EventS.h>

#include <iostream>

/**
 * This class is responsible for boosting particles into the Breit frame from
 * the default lab frame that is output from EICSmear. It can be constructed
 * and called for either truth or smeared events.
 */
class BreitFrame {
  
 public:
  BreitFrame(){}

  /// Truth event constructor
  BreitFrame( erhic::EventPythia &truthEvent )
   : m_truthEvent(&truthEvent)
  {}

  /// Smeared event constructor
  BreitFrame(erhic::EventPythia &truthEvent, Smear::Event &smearEvent)
   : m_truthEvent(&truthEvent)
   , m_smearEvent(&smearEvent)
  {}

  ~BreitFrame(){}

  /// Function calls which boost a passed TLorentzVector to the breit frame,
  /// with either the truth or smeared exchange boson
  void labToBreitTruth( TLorentzVector *l );
  void labToBreitSmear( TLorentzVector *l );
 
 private:
  erhic::EventPythia *m_truthEvent;
  Smear::Event *m_smearEvent;

  int m_verbosity = 0;

};

#endif
