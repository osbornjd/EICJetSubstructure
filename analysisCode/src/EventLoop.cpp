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
      if(event % 10 == 0)
	std::cout<<"Processed " << event << " events" << std::endl;

      mctree->GetEntry(event);

      SmearedEvent smearedEvent(*truthEvent, *smearEvent);
      smearedEvent.processEvent();

      std::vector<fastjet::PseudoJet> recoR1Jets = 
	smearedEvent.getRecoJets(jetdef);
      
      std::vector<fastjet::PseudoJet> recoR1SDJets = 
	smearedEvent.getRecoSoftDropJets(recoR1Jets, sd);

    

  for(int i = 0; i < recoR1Jets.size(); i++)
    {
      fastjet::PseudoJet recojet = recoR1Jets.at(i);
      fastjet::PseudoJet sdjet = recoR1SDJets.at(i);
      std::cout << "antikt jet: " << recojet.px() << " " << recojet.py()
		<< " " << recojet.pz() << " " << recojet.e() << std::endl;
        std::cout << "softdrop jet : "<< sdjet.px() << " " << sdjet.py()
      	<< " " << sdjet.pz() << " " << sdjet.e() << std::endl;


    }
}
  std::cout << "Finished EventLoop" << std::endl;

}
