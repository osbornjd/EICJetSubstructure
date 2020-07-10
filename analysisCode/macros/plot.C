{
  //Please label the plot with indicating the particle ID. Normalize your counts to an integrated luminosity of 10 fb-1

  TString title("Electron Momentum vs Theta (#intL=10 fb^{-1} )"); // Title of your plot

  //TFile *f=new TFile("data/lab/pE275_pE18_minqsq9_9_breit0.root_histos.root");
  TFile *f = new TFile("data/lab_minpt250.root");

  //Please keep the following binning so that we can compare across different WG
  TH2D *h=(TH2D*)f->Get("h_scatLept");

  gStyle->SetOptStat(0);
  gStyle->SetTitleX(.5);
  gStyle->SetTitleY(.87);

  TH2F *hdummy=new TH2F("hdummy",title.Data(),10,-50.,70., 10, 0., 70.);

  Double_t xtit = h->GetYaxis()->GetTitleSize();
  xtit=xtit*1.;
  
  Double_t maxy = h->GetMaximum()*1.1;
  Double_t miny = h->GetMinimum(0.);
  
  hdummy->GetZaxis()->SetRangeUser(miny,maxy*100);
  hdummy->GetYaxis()->SetTitleSize(xtit);
  hdummy->GetXaxis()->SetTitleSize(xtit);
  
  hdummy->GetYaxis()->SetLabelSize(xtit);
  hdummy->GetXaxis()->SetLabelSize(xtit);
  
   hdummy->GetYaxis()->SetTitle("Momentum (GeV)  ");
  hdummy->GetXaxis()->SetTitle("Momentum (GeV)  ");
  
  hdummy->GetXaxis()->SetTitleOffset(1.2);     
  hdummy->GetYaxis()->SetTitleOffset(1.3);
  hdummy->SetLineColor(1);
  hdummy->SetLineStyle(1);
  hdummy->SetLineWidth(2);
  //hdummy->Scale(10./0.12);
  hdummy->DrawCopy("AXIS");
  delete hdummy;

  gPad->SetRightMargin(0.15);
  gPad->SetBottomMargin(0.15);
  gPad->SetLogz();
  h->Scale(10./0.12);
  h->DrawCopy("POL COLZ SAME");


  // ---- Output -------
  /*
  TFile *fout=new TFile("output_plot.root","recreate");
  h->Write(title.Data());
  gPad->Write(title.Data());
  title+=".gif";
  gPad->Print(title.Data());
  */
}
