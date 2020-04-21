#include "include/TruthEvent.h"
#include <iostream>

TruthEvent::TruthEvent(){cout << "A truth event" << endl;}

void TruthEvent::ProcessEvent(erhic::EventPythia* ev, JetDef def, SoftDropJetDef softdrop_def)
{ 
  contrib::SoftDrop softdrop = softdrop_def.getSoftDrop(); 
  Set_truthEvent(ev);
  //PrintEvent();
  ParticleLoop(ev);
  JetLoop(def, softdrop);   
 }

void TruthEvent::ParticleLoop(erhic::EventPythia* ev) 
{
  // initialize variables for boost to Breit frame -- this will be moved
  double Px,Py,Pz,P0,qx,qy,qz,q0;
  for (int part = 0; part < nTracks; part++)
    {
      const Particle *truthPart = ev->GetTrack(part);
      Set_truthPart(truthPart);

      // Block for boost to Breit frame -- this will be moved //
      if (part==1)
	{
	  Px = part_px; Py = part_py; Pz = part_pz; P0 = part_E;
	}
      if (part==3) 
	{ 
	  qx = part_px; qy = part_py; qz = part_pz; q0 = part_E;
	}
      double E_breit = 2*true_x*P0 + q0;
      TVector3 breit((2*true_x*Px+qx)/E_breit,(2*true_x*Py+qy)/E_breit, (2*true_x*Pz+qz)/E_breit);
      //PrintParticle();
      TLorentzVector part_4vec = truthPart->PxPyPzE();
      //cout << " Lab frame... " << endl;
      //part_4vec.Print();
      part_4vec.Boost(breit);
      //cout << " Breit frame... " << endl; 
      //part_4vec.Print();
      truthPseudoJets.push_back(PseudoJet(part_px, part_py, part_pz, part_E));
      //truthPseudoJets.push_back(PseudoJet(part_4vec));

      // End Breit Block //
    }
  
}

void TruthEvent::JetLoop(JetDef def, contrib::SoftDrop sd) 
{ 
  Selector selectRapidity = SelectorAbsRapMax(4);
  Selector selectPtMin = SelectorPtMin(5.0);
  Selector selectBoth = selectPtMin and selectRapidity;

  /// Make the truth jets
  ClusterSequence cs(truthPseudoJets, def.GetJetDef());
  vector<PseudoJet> allTruthJets = sorted_by_pt(cs.inclusive_jets());
  /// Make the cuts
  truthJets = selectBoth(allTruthJets);
  cout << "truthjets : " << truthJets.size() << endl;
  
  for(int truthJet = 0; truthJet < truthJets.size(); truthJet++)
    {
      /// Look at antikt constituents
      vector<PseudoJet> constituents = truthJets[truthJet].constituents();
      /// get jet properties
      // double jetphi = truthJets[truthJet].phi();
      Set_truthJet(truthJets[truthJet]);
      PrintJet();
      
      /// run soft drop on truth anti-kt jets
      PseudoJet softDropJet = sd(truthJets[truthJet]);
      
      // now you can do stuff with the SD jet
      //float subJetDR = softDropJet.structure_of<contrib::SoftDrop>().delta_R(); 
      Set_groomed_truthJet(softDropJet);
      // PrintGroomedJet();
    }  
}


