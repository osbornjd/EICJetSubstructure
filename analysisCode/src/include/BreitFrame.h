#ifndef BREITFRAME_H
#define BREITFRAME_H

#include <eicsmear/erhic/EventPythia.h>
#include <eicsmear/smear/EventS.h>


class BreitFrame {
  
 public:
  BreitFrame(){}
  BreitFrame( erhic::EventPythia &truthEvent )
   : m_truthEvent(&truthEvent)
  {}
  ~BreitFrame(){}

  
  BreitFrame(erhic::EventPythia &truthEvent, Smear::Event &smearEvent)
   : m_truthEvent(&truthEvent)
   , m_smearEvent(&smearEvent)
  {}
  

  void labToBreitTruth( TLorentzVector *l );
  void labToBreitSmear( TLorentzVector *l );


 
 private:
  erhic::EventPythia *m_truthEvent;
  Smear::Event *m_smearEvent;

  int m_verbosity = 0;

};

#endif
