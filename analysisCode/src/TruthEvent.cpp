#include "include/TruthEvent.h"

#include <iostream>

TruthEvent::TruthEvent(){cout << "A truth event" << endl;}

void TruthEvent::ProcessEvent(erhic::EventPythia* ev)
{
    
  Set_truthEvent(ev);
  ParticleLoop(ev);
  //  FindJets();
  // JetLoop();
    
 }

void TruthEvent::ParticleLoop(erhic::EventPythia* ev) 
{
  for (int part = 0; part < nTracks; part++)
    {
      const Particle *truthPart = ev->GetTrack(part);
      Set_truthPart(truthPart);
      truthPseudoJets.push_back(PseudoJet(part_px, part_py, part_pz, part_E));
    }
  
}

/*
void TruthEvent::FindJets() 
{  
  /// Make the truth jets
  ClusterSequence cs(truthPseudoJets, jetDef);
  vector<PseudoJet> allTruthJets = sorted_by_pt(cs.inclusive_jets());
  /// Make the cuts
  truthJets = selectBoth(allTruthJets);
  
}

void TruthEvent::JetLoop() 
{  
  for(int truthJet = 0; truthJet < truthJets.size(); truthJet++)
    {
      /// Look at antikt constituents
      vector<PseudoJet> constituents = truthJets[truthJet].constituents();
      /// get jet properties
      // double jetphi = truthJets[truthJet].phi();
      Set_truthJet(truthJets[truthJet]);
      
      /// run soft drop on truth anti-kt jets
      PseudoJet softDropJet = sd(truthJets[truthJet]);
      
      // now you can do stuff with the SD jet
      //float subJetDR = softDropJet.structure_of<contrib::SoftDrop>().delta_R(); 
      Set_groomed_truthJet(truthJets[truthJet]);
    }  
  
}

*/
