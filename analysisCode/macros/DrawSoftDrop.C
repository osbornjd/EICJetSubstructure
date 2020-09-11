
void drawJetNConst();
void drawSDJetNConst();
void drawSDAndJetTotalNConst();
void drawSDzComparison();
void drawSDjtComparison();
void drawSDrComparison();
TFile *file;

void DrawSoftDrop()
{
  gStyle->SetOptStat(0);
  file = TFile::Open("data/softDropParam/R1.0z0.1beta0/pE275_pE18_minqsq9_99_breit0.root_histos.root");
  
  drawJetNConst();

  drawSDJetNConst();

  drawSDAndJetTotalNConst();

  drawSDzComparison();

  drawSDjtComparison();
  drawSDrComparison();
}

void drawSDjtComparison()
{
  TH2F *sdjt = (TH2F*)file->Get("trueSDjetptjt");
  TH2F *jetjt = (TH2F*)file->Get("truejetptjt");

  int bin10gev = sdjt->GetYaxis()->FindBin(10.);
  int bin15gev = sdjt->GetYaxis()->FindBin(15.);

  TH1D *sd = sdjt->ProjectionX("sdjt1d",bin10gev,bin15gev);
  TH1D *jet = jetjt->ProjectionX("jetjt1d",bin10gev,bin15gev);

  jet->SetLineColor(kRed);
  jet->SetMarkerColor(kRed);

  jet->Sumw2();
  sd->Sumw2();

  jet->Scale(1,"width");
  sd->Scale(1,"width");
  jet->Scale(1./jet->GetEntries());
  sd->Scale(1./sd->GetEntries());

  TCanvas *can6 = new TCanvas("can6","can6",200,200,800,600);
  can6->cd();
  gPad->SetLogy();
  
  TAxis *x = jet->GetXaxis();
  TAxis *y = jet->GetYaxis();
  x->SetRangeUser(0.001,3);
  x->SetTitle("j_{T} [GeV]");
  y->SetTitle("Arb. Units");
  jet->Draw();
  sd->Draw("psame");

  myText(0.2,0.38,kBlack,"#sqrt{s}=140 GeV");
  myText(0.2,0.32,kBlack, "R=1, z=0.1, #beta=0");
  myText(0.2,0.26,kBlack, "Q^{2}>10 GeV^{2}");
  myText(0.2,0.2, kBlack, "0.05<y<0.95");

  TLegend *leg = new TLegend(0.5,0.7,0.6,0.8);
  leg->AddEntry(jet,"Anti-k_{T}","p");
  leg->AddEntry(sd,"Soft Drop Groomed","P");
  leg->Draw("same");
}

void drawSDrComparison()
{
  TH2F *sdr = (TH2F*)file->Get("trueSDjetptr");
  TH2F *jetr = (TH2F*)file->Get("truejetptr");

  int bin10gev = sdr->GetYaxis()->FindBin(10.);
  int bin15gev = sdr->GetYaxis()->FindBin(15.);

  TH1D *sd = sdr->ProjectionX("sdr1d",bin10gev,bin15gev);
  TH1D *jet = jetr->ProjectionX("jetr1d",bin10gev,bin15gev);

  jet->SetLineColor(kRed);
  jet->SetMarkerColor(kRed);

  jet->Sumw2();
  sd->Sumw2();

  jet->Scale(1,"width");
  sd->Scale(1,"width");
  jet->Scale(1./jet->GetEntries());
  sd->Scale(1./sd->GetEntries());

  TCanvas *can5 = new TCanvas("can5","can5",200,200,800,600);
  can5->cd();
  gPad->SetLogy();
  
  TAxis *x = jet->GetXaxis();
  TAxis *y = jet->GetYaxis();
  x->SetRangeUser(0,1);
  x->SetTitle("r");
  y->SetTitle("Arb. Units");
  jet->Draw();
  sd->Draw("psame");

  myText(0.2,0.38,kBlack,"#sqrt{s}=140 GeV");
  myText(0.2,0.32,kBlack, "R=1, z=0.1, #beta=0");
  myText(0.2,0.26,kBlack, "Q^{2}>10 GeV^{2}");
  myText(0.2,0.2, kBlack, "0.05<y<0.95");

  TLegend *leg = new TLegend(0.5,0.75,0.6,0.85);
  leg->AddEntry(jet,"Anti-k_{T}","p");
  leg->AddEntry(sd,"Soft Drop Groomed","P");
  leg->Draw("same");
}
void drawSDzComparison()
{

  TH2F *sdz = (TH2F*)file->Get("trueSDjetptz");
  TH2F *jetz = (TH2F*)file->Get("truejetptz");

  int bin10gev = sdz->GetYaxis()->FindBin(10.);
  int bin15gev = sdz->GetYaxis()->FindBin(15.);

  TH1D *sd = sdz->ProjectionX("sdz1d",bin10gev,bin15gev);
  TH1D *jet = jetz->ProjectionX("jet1d",bin10gev,bin15gev);

  jet->SetLineColor(kRed);
  jet->SetMarkerColor(kRed);

  jet->Sumw2();
  sd->Sumw2();

  jet->Scale(1,"width");
  sd->Scale(1,"width");
  jet->Scale(1./jet->GetEntries());
  sd->Scale(1./sd->GetEntries());

  TCanvas *can4 = new TCanvas("can4","can4",200,200,800,600);
  can4->cd();
  gPad->SetLogy();
  
  TAxis *x = jet->GetXaxis();
  TAxis *y = jet->GetYaxis();
  
  x->SetTitle("z");
  y->SetTitle("Arb. Units");
  jet->Draw();
  sd->Draw("psame");

  myText(0.2,0.38,kBlack,"#sqrt{s}=140 GeV");
  myText(0.2,0.32,kBlack, "R=1, z=0.1, #beta=0");
  myText(0.2,0.26,kBlack, "Q^{2}>10 GeV^{2}");
  myText(0.2,0.2, kBlack, "0.05<y<0.95");

  TLegend *leg = new TLegend(0.5,0.7,0.6,0.8);
  leg->AddEntry(jet,"Anti-k_{T}","p");
  leg->AddEntry(sd,"Soft Drop Groomed","P");
  leg->Draw("same");

}

void drawSDAndJetTotalNConst()
{
  TH2F *jet = (TH2F*)file->Get("truenconst");
  TH2F *sd = (TH2F*)file->Get("trueSDnconst");
  
  TH1D *jetq = jet->QuantilesX(0.5,"jetq");
  TH1D *sdjetq = sd->QuantilesX(0.5,"sdjetq");
  
  jetq->SetLineColor(kRed);
  jetq->SetMarkerColor(kRed);
  
  TCanvas *can3 = new TCanvas("can3","can3",200,200,800,600);
  can3->cd();
  
  TAxis *x = jetq->GetXaxis();
  TAxis *y = jetq->GetYaxis();
  
  x->SetTitle("p_{T} [GeV]");
  y->SetTitle("N_{const}");
  x->SetRangeUser(5,35);
  y->SetRangeUser(2,15);

  jetq->Draw();
  sdjetq->Draw("psame");

  myText(0.2,0.83,kBlack,"#sqrt{s}=140 GeV");
  myText(0.2,0.77,kBlack, "R=1, z=0.1, #beta=0");
  myText(0.2,0.71,kBlack, "Q^{2}>10 GeV^{2}");
  myText(0.2,0.65, kBlack, "0.05<y<0.95");

  TLegend *leg = new TLegend(0.5,0.2,0.6,0.3);
  leg->AddEntry(jetq,"Anti-k_{T}","p");
  leg->AddEntry(sdjetq,"Soft Drop Groomed","P");
  leg->Draw("same");

}
void drawSDJetNConst()
{

  TH2F *lept = (TH2F*)file->Get("h_SDjetLepton");
  TH2F *chg = (TH2F*)file->Get("h_SDjetChargedHadron");
  TH2F *neut = (TH2F*)file->Get("h_SDjetNeutralHadron");
  TH2F *phot = (TH2F*)file->Get("h_SDjetPhoton");
  
  TH1D *sdlept = lept->QuantilesX(0.5,"sdlepx");
  TH1D *sdchg = chg->QuantilesX(0.5,"sdchgx");
  TH1D *sdneut = neut->QuantilesX(0.5,"sdneutx");
  TH1D *sdphot = phot->QuantilesX(0.5,"sdphotx");
  
  TCanvas *can2 = new TCanvas("can2","can2",200,200,800,600);
  can2->cd();
  
  sdneut->SetMarkerColor(kBlue);
  sdphot->SetMarkerColor(kRed);
  sdneut->SetLineColor(kBlue);
  sdphot->SetLineColor(kRed);

  TAxis *x = sdchg->GetXaxis();
  TAxis *y = sdchg->GetYaxis();
  
  x->SetTitle("p_{T} [GeV]");
  y->SetTitle("N_{const}");
  x->SetRangeUser(5,35);
  y->SetRangeUser(0,12);
  sdchg->Draw("p");
  sdneut->Draw("psame");
  sdphot->Draw("psame");
  
  myText(0.2,0.83,kBlack,"#sqrt{s}=140 GeV");
  myText(0.2,0.77,kBlack, "R=1, z=0.1, #beta=0");
  myText(0.2,0.71,kBlack, "Q^{2}>10 GeV^{2}");
  myText(0.2,0.65, kBlack, "0.05<y<0.95");

  TLegend *leg = new TLegend(0.5,0.65,0.6,0.8);
  leg->AddEntry(sdchg,"Charged hadrons","P");
  leg->AddEntry(sdneut,"Neutral hadrons","P");
  leg->AddEntry(sdphot,"Photons","P");
  leg->Draw("same");

}
void drawJetNConst()
{

  TH2F *lept = (TH2F*)file->Get("h_jetLepton");
  TH2F *chg = (TH2F*)file->Get("h_jetChargedHadron");
  TH2F *neut = (TH2F*)file->Get("h_jetNeutralHadron");
  TH2F *phot = (TH2F*)file->Get("h_jetPhoton");
  
  TH1D *avglept = lept->QuantilesX(0.5,"lepx");
  TH1D *avgchg = chg->QuantilesX(0.5,"chgx");
  TH1D *avgneut = neut->QuantilesX(0.5,"neutx");
  TH1D *avgphot = phot->QuantilesX(0.5,"photx");
  
  TCanvas *can = new TCanvas("can","can",200,200,800,600);
  can->cd();
  
  avgneut->SetMarkerColor(kBlue);
  avgphot->SetMarkerColor(kRed);
  avgneut->SetLineColor(kBlue);
  avgphot->SetLineColor(kRed);

  TAxis *x = avgchg->GetXaxis();
  TAxis *y = avgchg->GetYaxis();
  
  x->SetTitle("p_{T} [GeV]");
  y->SetTitle("N_{const}");
  x->SetRangeUser(5,35);
  y->SetRangeUser(0,12);
  avgchg->Draw("p");
  avgneut->Draw("psame");
  avgphot->Draw("psame");
  
  myText(0.2,0.83,kBlack,"#sqrt{s}=140 GeV");
  myText(0.2,0.77,kBlack, "R=1 Anti-k_{T}");
  myText(0.2,0.71,kBlack, "Q^{2}>10 GeV^{2}");
  myText(0.2,0.65, kBlack, "0.05<y<0.95");

  TLegend *leg = new TLegend(0.5,0.65,0.6,0.8);
  leg->AddEntry(avgchg,"Charged hadrons","P");
  leg->AddEntry(avgneut,"Neutral hadrons","P");
  leg->AddEntry(avgphot,"Photons","P");
  leg->Draw("same");

}
