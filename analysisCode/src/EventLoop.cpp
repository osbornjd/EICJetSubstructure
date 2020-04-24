/// Fastjet includes
#include <fastjet/ClusterSequence.hh>
#include <fastjet/Selector.hh>
#include <fastjet/contrib/SoftDrop.hh>

/// Root includes
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>

/// EIC smear includes
#include <eicsmear/erhic/EventPythia.h>
#include <eicsmear/smear/EventS.h>

/// Local includes
#include "include/TruthEvent.h"
#include "include/SmearedEvent.h"
#include "include/JetDef.h"
#include "include/SoftDropJetDef.h"

#include <iostream>

std::vector<fastjet::PseudoJet> getTruthJets(erhic::EventPythia *truthEvent, JetDef jetdef);

int main(int argc, char **argv)
{
  /// Collect arguments
  std::string mcFile = argv[1];
  std::string smearedFile = argv[2];

  TFile mc(mcFile.c_str());
  TTree *mctree = (TTree*)mc.Get("EICTree");
  
  TruthEvent event;
  SmearedEvent smearevent;

  JetDef jetdef(fastjet::antikt_algorithm, 1.0);
  jetdef.setMinJetPt(2.);
  jetdef.setMaxJetRapidity(4);
  SoftDropJetDef sd(0.01, 2, jetdef.getR());
  
  mctree->AddFriend("Smeared", smearedFile.c_str());
  erhic::EventPythia* truthEvent(NULL);
  Smear::Event* smearEvent(NULL);

  mctree->SetBranchAddress("event", &truthEvent);
  mctree->SetBranchAddress("eventS", &smearEvent);

  std::cout<<"begin event loop"<<std::endl;
  for(int event = 0; event < mctree->GetEntries(); ++event)
    {
      //if(event % 10 == 0)
	std::cout<<"Processed " << event << " events" << std::endl;

      mctree->GetEntry(event);

      SmearedEvent smearedEvent(*truthEvent, *smearEvent);
      smearedEvent.setVerbosity(0);
      smearedEvent.processEvent();

      std::vector<fastjet::PseudoJet> truthjets = getTruthJets(truthEvent, 
							       jetdef);

      std::vector<fastjet::PseudoJet> recoR1Jets = 
	smearedEvent.getRecoJets(jetdef);

      std::vector<std::vector<fastjet::PseudoJet>> matchedJets = 
	smearedEvent.matchTruthRecoJets(truthjets, recoR1Jets);
      
      std::vector<fastjet::PseudoJet> recoR1SDJets = 
	smearedEvent.getRecoSoftDropJets(recoR1Jets, sd);


      
    }
  std::cout << "Finished EventLoop" << std::endl;

}



std::vector<fastjet::PseudoJet> getTruthJets(erhic::EventPythia* truthEvent, JetDef jetdef)
{
  std::vector<fastjet::PseudoJet> truthJets;
  
  /// Just making a truth jet finder place holder, will remove later
  for(int i=0; i<truthEvent->GetNTracks(); ++i)
    {
      const Particle *truthPart = truthEvent->GetTrack(i);
      /// skip the beam particles
      if( i < 3 )
	continue;
      // oly final state particles
      if(truthPart->GetStatus() != 1)
	continue;
      /// skip scattered electron
      if(truthPart->GetE() == truthEvent->ScatteredLepton()->GetE())
	continue;
      
      truthJets.push_back(fastjet::PseudoJet(truthPart->GetPx(),
					     truthPart->GetPy(),
					     truthPart->GetPz(),
					     truthPart->GetE()));
    }
  
  fastjet::ClusterSequence truthcs(truthJets, jetdef.getJetDef());
  std::vector<fastjet::PseudoJet> allTruthJets = fastjet::sorted_by_pt(truthcs.inclusive_jets());
  /// Make eta/pt selections
  fastjet::Selector selectPt = fastjet::SelectorPtMin(jetdef.getMinJetPt());
  fastjet::Selector selectEta = fastjet::SelectorAbsRapMax(jetdef.getMaxJetRapidity());
  fastjet::Selector select = selectPt and selectEta;
  
  std::vector<fastjet::PseudoJet> selectTruthJets = select(allTruthJets);
  
  return selectTruthJets;
}
