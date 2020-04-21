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

using namespace std;
using namespace fastjet;

class TruthEvent {
  
 public:

  TruthEvent();
  //  TruthEvent(erhic::EventPythia* ev);
  ~TruthEvent(){}

  // member functions //
  void Set_truthEvent(erhic::EventPythia* ev);
  void Set_truthPart(const Particle *part);
  void Set_truthJet(PseudoJet jet);
  void Set_groomed_truthJet(PseudoJet jet);

  int Get_processId() const {return processId;}
  int Get_eventNumber() const {return eventNumber;}
  double Get_true_q2() const {return true_q2;}
  double Get_true_x() const {return true_x;}
  double Get_q2() const {return q2;}

  
  double Get_part_px()  const {return part_px;}
  double Get_part_py()  const {return part_py;}
  double Get_part_pz()  const {return part_pz;}
  double Get_part_pt()  const {return part_pt;}
  double Get_part_E()   const {return part_E;}
  double Get_part_eta() const {return part_eta;}
  double Get_part_phi() const {return part_phi;}

  double Get_jet_px()  const  {return jet_px;}
  double Get_jet_py()  const  {return jet_py;}
  double Get_jet_pz()  const  {return jet_pz;}
  double Get_jet_pt()  const  {return jet_pt;}
  double Get_jet_E()   const  {return jet_E;}
  double Get_jet_eta() const  {return jet_eta;}
  double Get_jet_phi() const  {return jet_phi;}

  double Get_groomed_px()  const  {return groomed_px;}
  double Get_groomed_py()  const  {return groomed_py;}
  double Get_groomed_pz()  const  {return groomed_pz;}
  double Get_groomed_pt()  const  {return groomed_pt;}
  double Get_groomed_E()   const  {return groomed_E;}
  double Get_grooemd_eta() const  {return groomed_eta;}
  double Get_groomed_phi() const  {return groomed_phi;}
  
  void ProcessEvent(erhic::EventPythia* ev, JetDef def, SoftDropJetDef softdrop_def );
  void ParticleLoop(erhic::EventPythia* ev); 
  void JetLoop(JetDef def, contrib::SoftDrop sd);
  
  void PrintEvent();
  void PrintParticle();
  void PrintJet();
  void PrintGroomedJet();
  

 protected:

  // member variables //
  vector<PseudoJet> truthPseudoJets;
  vector<PseudoJet> truthJets;
  vector<const erhic::VirtualParticle*> truthParticles;  // not sure if this is needed, doesn't look like it was used in standalone.cpp example

  int processId, eventNumber, nTracks;
  int pid;
  double true_q2, true_x, q2;
  double part_px, part_py, part_pz, part_pt, part_E;
  double part_eta, part_phi;
  short  part_status;
  double jet_px, jet_py, jet_pz, jet_pt, jet_E;
  double jet_eta, jet_phi;
  double groomed_px, groomed_py, groomed_pz, groomed_pt, groomed_E;
  double groomed_eta, groomed_phi;

};


  // Setter and Getter implementation //

inline void TruthEvent::Set_truthEvent(erhic::EventPythia* ev) 
{
  processId       =  ev->GetProcess();
  eventNumber     =  ev->GetN(); 
  q2              =  ev->GetHardQ2();
  true_q2         =  ev->GetTrueQ2(); 
  true_x          =  ev->GetTrueX();
  truthParticles  =  ev->GetTracks();
  nTracks         =  ev->GetNTracks();

  truthPseudoJets.clear();
  truthJets.clear();
  
}

inline void TruthEvent::Set_truthPart(const Particle *part) 
{ 
  erhic::Pid pid  =  part->Id();
  part_px         =  part->GetPx();
  part_py         =  part->GetPy();
  part_pz         =  part->GetPz();
  part_E          =  part->GetE();
  part_status     =  part->GetStatus();
}

inline void TruthEvent::Set_truthJet(PseudoJet jet)
{
  jet_px   =  jet.px();
  jet_py   =  jet.py();
  jet_pz   =  jet.pz();
  jet_E    =  jet.E();
  jet_pt   =  jet.pt();
  jet_phi  =  jet.phi();
  jet_eta  =  jet.eta();
}

inline void TruthEvent::Set_groomed_truthJet(PseudoJet jet)
{
  groomed_px   =  jet.px();
  groomed_py   =  jet.py();
  groomed_pz   =  jet.pz();
  groomed_E    =  jet.E();
  groomed_pt   =  jet.pt();
  groomed_phi  =  jet.phi();
  groomed_eta  =  jet.eta();}

inline void TruthEvent::PrintEvent()
{
  cout << "TruthEvent::PrintEvent" << endl;
  cout << " ---- " << "Process ID : " << processId 
       << " ---- " << "Event # : " << eventNumber 
       << " ---- " << "Q2 : " << q2
       << " ---- " << "True Q2 : " << true_q2 
       << " ---- " << "True x : " <<  true_x
  << " ---- " << "# Tracks : " << nTracks << endl;
}

inline void TruthEvent::PrintParticle()
{
  cout << "TruthEvent::PrintParticle"  <<  endl;
  cout << " ---- " << "PID : "         <<  pid 
       << " ---- " << "Status : "      <<  part_status 
       << " ---- " << "Particle_px : " <<  part_px 
       << " ---- " << "Particle_py : " <<  part_py
       << " ---- " << "Particle_pz : " <<  part_pz
       << " ---- " << "Particle_E : "  <<  part_E << endl;
}

inline void TruthEvent::PrintJet()
{
  cout << "TruthEvent::PrintJet"    <<  endl;
  cout << " ---- " << "Jet_px  : "  <<  jet_px
       << " ---- " << "Jet_py  : "  <<  jet_py
       << " ---- " << "Jet_pz  : "  <<  jet_pz
       << " ---- " << "Jet_E   : "  <<  jet_E 
       << " ---- " << "Jet_pt  : "  <<  jet_pt
       << " ---- " << "Jet_eta : "  <<  jet_eta
       << " ---- " << "Jet_phi : "  <<  jet_phi << endl;
}

inline void TruthEvent::PrintGroomedJet()
{
  cout << "TruthEvent::PrintGroomedJet"        <<  endl;
  cout << " ---- " << "Groomed_px  : "  <<  groomed_px
       << " ---- " << "Groomed_py  : "  <<  groomed_py
       << " ---- " << "Groomed_pz  : "  <<  groomed_pz
       << " ---- " << "Groomed_E   : "  <<  groomed_E 
       << " ---- " << "Groomed_pt  : "  <<  groomed_pt
       << " ---- " << "Groomed_eta : "  <<  groomed_eta
       << " ---- " << "Groomed_phi : "  <<  groomed_phi << endl;
}

#endif
