



void drawMap()
{
  const int ngr=4;
  TFile *file = TFile::Open("outfileb.root");
  TH2F *map[ngr];
  TGraph *jet[ngr];
  TGraph *elec[ngr];
  TCanvas *can[ngr];
  
  ostringstream name;
  for(int i=0;i<ngr; i++)
    {
      name.str("");
      name<<"truthMap_"<<i;
      map[i] = (TH2F*)file->Get(name.str().c_str());
      
      name.str("");
      name<<"truthMapGr_"<<i;
      elec[i]=(TGraph*)file->Get(name.str().c_str());
      
      name.str("");
      name<<"truthMapJetGr_"<<i;
      jet[i] = (TGraph*)file->Get(name.str().c_str());

      name.str("");
      name<<"can_"<<i;
      can[i] = new TCanvas(name.str().c_str(), name.str().c_str(),
			   200,200,800,800);
      gStyle->SetOptStat(0);
      if(jet[i]->GetPointX(0) > 3.14159)
	jet[i]->SetPointX(0,jet[i]->GetPointX(0) - 2.*TMath::Pi());
      if(jet[i]->GetPointX(0) < -3.14159)
	jet[i]->SetPointX(0,jet[i]->GetPointX(0) + 2.*TMath::Pi());

      map[i]->Draw("colz");
      jet[i]->Draw("psame");
      elec[i]->Draw("psame");
	
    }

  


}
