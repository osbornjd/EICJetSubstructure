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
  SoftDropJetDef R1sd(0.1, 0, R1jetdef.getR());
  
  TH2F *truthMaps[1000];
  TH2F *recoMaps[1000];
  TGraph *scategr[1000];
  TGraph *tjetgr[1000];
  ostringstream name;
  for(int i =0; i<1000; i++)
    {
      name.str("");
      name<<"truthMap_"<<i;
      truthMaps[i] = new TH2F(name.str().c_str(),"",200,-3.14159,3.14159,
			      200,-3.5,3.5);
      name.str("");
      name<<"recoMap_"<<i;
      recoMaps[i] = new TH2F(name.str().c_str(),"",100,-3.14159,3.14159,
			     100,-3.5,3.5);
      
    }
  int counter = 0;
  std::cout<<"begin event loop"<<std::endl;
  for(int event = 0; event < mctree->GetEntries(); ++event)
    {
      if(event % 20000 == 0)
	std::cout<<"Processed " << event << " events" << std::endl;
      if(counter == 1000)
	break;
      mctree->GetEntry(event);

      truex = truthEvent->GetTrueX();
      truey = truthEvent->GetTrueY();
      trueq2 = truthEvent->GetTrueQ2();
      truenu = truthEvent->GetTrueNu();

      TruthEvent trueEvent(*truthEvent);
      trueEvent.setVerbosity(0);
      trueEvent.useBreitFrame(breitFrame);
      exchangeBoson = trueEvent.getExchangeBoson();

      /// Set event level cuts
      trueEvent.setMinQ2(16);
      trueEvent.setMinY(0.05);
      trueEvent.setMaxY(0.95);
      trueEvent.setMinX(0.00001);
      /// Check the cuts
      if(!trueEvent.passCuts()){
	continue;
      }
      
    
      
      recx = smearEvent->GetX();
      recy = smearEvent->GetY();
      recq2 = smearEvent->GetQ2();
      recnu = smearEvent->GetNu();
 
      trueEvent.processEvent();

      PseudoJetVec fjtruthR1Jets = trueEvent.getTruthJets(truthcs, R1jetdef);
      PseudoJetVec fjtruthR1SDJets = trueEvent.getTruthSoftDropJets(fjtruthR1Jets, R1sd);
      if(fjtruthR1Jets.size() == 0)
	{
	  continue;
	}


      if(counter<1000){
	scategr[counter] = fillTruthMaps(truthEvent, truthMaps[counter]);
	//fillRecoMaps(smearEvent, recoMaps[counter]);
	ostringstream namer;
	namer.str("");
	namer<<"truthMapGr_"<<counter;
	scategr[counter]->SetName(namer.str().c_str());
	scategr[counter]->SetMarkerStyle(24);
	scategr[counter]->SetMarkerSize(2.1);
	float highestpt = 0;
	int num = 0;
	for(int jet = 0; jet < fjtruthR1Jets.size(); jet++)
	  {
	    if(fjtruthR1Jets.at(jet).pt() > highestpt){
	      highestpt = fjtruthR1Jets.at(jet).pt();
	      num = jet;
	    }
	  }
	float jetval[1],jetval2[1];
	jetval[0] = fjtruthR1Jets.at(num).phi();
	jetval2[0] = fjtruthR1Jets.at(num).eta();
	tjetgr[counter] = new TGraph(1,jetval,jetval2);
	namer.str("");
	namer<<"truthMapJetGr_"<<counter;
	tjetgr[counter]->SetName(namer.str().c_str());
	tjetgr[counter]->SetMarkerStyle(24);
	tjetgr[counter]->SetMarkerColor(kRed);
	tjetgr[counter]->SetMarkerSize(2.1);
	counter++;
      }


      SmearedEvent smearedEvent(*truthEvent, *smearEvent);
      smearedEvent.setVerbosity(0);     
      smearedEvent.useBreitFrame(breitFrame);
      smearedEvent.processEvent();

      smearExchangeBoson = smearedEvent.getExchangeBoson();
      matchedParticles = smearedEvent.getMatchedParticles();      

      PseudoJetVec fjrecoR1Jets = smearedEvent.getRecoJets(cs, R1jetdef);
      std::vector<PseudoJetVec> fjmatchedR1Jets = 
      	smearedEvent.matchTruthRecoJets(fjtruthR1Jets, fjrecoR1Jets);

      PseudoJetVec fjrecoR1SDJets = 
	smearedEvent.getRecoSoftDropJets(fjrecoR1Jets, R1sd);
      std::vector<PseudoJetVec> fjmatchedR1SDJets = 
	smearedEvent.matchTruthRecoJets(fjtruthR1SDJets, fjrecoR1SDJets);

      truthR1Jets  = convertToTLorentzVectors(fjtruthR1Jets, false);
      recoR1Jets   = convertToTLorentzVectors(fjrecoR1Jets, false);

      recoR1SDJets = convertToTLorentzVectors(fjrecoR1SDJets, true);
      truthR1SDJets = convertToTLorentzVectors(fjtruthR1SDJets, true);

      matchedR1Jets = convertMatchedJetVec(fjmatchedR1Jets, false);
      matchedR1SDJets = convertMatchedJetVec(fjmatchedR1SDJets, true);
      
      jetTree->Fill();
    }
  
  outfile->cd();
  jetTree->Write();
  for(int i =0; i<1000; i++)
    {
      scategr[i]->Write();
      truthMaps[i]->Write();
      tjetgr[i]->Write();
      //recoMaps[i]->Write();
    }
  outfile->Close();
  mc.Close();

  std::cout << "Finished EventLoop" << std::endl;

}

std::vector<std::vector<JetConstPair>> convertMatchedJetVec(std::vector<PseudoJetVec> vec, bool SDJet)
{
  std::vector<std::vector<JetConstPair>> matchedJets;
  //num jets per event
  for(int i = 0; i < vec.size(); i++)
    {
      PseudoJetVec pair = vec.at(i);
      JetConstVec TLpair = convertToTLorentzVectors(pair, SDJet);
      
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
      
      /// swap fastjet::pseudojet with a TLorentzVector
      TLorentzVector tJet;
      tJet.SetPxPyPzE(pseudojet.px(),
		      pseudojet.py(),
		      pseudojet.pz(),
		      pseudojet.e());
    
   
      TLorentzVectorVec tConstituents;
      /// If it is SDJets first add the two subjets
      if(SDJet)
	{
	  /// This is always size 2 since it has two subjets by definition
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


TGraph* fillTruthMaps(erhic::EventPythia* event, TH2F *hist)
{
  BreitFrame breit(*event);
  for(int i = 0; i < event->GetNTracks(); ++i)
    {
      if(i < 3 )
	continue;
      
      const Particle *truthParticle = event->GetTrack(i);

      if(truthParticle->GetStatus() != 1)
	continue;
      
      if(fabs(truthParticle->GetEta()) > 3.5)
	continue;
      if(truthParticle->GetPt() < 0.25)
	continue;

      TLorentzVector vector = truthParticle->Get4Vector();
      float phi = vector.Phi();
      float eta = vector.Eta();
      float pt = vector.Pt();
      
      int binx = hist->GetXaxis()->FindBin(phi);
      int biny = hist->GetYaxis()->FindBin(eta);
      hist->SetBinContent(binx,biny,pt);
    }
  
  TLorentzVector scat = event->ScatteredLepton()->Get4Vector();
  float scatphi[1], scateta[1];
  scatphi[0] = scat.Phi();
  scateta[0] = scat.Eta();
  TGraph *gr = new TGraph(1,scatphi,scateta);

  return gr;
}

TGraph* fillRecoMaps(Smear::Event* event, TH2F *hist)
{

  return nullptr;
}
