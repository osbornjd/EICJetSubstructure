#include "include/EventLoop.h"


int main(int argc, char **argv)
{
  /// Link the libraries to be able to write std::vector<TLorentzVector>
  /// and std::pairs to TTreess
  gROOT->ProcessLine(".L src/fastJetLinker.C+");
  
  /// Collect arguments
  /// Truth MC file
  std::string mcFile = argv[1];
  /// Smeared MC file
  std::string smearedFile = argv[2];
  /// Name of output root file for trees to reside
  std::string outputFile = argv[3];

  TFile mc(mcFile.c_str());
  TTree *mctree = (TTree*)mc.Get("EICTree");
  
  TFile *outfile = new TFile(outputFile.c_str(), "recreate");
  jetTree = new TTree("jettree", "A tree with jets");

  setupJetTree(jetTree);

  TruthEvent event;
  SmearedEvent smearevent;

  JetDef R1jetdef(fastjet::antikt_algorithm, 1.0);
  R1jetdef.setMinJetPt(2.);
  R1jetdef.setMaxJetRapidity(4);
  SoftDropJetDef R1sd(0.01, 2, R1jetdef.getR());
  
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
      smearedEvent.setVerbosity(0);
      smearedEvent.processEvent();

      
      PseudoJetVec fjrecoR1Jets = smearedEvent.getRecoJets(cs, R1jetdef);
      PseudoJetVec fjtruthR1Jets = getTruthJets(truthcs, truthEvent, R1jetdef);
      matchedR1Jets = smearedEvent.matchTruthRecoJets(fjtruthR1Jets, fjrecoR1Jets);
      PseudoJetVec fjrecoR1SDJets = smearedEvent.getRecoSoftDropJets(fjrecoR1Jets, R1sd);

      truthR1Jets  = convertToTLorentzVectors(fjtruthR1Jets);
      recoR1Jets   = convertToTLorentzVectors(fjrecoR1Jets);
      recoR1SDJets = convertToTLorentzVectors(fjrecoR1SDJets);
      
      jetTree->Fill();
    }
  
  outfile->cd();
  jetTree->Write();
  outfile->Close();
  mc.Close();

  std::cout << "Finished EventLoop" << std::endl;

}


/**
 * function to get some truth jets for testing. will remove later once 
 * truthevent class is finished
 */
std::vector<fastjet::PseudoJet> getTruthJets(fastjet::ClusterSequence *truthcs, 
					     erhic::EventPythia* truthEvent, 
					     JetDef jetdef)
{
  std::vector<fastjet::PseudoJet> truthJets;
  
  /// Just making a truth jet finder place holder, will remove later
  for(int i=0; i<truthEvent->GetNTracks(); ++i)
    {
      const Particle *truthPart = truthEvent->GetTrack(i);
      /// skip the beam particles
      if( i < 3 )
	continue;
      // only final state particles
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

  truthcs = new fastjet::ClusterSequence(truthJets, jetdef.getJetDef());

  PseudoJetVec allTruthJets = fastjet::sorted_by_pt(truthcs->inclusive_jets());
  /// Make eta/pt selections
  fastjet::Selector selectPt = fastjet::SelectorPtMin(jetdef.getMinJetPt());
  fastjet::Selector selectEta = fastjet::SelectorAbsRapMax(jetdef.getMaxJetRapidity());
  fastjet::Selector select = selectPt and selectEta;
  
  PseudoJetVec selectTruthJets = select(allTruthJets);
  
  return selectTruthJets;
}


void setupJetTree(TTree *tree)
{

  jetTree->Branch("truthR1Jets", &truthR1Jets);
  jetTree->Branch("recoR1Jets", &recoR1Jets);
  jetTree->Branch("recoR1SDJets", &recoR1SDJets);
  jetTree->Branch("matchedR1Jets", &matchedR1Jets);


  return;
}


JetConstVec convertToTLorentzVectors(PseudoJetVec pseudoJets)
{
  JetConstVec jets;

  for(int jet = 0; jet < pseudoJets.size(); jet++)
    {
      fastjet::PseudoJet pseudojet = pseudoJets.at(jet);
      
      /// swap fastjet::pseudojet with a TLorentzVector
      TLorentzVector tJet;
      tJet.SetPxPyPzE(pseudojet.px(),
		      pseudojet.py(),
		      pseudojet.pz(),
		      pseudojet.e());
      
      /// Get jet constituents
      PseudoJetVec constituents = pseudojet.constituents();
      TLorentzVectorVec tConstituents;
      for(int con = 0; con < constituents.size(); con++)
	{
	  fastjet::PseudoJet fjConstituent = constituents.at(con);

	  TLorentzVector tConstituent;
	  tConstituent.SetPxPyPzE(fjConstituent.px(),
				  fjConstituent.py(),
				  fjConstituent.pz(),
				  fjConstituent.e());

	  tConstituents.push_back(tConstituent);

	}
      
      jets.push_back(std::make_pair(tJet, tConstituents));
    }

  return jets;

}
