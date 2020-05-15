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
  /// int whether or not to use breit frame or lab frame. 0 = lab, 1 = breit
  int breitFrame = std::stoi(argv[4]);

  TFile mc(mcFile.c_str());
  TTree *mctree = (TTree*)mc.Get("EICTree");
  
  TFile *outfile = new TFile(outputFile.c_str(), "recreate");
  jetTree = new TTree("jettree", "A tree with jets");

  setupJetTree(jetTree);

  mctree->AddFriend("Smeared", smearedFile.c_str());
  erhic::EventPythia* truthEvent(NULL);
  Smear::Event* smearEvent(NULL);

  mctree->SetBranchAddress("event", &truthEvent);
  mctree->SetBranchAddress("eventS", &smearEvent);

  JetDef R1jetdef(fastjet::antikt_algorithm, 1.0);
  R1jetdef.setMinJetPt(2.);
  R1jetdef.setMaxJetRapidity(4);
  SoftDropJetDef R1sd(0.01, 2, R1jetdef.getR());
  

  std::cout<<"begin event loop"<<std::endl;
  for(int event = 0; event < mctree->GetEntries(); ++event)
    {
      if(event % 20000 == 0)
	std::cout<<"Processed " << event << " events" << std::endl;

      mctree->GetEntry(event);

      truex = truthEvent->GetTrueX();
      truey = truthEvent->GetTrueY();
      trueq2 = truthEvent->GetTrueQ2();
     
      TruthEvent trueEvent(*truthEvent);
      trueEvent.setVerbosity(0);
      trueEvent.useBreitFrame(breitFrame);
      exchangeBoson = trueEvent.getExchangeBoson();

      /// Set event level cuts
      trueEvent.setMinQ2(16);
      trueEvent.setMinY(0.01);
      trueEvent.setMaxY(0.95);
      trueEvent.setMinX(0.00001);
      /// Check the cuts
      if(!trueEvent.passCuts()){
	continue;
      }

      trueEvent.processEvent();

      PseudoJetVec fjtruthR1Jets = trueEvent.getTruthJets(truthcs, R1jetdef);
      PseudoJetVec fjtruthR1SDJets = trueEvent.getTruthSoftDropJets(fjtruthR1Jets, R1sd);
      if(fjtruthR1Jets.size() == 0)
	{
	  continue;
	}

      SmearedEvent smearedEvent(*truthEvent, *smearEvent);
      smearedEvent.setVerbosity(0);     
      smearedEvent.useBreitFrame(breitFrame);
      smearedEvent.processEvent();
      recx = smearEvent->GetX();
      recy = smearEvent->GetY();
      recq2 = smearEvent->GetQ2();
      smearExchangeBoson = smearedEvent.getExchangeBoson();
      matchedParticles = smearedEvent.getMatchedParticles();      

      PseudoJetVec fjrecoR1Jets = smearedEvent.getRecoJets(cs, R1jetdef);
      std::vector<PseudoJetVec> fjmatchedR1Jets = 
      	smearedEvent.matchTruthRecoJets(fjtruthR1Jets, fjrecoR1Jets);

      PseudoJetVec fjrecoR1SDJets = 
	smearedEvent.getRecoSoftDropJets(fjrecoR1Jets, R1sd);
      std::vector<PseudoJetVec> fjmatchedR1SDJets = 
	smearedEvent.matchTruthRecoJets(fjtruthR1SDJets, fjrecoR1SDJets);

      truthR1Jets  = convertToTLorentzVectors(fjtruthR1Jets);
      recoR1Jets   = convertToTLorentzVectors(fjrecoR1Jets);
      recoR1SDJets = convertToTLorentzVectors(fjrecoR1SDJets);
      
      matchedR1Jets = convertMatchedJetVec(fjmatchedR1Jets);
      matchedR1SDJets = convertMatchedJetVec(fjmatchedR1SDJets);
      
      jetTree->Fill();
    }
  
  outfile->cd();
  jetTree->Write();
  outfile->Close();
  mc.Close();

  std::cout << "Finished EventLoop" << std::endl;

}

std::vector<std::vector<JetConstPair>> convertMatchedJetVec(std::vector<PseudoJetVec> vec)
{
  std::vector<std::vector<JetConstPair>> matchedJets;
  //num jets per event
  for(int i = 0; i < vec.size(); i++)
    {
      PseudoJetVec pair = vec.at(i);
      JetConstVec TLpair = convertToTLorentzVectors(pair);
      
      matchedJets.push_back(TLpair);
    }

  return matchedJets;
}

void setupJetTree(TTree *tree)
{

  jetTree->Branch("truthR1Jets", &truthR1Jets);
  jetTree->Branch("recoR1Jets", &recoR1Jets);
  jetTree->Branch("recoR1SDJets", &recoR1SDJets);
  jetTree->Branch("matchedR1Jets", &matchedR1Jets);
  jetTree->Branch("matchedR1SDJets", &matchedR1SDJets);
  jetTree->Branch("exchangeBoson", &exchangeBoson);
  jetTree->Branch("smearExchangeBoson", &smearExchangeBoson);
  jetTree->Branch("truex",&truex,"truex/D");
  jetTree->Branch("truey",&truey,"truey/D");
  jetTree->Branch("trueq2",&trueq2,"trueq2/D");
  jetTree->Branch("recx",&recx,"recx/D");
  jetTree->Branch("recy",&recy,"recy/D");
  jetTree->Branch("recq2",&recq2,"recq2/D");
  jetTree->Branch("matchedParticles",&matchedParticles);
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
