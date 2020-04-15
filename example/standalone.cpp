
#include <fastjet/ClusterSequence.hh>
#include <fastjet/Selector.hh>
#include <fastjet/contrib/SoftDrop.hh>
#include <iostream>


#include <eicsmear/erhic/EventPythia.h>
#include <eicsmear/smear/EventS.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>


using namespace std;
using namespace fastjet;

int main()
{
  
  TFile mcf("../MCData/example/truth.root");
  TTree *mctree = (TTree*)mcf.Get("EICTree");
  mctree->AddFriend("Smeared","../MCData/example/smeared.root");

  erhic::EventPythia* truthEvent(NULL);
  Smear::Event* smearEvent(NULL);

  mctree->SetBranchAddress("event", &truthEvent);
  mctree->SetBranchAddress("eventS", &smearEvent);

  vector<PseudoJet> truthPseudoJets;
  double R=0.7;
  JetDefinition jetDef(antikt_algorithm, R);
  Selector selectRapidity = SelectorAbsRapMax(4);
  Selector selectPtMin = SelectorPtMin(5.0);
  Selector selectBoth = selectPtMin and selectRapidity;

  /// soft drop conditions
  double zcut = 0.1;
  double beta = 0;
  contrib::SoftDrop sd(beta, zcut, R);

  for(int event = 0; event < 1; event++)
    {
      mctree->GetEntry(event);

      double q2 = truthEvent->GetHardQ2();
      double trueq2 = truthEvent->GetTrueQ2();
      int processId = truthEvent->GetProcess();
      int eventNumber = truthEvent->GetN();

      cout<<"Processing event number " <<eventNumber << " with :"
	  <<q2<<"   "<<processId<<endl;
      std::vector<const erhic::VirtualParticle*> truthParticles
	= truthEvent->GetTracks();

      int nTruth = truthEvent->GetNTracks();
      
      truthPseudoJets.clear();

      for(int part = 0; part < nTruth; part++)
	{
	  const Particle *truthPart = truthEvent->GetTrack(part);

	  erhic::Pid pid = truthPart->Id();
	  double px = truthPart->GetPx();
	  double py = truthPart->GetPy();
	  double pz = truthPart->GetPz();
	  double e = truthPart->GetE();
	  short status = truthPart->GetStatus();
	 
	  truthPseudoJets.push_back(PseudoJet(px,py,pz,e));

	  //cout<<part+1<<"  "<<status<<"  "<<pid<<"  "<<px<< "  "<<py<<"  "<<pz<<"  "<<e<<endl;
	}

      /// Make the truth jets
      ClusterSequence cs(truthPseudoJets, jetDef);
      vector<PseudoJet> allTruthJets = sorted_by_pt(cs.inclusive_jets());
      /// Make the cuts
      vector<PseudoJet> truthJets = selectBoth(allTruthJets);
      
      for(int truthJet = 0; truthJet < truthJets.size(); truthJet++)
	{
	  /// Look at antikt constituents
	  vector<PseudoJet> constituents = truthJets[truthJet].constituents();
	  /// get jet properties
	  double jetphi = truthJets[truthJet].phi();

	  
	  /// run soft drop on truth anti-kt jets
	  PseudoJet softDropJet = sd(truthJets[truthJet]);
	  
	  // now you can do stuff with the SD jet
	  float subJetDR = softDropJet.structure_of<contrib::SoftDrop>().delta_R();
	  
	}

      for(int part = 0; part < smearEvent->GetNTracks(); part++)
	{
	  /// Skip the beam
	  if( part < 3 ) 
	    continue;

	  const Smear::ParticleMCS *smearPart = smearEvent->GetTrack(part);
	  const Particle * truthPart = truthEvent->GetTrack(part);

	  /// Only select final state 
	  if(truthPart->GetStatus() != 1)
	    continue;

	  /// If truth particle wasn't smeared (e.g. out of acceptance), skip
	  if(smearPart == NULL)
	    continue;

	  float tpx = truthPart->GetPx();
	  float tpy = truthPart->GetPy();
	  float tpz = truthPart->GetPz();
	  float te = truthPart->GetE();
	  float px = smearPart->GetPx();
	  float py = smearPart->GetPy();
	  float pz = smearPart->GetPz();
	  float e = smearPart->GetE();
	  TLorentzVector truth, smear;
	  truth.SetPxPyPzE(tpx,tpy,tpz,te);
	  smear.SetPxPyPzE(px,py,pz,e);
	  
	  cout<<"truth: "<<tpx<<"  "<<tpy<<"  "<<tpz<<"  "<<te<<"    "<<truth.Eta()<<endl;
	  cout<<"smear : "<<px<<"  "<<py<<"  "<<pz<<"  "<<e<<"   "<<smear.Eta()<<endl;
	}


     

    }
  

}
