#include "include/EventLoop.h"


int main(int argc, char **argv)
{
  /// Link the libraries to be able to write std::vector<TLorentzVector>
  /// and std::pairs to TTrees
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

  /// Create branches for trees
  setupJetTree();
  setupRunTree();
  
  mctree->AddFriend("Smeared", smearedFile.c_str());
  
  erhic::EventPythia* truthEvent(NULL);
  Smear::Event* smearEvent(NULL);

  mctree->SetBranchAddress("event", &truthEvent);
  mctree->SetBranchAddress("eventS", &smearEvent);

  /// Make a basic R=1 jet definition
  JetDef R1jetdef(fastjet::antikt_algorithm, 1.0);
  R1jetdef.setMinJetPt(4.);
  R1jetdef.setMaxJetRapidity(3.5);
  
  SoftDropJetDef R1sd(0.1, 0, R1jetdef.getR());
  
  /// Collect information to write out integrated lumi from PYTHIA run
  TObjString *nEventsString(NULL), *crossSectionString(NULL), 
    *nEventsTriedString(NULL);
  mc.GetObject("nTrials", nEventsTriedString);
  mc.GetObject("nEvents", nEventsString);
  mc.GetObject("crossSection", crossSectionString);
  
  std::stringstream stream;
  if(nEventsString != nullptr){
    std::stringstream(nEventsString->GetString().Data()) >> nEventsGen;
  }
  if(crossSectionString != nullptr){
    std::stringstream(crossSectionString->GetString().Data()) >> totalCrossSection;
  }
  if(nEventsTriedString != nullptr){
    std::stringstream(nEventsTriedString->GetString().Data()) >> nEventsTried;
  }

  /// total cross section is units of micro barn
  integratedLumi = (float) nEventsGen / totalCrossSection;
  runTree->Fill();
  
  std::cout<<"begin event loop"<<std::endl;
  for(int event = 0; event < mctree->GetEntries(); ++event)
    {
      if(event % 20000 == 0)
	std::cout<<"Processed " << event << " events" << std::endl;

      mctree->GetEntry(event);

      /// Get event level information
      processId = truthEvent->GetProcess();
      truex = truthEvent->GetTrueX();
      truey = truthEvent->GetTrueY();
      trueq2 = truthEvent->GetTrueQ2();
      truenu = truthEvent->GetTrueNu();
      recx = smearEvent->GetX();
      recy = smearEvent->GetY();
      recq2 = smearEvent->GetQ2();
      recnu = smearEvent->GetNu();
 
      TruthEvent trueEvent(*truthEvent);
      trueEvent.setVerbosity(0);
      trueEvent.useBreitFrame(breitFrame);
      exchangeBoson = trueEvent.getExchangeBoson();

      /// Set event level cuts
      trueEvent.setMinQ2(16);
      trueEvent.setMinY(0.05);
      trueEvent.setMaxY(0.95);
      trueEvent.setMinX(0.00001);
      trueEvent.setMinPartPt(0.25);
      trueEvent.setMaxPartEta(3.5);
      
      /// Check the cuts
      if(!trueEvent.passCuts()){
	continue;
      }

      /// Set particle vectors in trueEvent
      trueEvent.processEvent();

      /// Perform truth jet clustering
      PseudoJetVec fjtruthR1Jets = trueEvent.getTruthJets(truthcs, R1jetdef);
      PseudoJetVec fjtruthR1SDJets = trueEvent.getTruthSoftDropJets(fjtruthR1Jets, R1sd);
      
      /// Skip events with no truth jets that pass jet cuts
      if(fjtruthR1Jets.size() == 0)
	{
	  continue;
	}

      SmearedEvent smearedEvent(*truthEvent, *smearEvent);
      smearedEvent.setVerbosity(0);     
      smearedEvent.useBreitFrame(breitFrame);
      smearedEvent.setMaxPartEta(3.5);
      smearedEvent.setMinPartPt(0.25);
      /// Set smeared particle vectors, apply cuts to particles
      smearedEvent.processEvent();

      smearExchangeBoson = smearedEvent.getExchangeBoson();
      matchedParticles = smearedEvent.getMatchedParticles();      
      
      /// Get reco jets and corresponding matched truth jets
      PseudoJetVec fjrecoR1Jets = smearedEvent.getRecoJets(cs, R1jetdef);
      std::vector<PseudoJetVec> fjmatchedR1Jets = 
      	smearedEvent.matchTruthRecoJets(fjtruthR1Jets, fjrecoR1Jets);

      PseudoJetVec fjrecoR1SDJets = 
	smearedEvent.getRecoSoftDropJets(fjrecoR1Jets, R1sd);
      std::vector<PseudoJetVec> fjmatchedR1SDJets = 
	smearedEvent.matchTruthRecoJets(fjtruthR1SDJets, fjrecoR1SDJets);

      /// Everything is in fastjet::PseudoJet right now. Convert to
      /// TLorentzVectors so that they can be written to trees easily
      truthR1Jets  = convertToTLorentzVectors(fjtruthR1Jets, false);
      recoR1Jets   = convertToTLorentzVectors(fjrecoR1Jets, false);
      recoR1SDJets = convertToTLorentzVectors(fjrecoR1SDJets, true);
      truthR1SDJets = convertToTLorentzVectors(fjtruthR1SDJets, true);
      matchedR1Jets = convertMatchedJetVec(fjmatchedR1Jets, false);
      matchedR1SDJets = convertMatchedJetVec(fjmatchedR1SDJets, true);
      
      jetTree->Fill();
    }
  
  /// Write the tree out
  outfile->cd();
  jetTree->Write();
  runTree->Write();
  outfile->Close();
  mc.Close();

  std::cout << "Finished EventLoop" << std::endl;

}

std::vector<std::vector<JetConstPair>> convertMatchedJetVec(
				       std::vector<PseudoJetVec> vec, 
				       bool SDJet)
{
  std::vector<std::vector<JetConstPair>> matchedJets;

  /// Call the convert function for each entry in the vector
  for(int i = 0; i < vec.size(); i++)
    {
      PseudoJetVec pair = vec.at(i);
      JetConstVec TLpair = convertToTLorentzVectors(pair, SDJet);
      
      matchedJets.push_back(TLpair);
    }

  return matchedJets;
}

void setupRunTree()
{
  runTree = new TTree("runTree","A tree with run info");
  runTree->Branch("nEventsTried", &nEventsTried, "nEventsTried/F");
  runTree->Branch("nEventsGen", &nEventsGen, "nEventsGen/F");
  runTree->Branch("totalCrossSection", &totalCrossSection, "totalCrossSection/F");
  runTree->Branch("integratedLumi", &integratedLumi, "integratedLumi/F");

}

void setupJetTree()
{
  jetTree->Branch("processId",&processId,"processId/I");
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
  jetTree->Branch("truenu",&truenu,"truenu/D");
  jetTree->Branch("recx",&recx,"recx/D");
  jetTree->Branch("recy",&recy,"recy/D");
  jetTree->Branch("recq2",&recq2,"recq2/D");
  jetTree->Branch("recnu",&recnu,"recnu/D");
  jetTree->Branch("matchedParticles",&matchedParticles);
  jetTree->Branch("truthR1SDJets", &truthR1SDJets);
  return;
}


JetConstVec convertToTLorentzVectors(PseudoJetVec pseudoJets, bool SDJet)
{
  JetConstVec jets;

  for(int jet = 0; jet < pseudoJets.size(); jet++)
    {
      fastjet::PseudoJet pseudojet = pseudoJets.at(jet);
      
      /// Swap fastjet::pseudojet with a TLorentzVector
      TLorentzVector tJet;
      tJet.SetPxPyPzE(pseudojet.px(),
		      pseudojet.py(),
		      pseudojet.pz(),
		      pseudojet.e());
    
   
      TLorentzVectorVec tConstituents;
      /// If it is SDJets first add the two subjets
      if(SDJet)
	{
	  /// This is size 2 since it has two subjets by definition
	  /// If it is size 0 then no grooming was performed
	  PseudoJetVec subjets = pseudojet.pieces();
	  TLorentzVector subjet1, subjet2;
	  if(subjets.size() != 2)
	    {
	      /// no grooming performed
	      subjet1.SetPxPyPzE(-999,-999,-999,-999);
	      subjet2.SetPxPyPzE(-999,-999,-999,-999);
	    }
	  else
	    {
	      subjet1.SetPxPyPzE(subjets.at(0).px(),
				 subjets.at(0).py(),
				 subjets.at(0).pz(),
				 subjets.at(0).e());
	      subjet2.SetPxPyPzE(subjets.at(1).px(),
				 subjets.at(1).py(),
				 subjets.at(1).pz(),
				 subjets.at(1).e());
	    }
	  
	  tConstituents.push_back(subjet1);
	  tConstituents.push_back(subjet2);
	}

      /// Get jet constituents
      PseudoJetVec constituents = pseudojet.constituents();
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
