

/**
 * Quick and ugly macro to draw some histograms and comparisons between
 * breit frame and lab frame
 */

#include <TH2.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include "../sPhenixStyle.h"
#include "../sPhenixStyle.C"

TGraphErrors *FitProfile(const TH2 *h2);
void drawFitProfile(TH2 *hist, TGraphErrors *gr);
void draw2DHisto(TH2 *hist);
void projectBin(TH2 *hist);

void AnalyzeHistos()
{
  SetsPhenixStyle();
  TFile *file = TFile::Open("lab.root");
  TFile *bfile = TFile::Open("breit.root");
  
  TH2 *truejetptz = (TH2F*)file->Get("truejetptz");
  TH2 *truejetptr = (TH2F*)file->Get("truejetptr");
  TH2 *truejetptjt = (TH2F*)file->Get("truejetptjt");
  TH2 *truenconst = (TH2F*)file->Get("truenconst");
  TH2 *trueSDrg = (TH2F*)file->Get("truthSDjetrg");
  TH2 *trueSDzg = (TH2F*)file->Get("truthSDjetzg");
  
  TH2 *btrueSDrg = (TH2F*)bfile->Get("truthSDjetrg");
  TH2 *btrueSDzg = (TH2F*)bfile->Get("truthSDjetzg");
  TH2 *btruejetptz = (TH2F*)bfile->Get("truejetptz");
  TH2 *btruejetptr = (TH2F*)bfile->Get("truejetptr");
  TH2 *btruejetptjt = (TH2F*)bfile->Get("truejetptjt");
  TH2 *btruenconst = (TH2F*)bfile->Get("truenconst");

  btrueSDrg->SetName("btruthSDjetrg");
  btrueSDzg->SetName("btruthSDjetzg");
  btruenconst->SetName("btruenconst");
  btruejetptz->SetName("btruejetptz");
  btruejetptr->SetName("btruejetptr");
  btruejetptjt->SetName("btruejetptjt");

  TGraphErrors *truenconstfit = FitProfile(truenconst);
  TGraphErrors *btruenconstfit = FitProfile(btruenconst);
  
  draw2DHisto(truejetptz);
  draw2DHisto(truejetptjt);
  draw2DHisto(truejetptr);
  draw2DHisto(btruejetptz);
  draw2DHisto(btruejetptjt);
  draw2DHisto(btruejetptr);
  
  drawFitProfile(truenconst, truenconstfit);
  drawFitProfile(btruenconst, btruenconstfit);

  TH1D *zgproj = trueSDzg->ProjectionX("zgproj",10,11);
  TH1D *bzgproj = btrueSDzg->ProjectionX("bzgproj",10,11);
  TH1D *rgproj = trueSDrg->ProjectionX("rgproj",10,11);
  TH1D *brgproj = btrueSDrg->ProjectionX("brgproj",10,11);

  TCanvas *zgbreitlabcan = new TCanvas("zgbreitlabcan","zgbreitlabcan",
				       200,200,1000,800);
  zgproj->Scale(1./zgproj->GetEntries());
  bzgproj->Scale(1./bzgproj->GetEntries());
  bzgproj->GetXaxis()->SetTitle("z_{g}^{true}");
  bzgproj->GetYaxis()->SetTitle("Normalized Counts");
  bzgproj->SetLineColor(kRed);
  bzgproj->SetMarkerColor(kRed);
  bzgproj->Draw("hist");
  zgproj->Draw("histsame");
  TLegend *leggg = new TLegend(0.7,0.6,0.8,0.8);
  leggg->AddEntry(zgproj,"Lab","L");
  leggg->AddEntry(bzgproj,"Breit","L");
  leggg->AddEntry((TObject*)0,"9<p_{T}^{jet}<10 GeV","");
  leggg->Draw("same");
  
  TCanvas *rgbreitlabcan = new TCanvas("rgbreitlabcan","rgbreitlabcan",
				       200,200,1000,800);
  rgproj->Scale(1./rgproj->GetEntries());
  brgproj->Scale(1./brgproj->GetEntries());
  brgproj->GetXaxis()->SetTitle("R_{g}^{true}");
  brgproj->GetYaxis()->SetTitle("Normalized Counts");
  brgproj->SetLineColor(kRed);
  brgproj->SetMarkerColor(kRed);
  brgproj->Draw("hist");
  rgproj->Draw("histsame");
  leggg->Draw("same");


  TCanvas *zbreitlabcan = new TCanvas("zbreitlabcan","zbreitlabcan",
				      200,200,1000,800);
  TH1D *projection = truejetptz->ProjectionX("zproj",15,16);
  TH1D *bprojection = btruejetptz->ProjectionX("bzproj",15,16);
  
  projection->Scale(1./projection->GetEntries());
  bprojection->Scale(1./bprojection->GetEntries());
  gPad->SetLogx();
  gPad->SetLogy();
  bprojection->GetXaxis()->SetTitle("z");
  bprojection->GetYaxis()->SetTitle("Normalized Counts");
  bprojection->GetXaxis()->SetRangeUser(0.05,1);
  bprojection->SetLineColor(kRed);
  bprojection->Draw("hist");
  projection->Draw("histsame");
  TLegend *legg = new TLegend(0.2,0.2,0.4,0.4);
  legg->AddEntry(projection,"Lab","l");
  legg->AddEntry(bprojection,"Breit","L");
  legg->AddEntry((TObject*)0,"9<p_{T}^{jet,true}<10 GeV","");
  legg->Draw("same");

  TCanvas *nconstcan = new TCanvas("nconstcan","nconstcan",
				   200,200,1000,800);


  truenconstfit->GetXaxis()->SetTitle("p_{T}^{jet, true} [GeV]");
  truenconst->GetYaxis()->SetTitle("N_{const}^{true}");
  truenconstfit->Draw("aep");
  btruenconstfit->SetMarkerColor(kRed);
  btruenconstfit->SetLineColor(kRed);
  btruenconstfit->Draw("epsame");
  TLegend *leg = new TLegend(0.2,0.7,0.3,0.8);
  leg->AddEntry(truenconstfit,"Lab","P");
  leg->AddEntry(btruenconstfit,"Breit","P");
  leg->Draw("same");

}

void projectBin(TH2 *hist)
{
  ostringstream name;
  name.str("");
  name<<"projection_"<<hist->GetName();
  TH1D *projection = hist->ProjectionX(name.str().c_str(),10,11);
  
  TCanvas *can = new TCanvas(name.str().c_str(),name.str().c_str(),
			     200,200,1000,800);
  projection->Scale(1./projection->GetEntries());
  projection->Draw("hist");


}
void draw2DHisto(TH2 *hist)
{
  /// normalize the histo by its bin width
  hist->Scale(1, "width");
  ostringstream name;
  name.str("");
  name<<hist->GetName()<<"_can";
  TCanvas *can = new TCanvas(name.str().c_str(),name.str().c_str(),
			     200,200,1000,800);

  gPad->SetLogz();
  hist->Draw("colz");

}

void drawFitProfile(TH2 *hist, TGraphErrors *gr)
{

  ostringstream name;
  name.str("");
  name<<hist->GetName()<<"_can";
  TCanvas *can = new TCanvas(name.str().c_str(),name.str().c_str(),
			     200,200,1000,800);
  gPad->SetLogz();
  hist->Draw("colz");
  gr->Draw("epsame");


}

TGraphErrors *FitProfile(const TH2 * h2)
{

  TProfile * p2 = h2->ProfileX();

  int n = 0;
  double x[1000];
  double ex[1000];
  double y[1000];
  double ey[1000];

  for (int i = 1; i <= h2->GetNbinsX(); i++)
    {
      TH1D * h1 = h2->ProjectionY(Form("htmp_%d", rand()), i, i);

      if (h1->GetSum() < 30)
        {
          cout << "FitProfile - ignore bin " << i << endl;
          continue;
        }
     

      TF1 fgaus("fgaus", "gaus", -p2->GetBinError(i) * 4,
          p2->GetBinError(i) * 4);

      TF1 f2(Form("dgaus"), "gaus + [3]*exp(-0.5*((x-[1])/[4])**2) + [5]",
          -p2->GetBinError(i) * 4, p2->GetBinError(i) * 4);

      fgaus.SetParameter(1, p2->GetBinContent(i));
      fgaus.SetParameter(2, p2->GetBinError(i));

      h1->Fit(&fgaus, "MQ");

      f2.SetParameters(fgaus.GetParameter(0) / 2, fgaus.GetParameter(1),
          fgaus.GetParameter(2), fgaus.GetParameter(0) / 2,
          fgaus.GetParameter(2) / 4, 0);

      h1->Fit(&f2, "MQ");



      x[n] = p2->GetBinCenter(i);
      ex[n] = (p2->GetBinCenter(2) - p2->GetBinCenter(1)) / 2;
      y[n] = fgaus.GetParameter(1);
      ey[n] = fgaus.GetParError(1);

      n++;
      delete h1;
    }

  return new TGraphErrors(n, x, y, ex, ey);
}

