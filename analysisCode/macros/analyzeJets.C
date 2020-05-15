
#include "analyzeJets.h"


void analyzeJets()
{

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

  outfile->Write();

  outfile->Close();

}
void instantiateHistos()
{
  
  truerecx = new TH2F("truerecx",";x_{true}; x_{rec}",nxbins, xbins, nxbins, xbins);
  truerecy = new TH2F("truerecy",";y_{true}; y_{rec}",100,0,1,100,0,1);
  truerecq2 = new TH2F("truerecq2",";Q^{2}_{true} [GeV^{2}]; Q^{2}_{reco} [GeV^{2}]",nq2bins,qbins,nq2bins,qbins);
  trueQ2x = new TH2F("trueq2x",";x_{true};Q^{2}_{true} [GeV^{2}]",nxbins,xbins,nq2bins,qbins);
  
  trueQ2pT = new TH2F("trueq2pt",";Q_{true}^{2} [GeV^{2}]; p_{T}^{true} [GeV]",nq2bins,qbins,nptbins,ptbins);
  truejetpteta = new TH2F("truejetpteta",";p_{T}^{true} [GeV]; #eta",20,4,24,50,-3,3);
  truejetptphi = new TH2F("truejetptphi",";p_{T}^{true} [GeV]; #phi [rad]",20,4,24,50,-3.14159,3.14159);
}


void loop()
{

  for(int i=0; i<jettree->GetEntries(); i++)
    {
      if(i%10000 == 0)
	std::cout << "Processed " << i << " events " << std::endl;
      jettree->GetEntry(i);
      
      for(int jet = 0; jet < recoJets->size(); jet++)
	{

	}

      float jetpt = 0;
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
      
      truerecx->Fill(truex,recx);
      truerecy->Fill(truey,recy);
      truerecq2->Fill(trueq2,recq2);
      trueQ2x->Fill(truex,trueq2);
      trueQ2pT->Fill(trueq2,jetpt);
      

    }

}



void setupTree()
{
  infile = TFile::Open("pE275_pE18_minqsq9_lab.root");

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
