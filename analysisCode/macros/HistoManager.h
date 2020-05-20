/*
 * A separate header file to keep all of the histogram clutter away from
 * the actual source code
 */

TH2 *truerecoz, *truerecojt, *truerecor;
TH2 *recojetpteta, *recojetptphi;
TH2 *truerecx, *truerecy, *truerecq2;
TH2 *trueQ2x, *trueQ2pT;
TH2 *truejetpteta, *truejetptphi;
TH2 *recojetptz, *recojetptjt, *recojetptr;
TH2 *truejetptz, *truejetptjt, *truejetptr;
TH2 *recojetptetatruejetpt;
TH2 *recotruejetpt, *recotruejeteta, *recotruejetphi, 
  *recotruejetp, *recotruejete;
TH1 *matchedJetDr, *matchedJetdEta, *matchedJetdPhi;
TH1 *truthRecoConstdPhi, *truthRecoConstdEta, *truthRecoConstdRap;
TH2 *truthSDjetzg, *truthSDjetrg;
TH2 *recoSDjetzg, *recoSDjetrg;
TH2 *truthrecozg, *truthrecorg;
TH1 *sdenergygroomed, *truthrecosdjetdeltar;

void write()
{
  outfile = new TFile("histos.root","RECREATE");
  sdenergygroomed->Write();
  truthSDjetzg->Write();
  truthSDjetrg->Write();
  recoSDjetzg->Write();
  recoSDjetrg->Write();
  truthrecozg->Write();
  truthrecorg->Write();

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
  truejetptr->Write();
  truejetptjt->Write();
  truejetptz->Write();
  recojetpteta->Write();
  recojetptphi->Write();
  recotruejetpt->Write();
  recotruejetphi->Write();
  recotruejeteta->Write();
  recotruejetp->Write();
  recotruejete->Write();
  
  truerecoz->Write();
  truerecojt->Write();
  truerecor->Write();
  matchedJetDr->Write();
  matchedJetdPhi->Write();
  matchedJetdEta->Write();
  truthRecoConstdPhi->Write();
  truthRecoConstdEta->Write();
  truthRecoConstdRap->Write();
  recojetptetatruejetpt->Write();
  truthrecosdjetdeltar->Write();

  outfile->Write();
  outfile->Close();

}
void instantiateHistos()
{
  for(int i = 0; i< nzgbins+1; i++)
    {
      zgbins[i] = 0 + i * 0.5 / nzgbins;
    }

  truthrecosdjetdeltar = new TH1F("truthrecosdjetdeltar",";#DeltaR(truth,reco)",
				  120,0,1.2);
  sdenergygroomed = new TH1F("sdenergygroomed",";E_{SD}/E_{AKT}",101,0,1.01);
  truthrecozg = new TH2F("truthrecozg",";z_{g}^{true}; z_{g}^{reco}",
			 nzgbins, zgbins, nzgbins, zgbins);
  truthrecorg = new TH2F("truthrecorg",";R_{g}^{true}; R_{g}^{reco}",
			 nrbins, rbins, nrbins, rbins);
  truthSDjetzg = new TH2F("truthSDjetzg",
			  ";z_{g}^{true};p_{T}^{Soft Drop, true} [GeV]",
			  nzgbins,zgbins, nptbins, ptbins);
  truthSDjetrg = new TH2F("truthSDjetrg",
			  ";R_{g}^{true};p_{T}^{Soft Drop, true} [GeV]",
			  nrbins,rbins,nptbins,ptbins);
  recoSDjetzg = new TH2F("recoSDjetzg",
			  ";z_{g}^{reco};p_{T}^{Soft Drop, reco} [GeV]",
			  nzgbins,zgbins, nptbins, ptbins);
  recoSDjetrg = new TH2F("recoSDjetrg",
			  ";R_{g}^{reco};p_{T}^{Soft Drop, reco} [GeV]",
			  nrbins,rbins,nptbins,ptbins);



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
  truejetptz = new TH2F("truejetptz",";z_{true};p_{T}^{jet,true} [GeV]",
			nzbins,zbins,nptbins,ptbins);
  truejetptjt = new TH2F("truejetptjt",
			 ";j_{T}^{true} [GeV]; p_{T}^{jet,true} [GeV]",
			 njtbins,jtbins,nptbins,ptbins);
  truejetptr = new TH2F("truejetptr",";r_{true}; p_{T}^{jet,true} [GeV]",
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

  truerecoz = new TH2F("truerecoz",";z_{true};z_{reco}",nzbins,zbins,nzbins,zbins);
  truerecojt = new TH2F("truerecojt",";j_{T}^{true} [GeV];j_{T}^{reco} [GeV]",njtbins,jtbins,njtbins,jtbins);
  truerecor = new TH2F("truerecor",";r_{true}; r_{reco}",nrbins,rbins,nrbins,rbins);
  matchedJetDr = new TH1F("matchedJetDr",";#DeltaR(true,reco)",100,0,1.1);
  matchedJetdEta = new TH1F("matchedJetdEta",";#Delta#eta(true,reco)",100,-0.5,0.5);
  matchedJetdPhi = new TH1F("matchedJetdPhi",";#Delta#phi(true,reco) [rad]",100,-0.5,0.5);

  truthRecoConstdPhi = new TH1F("truthRecoConstdPhi",";#Delta#phi(true_{const},reco_{const}) [rad]", 100,-0.5,0.5);
  truthRecoConstdEta = new TH1F("truthRecoConstdEta",";#Delta#eta(true_{const}, reco_{const})",100,-0.5,0.5);
  truthRecoConstdRap = new TH1F("truthRecoConstdRap",";#Deltay(true_{const}, reco_{const})",100,-0.5,0.5);
  recojetptetatruejetpt = new TH2F("recojetptetatruejetpt",";p_{T}^{jet,true} [GeV]; #eta^{jet,true}",30,4,34,50,-3,3);
}
