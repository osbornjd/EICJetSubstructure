
#include "analyzeJets.h"
#include "HistoManager.h"
#include "sPhenixStyle.h"
#include "sPhenixStyle.C"

void analyzeJets(std::string file)
{
  SetsPhenixStyle();

  std::string filename = file;
  infile = TFile::Open(filename.c_str());

  /// If filename contains breit
  if(filename.find("breit") != string::npos)
    {
      breitFrame = true;
    }
  
  gROOT->ProcessLine(".L ../src/fastJetLinker.C+");
  setupTree();

  instantiateHistos();

  loop();

  getLumi();

  write(filename);

}

void getLumi()
{

  TTree *runtree = (TTree*)infile->Get("runTree");
  float lumi;
  float xsec;
  float events;
  runtree->SetBranchAddress("totalCrossSection",&xsec);
  runtree->SetBranchAddress("nEventsGen",&events);
  runtree->SetBranchAddress("integratedLumi",&lumi);
  for(int i = 0; i< runtree->GetEntries(); i++)
    {
      runtree->GetEntry(i);
      h_lumi->Fill(lumi);
      h_eventsGen->Fill(events);
      h_xsec->Fill(xsec);
      std::cout<<xsec<<"  "<<lumi<<"  "<<events<<std::endl;
    }

}
void recoJetAnalysis(JetConstVec *recojets)
{
  int njets = 0;
  for(int i = 0; i < recojets->size(); i++)
    {
      TLorentzVector jet;
      jet = recojets->at(i).first;

      float jetpt = jet.Pt();
      if(jetpt < minjetpt )
	continue;
      if(fabs(jet.Eta()) > maxjeteta)
	continue;

      njets++;
      recojetpteta->Fill(jetpt, jet.Eta());
      recojetptphi->Fill(jetpt, jet.Phi());

      TVector3 jet3;
      jet3.SetXYZ(jet.Px(), jet.Py(), jet.Pz());
      reconconst->Fill(jetpt, recojets->at(i).second.size());

      /// Iterate over constituents
      for(int j = 0; j < recojets->at(i).second.size(); ++j)
	{
	  TLorentzVector con = recojets->at(i).second.at(j);
	  TVector3 con3;
	  con3.SetXYZ(con.Px(), con.Py(), con.Pz());
	  TVector3 cross = jet3.Cross(con3);
	  
	  float z = jet3.Dot(con3) / (jet3.Mag2());
	  float jt = cross.Mag() / jet3.Mag();
	  float r = sqrt(pow(checkdPhi(jet.Phi() - con.Phi()), 2) + pow(jet.Rapidity() - con.Rapidity(),2));

	  recojetptz->Fill(z, jetpt);
	  recojetptjt->Fill(jt, jetpt);
	  recojetptr->Fill(r, jetpt);

	}
    }
  nrecojets->Fill(njets);
}

double truthJetAnalysis(JetConstVec *truthjets)
{

  double jetpt = 0;
  int njets = 0;
  for(int jet = 0; jet < truthJets->size(); jet++)
    {
      TLorentzVector jetVec;
      jetVec = truthJets->at(jet).first;
      if(jetVec.Pt() > jetpt)
	jetpt = jetVec.Pt();
      
      if(jetVec.Pt() < minjetpt)
	continue;
      if(fabs(jetVec.Eta()) > maxjeteta)
	continue;
      
      njets++;
      truenconst->Fill(jetVec.Pt(), truthJets->at(jet).second.size());

      truejetptphi->Fill(jetVec.Pt(), jetVec.Phi());
      truejetpteta->Fill(jetVec.Pt(), jetVec.Eta());
      truejetpttheta->Fill(jetVec.Pt(), jetVec.Theta());
      truejetptheta->Fill(jetVec.P(), jetVec.Theta());
      TVector3 jet3;
      jet3.SetXYZ(jetVec.Px(), jetVec.Py(), jetVec.Pz());

      for(int i = 0; i < truthJets->at(jet).second.size(); ++i)
	{
	  TLorentzVector con;
	  con = truthJets->at(jet).second.at(i);
	  TVector3 con3;
	  con3.SetXYZ(con.Px(), con.Py(), con.Pz());
	  TVector3 cross = jet3.Cross(con3);
	  
	  float z = jet3.Dot(con3) / (jet3.Mag2());
	  float jt = cross.Mag() / jet3.Mag();
	  float r = sqrt(pow(checkdPhi(jetVec.Phi() - con.Phi()), 2) + pow(jetVec.Eta() - con.Eta(),2));

	  truejetptz->Fill(z, jetpt);
	  truejetptjt->Fill(jt, jetpt);
	  truejetptr->Fill(r, jetpt);
	} 
    }

  ntruthjets->Fill(njets);

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

      analyzeMatchedJets(matchedJets, matchedParticles);

      recoSDJetAnalysis(recoSDJets);
      truthSDJetAnalysis(truthSDJets);
      analyzeMatchedSDJets(matchedSDJets);

      compareAKTSDTruthJets(truthJets, truthSDJets);

      /// Event level kinematics
      truerecx->Fill(truex,recx);
      truerecy->Fill(truey,recy);
      truerecq2->Fill(trueq2,recq2);
      trueQ2x->Fill(truex,trueq2);
  
      trueQ2pT->Fill(trueq2, highestTruthJetPt);
    
      /// Make some event level histograms of jets+exchange boson
      for(int jet = 0; jet < truthJets->size(); jet++)
	{
	  TLorentzVector jetVec;
	  jetVec = truthJets->at(jet).first;
	  if(jetVec.Pt() > 1)
	    continue;
	  truthjetbosonphi->Fill(truthExchangeBoson->Phi(),
				 jetVec.Phi());
	  truthjetbosoneta->Fill(truthExchangeBoson->Eta(),
				 jetVec.Eta());
	  truthjetbosontheta->Fill(truthExchangeBoson->Theta(),
				   jetVec.Theta());
	  
	}

    }

}

void compareAKTSDTruthJets(JetConstVec *truthjets, JetConstVec *truthsdjets)
{
  for(int i=0; i<truthjets->size(); ++i)
    {

      TLorentzVector aktjet = truthjets->at(i).first;
      TLorentzVector sdjet = truthsdjets->at(i).first;
    
      sdenergygroomed->Fill(sdjet.E() / aktjet.E());
    }

}

void analyzeMatchedJets(MatchedJets *matchedjets,
			TLorentzPairVec *matchedparticles)
{
  for(int i = 0; i < matchedjets->size(); i++)
    {
      JetConstPair truthJetConst = matchedjets->at(i).at(0);
      JetConstPair recoJetConst = matchedJets->at(i).at(1);

      TLorentzVector truthJet = truthJetConst.first;
      TLorentzVector recoJet = recoJetConst.first;
      TLorentzVectorVec truthConst = truthJetConst.second;
      TLorentzVectorVec recoConst = recoJetConst.second;

      if(truthJet.Pt() < minjetpt || fabs(truthJet.Eta()) > maxjeteta)
	continue;

      if(recoJet.Pt() > minjetpt && fabs(recoJet.Eta()) < maxjeteta)
	{
	  recojetptetatruejetpt->Fill(truthJet.Pt(), truthJet.Eta());
	}
      
      matchedJetDr->Fill((float)truthJet.DeltaR(recoJet));
      
      if(truthJet.DeltaR(recoJet) > 0.5)
	continue;
   

      truereconconst->Fill(truthConst.size(), recoConst.size());

      matchedJetdPhi->Fill((float)truthJet.DeltaPhi(recoJet));
      matchedJetdEta->Fill((float)truthJet.Eta() - recoJet.Eta());

      recotruejetpt->Fill(truthJet.Pt(), recoJet.Pt());
      recotruejeteta->Fill(truthJet.Eta(), recoJet.Eta());
      recotruejetphi->Fill(truthJet.Phi(), recoJet.Phi());
      
      recotruejetp->Fill(truthJet.P(), recoJet.P());
      recotruejete->Fill(truthJet.E(), recoJet.E());

      /// Match constituents up
      for(int j = 0; j< recoConst.size(); j++)
	{
	  TLorentzVector recoCon = recoConst.at(j);
	  TLorentzVector truthMatch;
	  
	  for(int k =0; k< matchedparticles->size(); k++)
	    {
	      TLorentzVector matchreco = matchedparticles->at(k).second;
	      if(matchreco.Px() == recoCon.Px() &&
		 matchreco.Py() == recoCon.Py() &&
		 matchreco.Pz() == recoCon.Pz())
		{
		  truthMatch = matchedparticles->at(k).first;
		}
	    }

	  bool matched = false;
	  /// now check that the truth particle was actually in the jet
	  for(int k = 0; k < truthConst.size(); k++)
	    {
	      TLorentzVector truthCon = truthConst.at(k);
	      if(truthCon.Px() == truthMatch.Px() &&
		 truthCon.Py() == truthMatch.Py() &&
		 truthCon.Pz() == truthMatch.Pz())
		{
		  matched = true;
		  break;
		}
	    }
	  if(matched)
	    {
	      /// Found a matched truth constituent and it was in the truth jet
	 
	      TVector3 truthJet3, recoJet3, recoCon3, truthMatch3;
	      truthJet3.SetXYZ(truthJet.Px(), 
			       truthJet.Py(), truthJet.Pz());
	      recoJet3.SetXYZ(recoJet.Px(), 
			      recoJet.Py(), recoJet.Pz());
	      recoCon3.SetXYZ(recoCon.Px(), 
			      recoCon.Py(), recoCon.Pz());
	      truthMatch3.SetXYZ(truthMatch.Px(),
				 truthMatch.Py(), truthMatch.Pz());

	      float recoz = recoJet3.Dot(recoCon3) / (recoJet3.Mag2());
	      float truthz = truthJet3.Dot(truthMatch3) / (truthJet3.Mag2());
	      TVector3 truecross = truthJet3.Cross(truthMatch3);
	      TVector3 recocross = recoJet3.Cross(recoCon3);
	      float recojt = recocross.Mag() / recoJet3.Mag();
	      float truejt = truecross.Mag() / truthJet3.Mag();
	      float recodphi = checkdPhi(recoJet.Phi() - recoCon.Phi());
	      float truedphi = checkdPhi(truthJet.Phi() - truthMatch.Phi());

	      matchconstp->Fill(recoCon.P() / truthMatch.P());
	      matchconstpt->Fill(recoCon.Pt() / truthMatch.Pt());
	      truthRecoConstdPhi->Fill(checkdPhi(truthMatch.Phi() - recoCon.Phi()));
	      truthRecoConstdEta->Fill(truthMatch.Eta() - recoCon.Eta());
	      truthRecoConstdRap->Fill(truthMatch.Rapidity() - recoCon.Rapidity());
	      float recodeltar = recoCon.DeltaR(recoJet);
	      recomatchdr->Fill(recodeltar,recoJet.Pt());
	      float recor = sqrt(pow(recodphi ,2) +
				 pow(recoJet.Rapidity() - recoCon.Rapidity(), 2));
	      float truer = sqrt(pow(truedphi ,2) +
				 pow(truthJet.Rapidity() - truthMatch.Rapidity(),2));
	      
	      truerecoz->Fill(truthz, recoz);
	      truerecojt->Fill(truejt,recojt);
	      truerecor->Fill(truer, recor);
	    }
	  else
	    {
	      /// If a match couldn't be found, reco jet const was mistakenly
	      /// reconstructed within jet
	    }

	}
    }

}

void recoSDJetAnalysis(JetConstVec *recojets)
{
  for(int ijet = 0; ijet< recojets->size(); ijet++)
    {
      TLorentzVector jet = recojets->at(ijet).first;
      
      if(jet.Pt() < minjetpt || fabs(jet.Eta()) > maxjeteta)
	continue;

      TLorentzVectorVec constituents = recojets->at(ijet).second;
      ///first two constituents are the subjets
      TLorentzVector subjet1, subjet2;
      subjet1 = constituents.at(0);
      subjet2 = constituents.at(1);
      float zg = std::min(subjet1.Pt(), subjet2.Pt()) / (subjet1.Pt() + subjet2.Pt());
      float Rg = subjet1.DeltaR(subjet2);
      recoSDjetzg->Fill(zg, jet.Pt());
      recoSDjetrg->Fill(Rg, jet.Pt());
    }

}

void truthSDJetAnalysis(JetConstVec *truthjets)
{
  for(int ijet = 0; ijet < truthjets->size(); ijet++)
    {
      TLorentzVector jet;
      jet = truthjets->at(ijet).first;

      if(jet.Pt() < minjetpt || fabs(jet.Eta()) > maxjeteta)
	 continue;


      TLorentzVectorVec constituents;
      constituents = truthjets->at(ijet).second;
      ///first two constituents are the subjets
      TLorentzVector subjet1, subjet2;
      subjet1 = constituents.at(0);
      subjet2 = constituents.at(1);
      float zg = std::min(subjet1.Pt(), subjet2.Pt()) / (subjet1.Pt() + subjet2.Pt());
      float Rg = subjet1.DeltaR(subjet2);
      truthSDjetzg->Fill(zg, jet.Pt());
      truthSDjetrg->Fill(Rg, jet.Pt());
    }

}

void analyzeMatchedSDJets(MatchedJets *matchedjets)
{
  for(int i = 0; i < matchedjets->size(); i++)
    {
      JetConstPair truthJetConst = matchedjets->at(i).at(0);
      JetConstPair recoJetConst = matchedjets->at(i).at(1);
      
      TLorentzVector truthJet = truthJetConst.first;
      TLorentzVector recoJet = recoJetConst.first;
      TLorentzVectorVec truthConst = truthJetConst.second;
      TLorentzVectorVec recoConst = recoJetConst.second;

      TLorentzVector truthSubjet1, truthSubjet2;
      truthSubjet1 = truthConst.at(0);
      truthSubjet2 = truthConst.at(1);

      TLorentzVector recoSubjet1, recoSubjet2;
      recoSubjet1 = recoConst.at(0);
      recoSubjet2 = recoConst.at(1);
      
      truthrecosdjetdeltar->Fill(truthJet.DeltaR(recoJet));
      if(truthJet.DeltaR(recoJet)>0.5)
	continue;
      if(truthJet.Pt() < minjetpt || fabs(truthJet.Eta()) > maxjeteta)
	 continue;
      
      
      /// Remove subjets where the soft drop criteria was already satisfied
      /// by the antikt jet, so subjets are listed as (-999,-999,-999,-999)
      if(truthSubjet1.E() < 0 || truthSubjet2.E() < 0)
        continue;
      if(recoSubjet1.E() < 0 || recoSubjet2.E() < 0)
        continue;
      if(truthSubjet1.Pt() < 0 || truthSubjet2.Pt() < 0)
	std::cout<<truthSubjet1.Pt()<<"  "<<truthSubjet2.Pt()<<std::endl;
      
      truthsubjetpt->Fill(truthSubjet1.Pt(), truthSubjet2.Pt());
      recosubjetpt->Fill(recoSubjet1.Pt(), recoSubjet2.Pt());

      assert(truthSubjet1.Pt() + truthSubjet2.Pt() == truthJet.Pt());
      assert(recoSubjet1.Pt() + recoSubjet2.Pt() == recoJet.Pt());

      float truthzg = std::min(truthSubjet1.Pt(), truthSubjet2.Pt())
	/(truthSubjet1.Pt() + truthSubjet2.Pt());
      float truthrg = truthSubjet1.DeltaR(truthSubjet2);
      float recozg = std::min(recoSubjet1.Pt(), recoSubjet2.Pt())
	/(recoSubjet1.Pt() + recoSubjet2.Pt());
      
      float recorg = recoSubjet1.DeltaR(recoSubjet2);
      truthrecozg->Fill(truthzg,recozg);
      truthrecorg->Fill(truthrg,recorg);

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
  jettree->SetBranchAddress("matchedParticles", &matchedParticles);
  jettree->SetBranchAddress("smearExchangeBoson", &smearedExchangeBoson);
  jettree->SetBranchAddress("truthR1SDJets", &truthSDJets);
}

float checkdPhi(float dphi)
{
  float newdphi = dphi;
  if(dphi < -1 * PI)
    newdphi += 2. * PI;
  else if(dphi > PI)
    newdphi -= 2. * PI;

  return newdphi;

}
