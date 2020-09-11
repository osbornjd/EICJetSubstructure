/*
 * A separate header file to keep all of the histogram clutter away from
 * the actual source code
 */

TLorentzPairVec *matchedParticles;
TLorentzVector *truthExchangeBoson, *smearedExchangeBoson, *scatteredLepton;
JetConstVec *truthJets, *recoJets, *recoSDJets, *truthSDJets;
MatchedJets *matchedJets, *matchedSDJets;
double recx, recy, recq2, truey, truex, trueq2;
float lumi;
int processId;

TFile *infile, *outfile;
TTree *jettree;

TH2 *truthsubjetpt, *recosubjetpt;
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
TH2 *truereconconst;
TH2 *truenconst, *reconconst;
TH1 *matchconstp, *matchconstpt;
TH2 *recomatchdr;
TH1 *nrecojets, *ntruthjets;
TH2 *truthjetbosonphi, *truthjetbosontheta, *truthjetbosoneta;
TH2 *truejetpttheta, *truejetptheta, *h_scatLept;
TH2 *h_scatJet;
TH1 *h_lumi, *h_eventsGen, *h_xsec;
TH1 *jes[npbins];
TH1 *h_processID;
TH2 *h_constpT;

TH2 *h_jetChargedHadron, *h_jetNeutralHadron, *h_jetPhoton, *h_jetLepton;
TH1 *h_mass;

TH2 *trueSDjetptz, *trueSDjetptjt, *trueSDjetptr;
TH2 *h_SDjetChargedHadron, *h_SDjetNeutralHadron, *h_SDjetPhoton, *h_SDjetLepton;
TH2 *trueSDnconst;

void write(std::string filename)
{
  std::string file = filename + "_histos.root";

  outfile = new TFile(file.c_str(),"RECREATE");
  
  trueSDnconst->Write();
  trueSDjetptz->Write();
  trueSDjetptjt->Write();
  trueSDjetptr->Write();
  h_SDjetLepton->Write();
  h_SDjetChargedHadron->Write();
  h_SDjetNeutralHadron->Write();
  h_SDjetPhoton->Write();

  h_mass->Write();
  h_jetLepton->Write();
  h_jetChargedHadron->Write();
  h_jetNeutralHadron->Write();
  h_jetPhoton->Write();
  h_constpT->Write();
  h_processID->Write();
  h_scatJet->Write();
  h_scatLept->Write();
  h_lumi->Write();
  h_xsec->Write();
  h_eventsGen->Write();
  truthjetbosonphi->Write();
  truthjetbosontheta->Write();
  truthjetbosoneta->Write();
  truejetpttheta->Write();
  truejetptheta->Write();
  
  recosubjetpt->Write();
  truthsubjetpt->Write();
  sdenergygroomed->Write();
  truthSDjetzg->Write();
  truthSDjetrg->Write();
  recoSDjetzg->Write();
  recoSDjetrg->Write();
  truthrecozg->Write();
  truthrecorg->Write();
  recomatchdr->Write();
  truereconconst->Write();
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
  matchconstp->Write();
  matchconstpt->Write();
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
  truenconst->Write();
  reconconst->Write();
  ntruthjets->Write();
  nrecojets->Write();

  for(int i=0; i<npbins; i++)
    jes[i]->Write();

  outfile->Write();
  outfile->Close();

}
void instantiateHistos()
{
  for(int i = 0; i< nzgbins+1; i++)
    {
      zgbins[i] = 0 + i * 0.5 / nzgbins;
    }
  ostringstream name;
  for(int i=0; i<npbins; i++)
    {
      name.str("");
      name<<"jes"<<i;
      jes[i] = new TH1F(name.str().c_str(),";E_{jet}^{reco}/E_{jet}^{true}",
		        200,0,2);
    }

  h_mass = new TH1F("h_mass",";mass [GeV]",10000,0,2);
  h_jetChargedHadron = new TH2F("h_jetChargedHadron",";p_{T} [GeV]; N_{chg}", 50,0,50,30,0,30);
  h_jetNeutralHadron = new TH2F("h_jetNeutralHadron",";p_{T} [GeV]; N_{neut}",50,0,50,30,0,30);
  h_jetPhoton = new TH2F("h_jetPhoton",";p_{T} [GeV]; N_{photon}",50,0,50,30,0,30);
  h_jetLepton = new TH2F("h_jetLepton",";p_{T} [GeV]; N_{muon}", 50,0,50,30,0,30);
  h_SDjetChargedHadron = new TH2F("h_SDjetChargedHadron",";p_{T}^{SD} [GeV]; N_{chg}", 50,0,50,30,0,30);
  h_SDjetNeutralHadron = new TH2F("h_SDjetNeutralHadron",";p_{T}^{SD} [GeV]; N_{neut}",50,0,50,30,0,30);
  h_SDjetPhoton = new TH2F("h_SDjetPhoton",";p_{T}^{SD} [GeV]; N_{photon}",50,0,50,30,0,30);
  h_SDjetLepton = new TH2F("h_SDjetLepton",";p_{T}^{SD} [GeV]; N_{muon}", 50,0,50,30,0,30);

  h_constpT = new TH2F("h_constpT",";p [GeV]; #eta",200,-100,100,100,-3.5,3.5);
  h_processID = new TH1I("h_processID",";Process ID",1000,0,1000);
  h_scatJet = new TH2F("h_scatJet","",36,0.,TMath::Pi(), 200.,0.,200.);
  
  h_scatLept = new TH2F("h_scatLept","",36,0.,TMath::Pi(),140.,0.,70.);
  //h_scatLept = new TH2F("h_scatLept","; E [GeV]; #theta [deg]",36, 0., TMath::Pi(), 10.,0.,50.);
  h_xsec = new TH1F("h_xsec",";#sigma [#mub]",1000,0,0.0001);
  h_lumi = new TH1F("h_lumi",";Luminosity [#mub]^{-1}",1000,10e9,10e11);
  h_eventsGen = new TH1F("h_eventsGen",";N_{events}",10000000,0,10000000);
  truejetptheta = new TH2F("truejetptheta",";p^{true} [GeV]; #theta_{jet}^{true} [rad]", 50,0,50,300,-4,4);
  truejetpttheta = new TH2F("truejetpttheta",";p_{T}^{true} [GeV]; #theta_{jet}^{true} [rad]", 50,0,50,300,-4,4);
  truthjetbosonphi = new TH2F("truthjetbosonphi",";#phi^{truth}_{boson} [rad]; #phi^{truth}_{jet} [rad]",300,-3.14159,3.14159, 300,-3.14159,3.14159);
  truthjetbosoneta = new TH2F("truthjetbosoneta",";#eta^{truth}_{boson};#eta^{truth}_{jet}",300,-40,40,300,-40,40);
  truthjetbosontheta = new TH2F("truthjetbosontheta",";#theta^{truth}_{boson} [rad]; #theta^{truth}_{jet} [rad]",300,-4,4,300,-4,4);

  recosubjetpt = new TH2F("recosubjetpt",";p_{T}^{subjet,1} [GeV];p_{T}^{subjet,2} [GeV]", 40,0,40,40,0,40);
  truthsubjetpt = new TH2F("truthsubjetpt",";p_{T}^{subjet,1} [GeV];p_{T}^{subjet,2} [GeV]", 40,0,40,40,0,40);

  ntruthjets = new TH1F("ntruthjets",";N_{jets}",10,-0.5,9.5);
  nrecojets = new TH1F("nrecojets",";N_{jets}",10,-0.5,9.5);
  recomatchdr = new TH2F("recomatchdr",";#DeltaR;p_{T}^{jet,reco} [GeV]",
			 30,0,3,30,4,34);

  matchconstp = new TH1F("matchconstp",";p_{T}^{reco}/p_{T}^{true}",100,0.5,1.5);
  matchconstpt = new TH1F("matchconstpt",";p_{T}^{reco}/p_{T}^{true}",100,0.5,1.5);
  reconconst = new TH2F("reconconst",";p_{T}^{jet,reco} [GeV]; N_{const}^{reco}",30,4,34,30,0,30);
  truenconst = new TH2F("truenconst",";p_{T} [GeV];N_{const}",30,4,34,30,0,30);
   trueSDnconst = new TH2F("trueSDnconst",";p_{T}^{SD} [GeV];N_{const}",30,4,34,30,0,30);
  truereconconst = new TH2F("truereconconst",";N_{const}^{true};N_{const}^{reco}",30,0,30,30,0,30);
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
			  34,0,34,500,-30,30);
  truejetptphi = new TH2F("truejetptphi",";p_{T}^{true} [GeV]; #phi [rad]",
			  34,0,34,50,-3.14159,3.14159);
  recojetptz = new TH2F("recojetptz",";z_{reco};p_{T}^{jet,reco} [GeV]",
			nzbins,zbins,nptbins,ptbins);
  recojetptjt = new TH2F("recojetptjt",
			 ";j_{T}^{reco} [GeV]; p_{T}^{jet,reco} [GeV]",
			 njtbins,jtbins,nptbins,ptbins);
  recojetptr = new TH2F("recojetptr",";r_{reco}; p_{T}^{jet,reco} [GeV]",
			nrbins,rbins,nptbins,ptbins);
  trueSDjetptz = new TH2F("trueSDjetptz",";z;p_{T}^{SD} [GeV]",
			nzbins,zbins,nptbins,ptbins);
  trueSDjetptjt = new TH2F("trueSDjetptjt",
			 ";j_{T} [GeV]; p_{T}^{SD} [GeV]",
			 njtbins,jtbins,nptbins,ptbins);
  trueSDjetptr = new TH2F("trueSDjetptr",";r; p_{T}^{SD} [GeV]",
			nrbins,rbins,nptbins,ptbins);
  truejetptz = new TH2F("truejetptz",";z;p_{T} [GeV]",
			nzbins,zbins,nptbins,ptbins);
  truejetptjt = new TH2F("truejetptjt",
			 ";j_{T} [GeV]; p_{T} [GeV]",
			 njtbins,jtbins,nptbins,ptbins);
  truejetptr = new TH2F("truejetptr",";r; p_{T} [GeV]",
			nrbins,rbins,nptbins,ptbins);
  recojetpteta = new TH2F("recojetpteta", ";p_{T}^{jet,reco} [GeV]; #eta",
			  30,4,34,500,-30,30);
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
