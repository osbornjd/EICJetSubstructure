
#include "analyzeJets.h"


void analyzeJets()
{
  std::string filename = "pE275_pE18_minqsq9_breit.root";
  infile = TFile::Open(filename.c_str());

  /// If filename contains breit
  if(filename.find("breit") != string::npos)
    {

      // Swap the q binning around, since breit frame specifically boosts
      // q to be -pz
      float dummybins[nq2bins+1];
      for(int i = 0; i < nq2bins+1; i++)
	{
	  dummybins[i] = qbins[i];
	}
      
      for(int i = 0; i < nq2bins+1; i++)
	{
	  qbins[i] = dummybins[nq2bins-i] * -1;
	}
    }
  
  gROOT->ProcessLine(".L ../src/fastJetLinker.C+");
  setupTree();

  instantiateHistos();

  loop();

  write();
}

void write()
{
  outfile = new TFile("histos.root","RECREATE");
  
  truerecx->Write();
  truerecy->Write();
  truerecq2->Write();
  trueQ2x->Write();
  
  trueQ2pT->Write();
  truejetpteta->Write();
  truejetptphi->Write();
  recojetptr->Write();
  recojetptjt->Write();
  recojetptz->Write();
  recojetpteta->Write();
  recojetptphi->Write();
  recotruejetpt->Write();
  recotruejetphi->Write();
  recotruejeteta->Write();
  recotruejetp->Write();
  recotruejete->Write();

  outfile->Write();

  outfile->Close();

}
void instantiateHistos()
{
  
  truerecx = new TH2F("truerecx",";x_{true}; x_{rec}",nxbins, xbins, 
		      nxbins, xbins);
  truerecy = new TH2F("truerecy",";y_{true}; y_{rec}",100,0,1,100,0,1);
  truerecq2 = new TH2F("truerecq2",
		       ";Q^{2}_{true} [GeV^{2}]; Q^{2}_{reco} [GeV^{2}]",
		       nq2bins,qbins,nq2bins,qbins);
  trueQ2x = new TH2F("trueq2x",";x_{true};Q^{2}_{true} [GeV^{2}]",
		     nxbins,xbins,nq2bins,qbins);
  
  trueQ2pT = new TH2F("trueq2pt",";Q_{true}^{2} [GeV^{2}]; p_{T}^{true} [GeV]",
		      nq2bins,qbins,nptbins,ptbins);
  truejetpteta = new TH2F("truejetpteta",";p_{T}^{true} [GeV]; #eta",
			  30,4,34,50,-3,3);
  truejetptphi = new TH2F("truejetptphi",";p_{T}^{true} [GeV]; #phi [rad]",
			  30,4,34,50,-3.14159,3.14159);
  recojetptz = new TH2F("recojetptz",";z_{reco};p_{T}^{jet,reco} [GeV]",
			nzbins,zbins,nptbins,ptbins);
  recojetptjt = new TH2F("recojetptjt",
			 ";j_{T}^{reco} [GeV]; p_{T}^{jet,reco} [GeV]",
			 njtbins,jtbins,nptbins,ptbins);
  recojetptr = new TH2F("recojetptr",";r_{reco}; p_{T}^{jet,reco} [GeV]",
			nrbins,rbins,nptbins,ptbins);
  recojetpteta = new TH2F("recojetpteta", ";p_{T}^{jet,reco} [GeV]; #eta",
			  30,4,34,50,-3,3);
  recojetptphi = new TH2F("recojetptphi",";p_{T}^{jet,reco} [GeV]; #phi",
			  30,4,34,50,-3.14159,3.14159);
  recotruejetpt = new TH2F("recotruejetpt",";p_{T}^{jet,true} [GeV]; p_{T}^{jet,reco} [GeV]",
			   nptbins, ptbins, nptbins, ptbins);
  recotruejetphi = new TH2F("recotruejetphi",";#phi_{true} [rad];#phi_{reco} [rad]",
			    200, -3.14159,3.14159, 200,-3.14159, 3.14159);
  recotruejeteta = new TH2F("recotruejeteta",";#eta_{true}; #eta_{reco}",
			    200,-3,3, 200,-3,3);
  recotruejetp = new TH2F("recotruejetp",";p^{jet,true} [GeV];p^{jet,reco} [GeV]", npbins, pbins, npbins, pbins);
  recotruejete = new TH2F("recotruejete",";E^{jet,true} [GeV]; E^{jet,reco} [GeV]", npbins, pbins, npbins, pbins);


}

void recoJetAnalysis(JetConstVec *recojets)
{

  for(int i = 0; i < recojets->size(); i++)
    {
      TLorentzVector jet;
      jet = recojets->at(i).first;

      float jetpt = jet.Pt();
      recojetpteta->Fill(jetpt, jet.Eta());
      recojetptphi->Fill(jetpt, jet.Phi());

      TVector3 jet3;
      jet3.SetXYZ(jet.Px(), jet.Py(), jet.Pz());

      /// Iterate over constituents
      for(int j = 0; j < recojets->at(i).second.size(); ++j)
	{
	  TLorentzVector con = recojets->at(i).second.at(j);
	  TVector3 con3;
	  con3.SetXYZ(con.Px(), con.Py(), con.Pz());
	  TVector3 cross = jet3.Cross(con3);
	  
	  float z = jet3.Dot(con3) / (jet3.Mag2());
	  float jt = cross.Mag() / jet3.Mag();
	  float r = sqrt(pow(jet.Phi() - con.Phi(), 2) + pow(jet.Rapidity() - con.Rapidity(),2));

	  recojetptz->Fill(z, jetpt);
	  recojetptjt->Fill(jt, jetpt);
	  recojetptr->Fill(r, jetpt);

	}
    }
}

double truthJetAnalysis(JetConstVec *truthjets)
{

  double jetpt = 0;
  for(int jet = 0; jet < truthJets->size(); jet++)
    {
      TLorentzVector jetVec;
      jetVec = truthJets->at(jet).first;
      if(jetVec.Pt() > jetpt)
	jetpt = jetVec.Pt();
      
      if(jetVec.Pt() < 4)
	continue;
      if(fabs(jetVec.Eta()) > 2)
	continue;
      
      truejetptphi->Fill(jetVec.Pt(), jetVec.Phi());
      truejetpteta->Fill(jetVec.Pt(), jetVec.Eta());
      
      for(int con = 0; con < truthJets->at(jet).second.size(); ++con)
	{
	  TLorentzVector constituent;
	  constituent = truthJets->at(jet).second.at(con);
	  
	} 
    }

  return jetpt;
}

void loop()
{

  for(int i=0; i<jettree->GetEntries(); i++)
    {
      if(i%10000 == 0)
	std::cout << "Processed " << i << " events " << std::endl;
      jettree->GetEntry(i);
      
      recoJetAnalysis(recoJets);

      float highestTruthJetPt = truthJetAnalysis(truthJets);

      analyzeMatchedJets(matchedJets);

      /// Event level kinematics
      truerecx->Fill(truex,recx);
      truerecy->Fill(truey,recy);
      truerecq2->Fill(trueq2,recq2);
      trueQ2x->Fill(truex,trueq2);
  
      trueQ2pT->Fill(truthExchangeBoson->Mag2(),highestTruthJetPt);
      

    }

}

void analyzeMatchedJets(MatchedJets *matchedjets)
{
  for(int i = 0; i < matchedjets->size(); i++)
    {
      JetConstPair truthJetConst = matchedjets->at(i).at(0);
      JetConstPair recoJetConst = matchedJets->at(i).at(1);

      TLorentzVector truthJet = truthJetConst.first;
      TLorentzVector recoJet = recoJetConst.first;
      TLorentzVectorVec truthConst = truthJetConst.second;
      TLorentzVectorVec recoConst = recoJetConst.second;
      
      recotruejetpt->Fill(truthJet.Pt(), recoJet.Pt());
      recotruejeteta->Fill(truthJet.Eta(), recoJet.Eta());
      recotruejetphi->Fill(truthJet.Phi(), recoJet.Phi());
      
      recotruejetp->Fill(truthJet.P(), recoJet.P());
      recotruejete->Fill(truthJet.E(), recoJet.E());
    }

}

void setupTree()
{

  jettree = (TTree*)infile->Get("jettree");
  jettree->SetBranchAddress("recx", &recx);
  jettree->SetBranchAddress("recq2", &recq2);
  jettree->SetBranchAddress("recy", &recy);
  jettree->SetBranchAddress("truex", &truex);
  jettree->SetBranchAddress("truey", &truey);
  jettree->SetBranchAddress("trueq2", &trueq2);
  jettree->SetBranchAddress("truthR1Jets", &truthJets);
  jettree->SetBranchAddress("recoR1Jets", &recoJets);
  jettree->SetBranchAddress("recoR1SDJets", &recoSDJets);
  jettree->SetBranchAddress("matchedR1Jets", &matchedJets);
  jettree->SetBranchAddress("matchedR1SDJets", &matchedSDJets);
  jettree->SetBranchAddress("exchangeBoson", &truthExchangeBoson);


}
