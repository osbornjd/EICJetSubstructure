
/**
 * To run:
 * $ root -l
 * $ root] gSystem->Load("libeicsmear")
 * $ root] .x exampleMacro.C
 */

void exampleMacro()
{

  TFile mcf("../MCData/example/truth.root");
  TTree *mctree = (TTree*)mcf.Get("EICTree");
  mctree->AddFriend("Smeared","../MCData/example/smeared.root");

  erhic::EventPythia* truthEvent(NULL);
  Smear::Event* smearEvent(NULL);

  mctree->SetBranchAddress("event", &truthEvent);
  mctree->SetBranchAddress("eventS", &smearEvent);

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
      for(int part = 0; part < nTruth; part++)
	{
	  //const erhic::VirtualParticle *truthPart = truthParticles.at(part);
	  const Particle *truthPart = truthEvent->GetTrack(part);
	  const Smear::ParticleMCS *smearPart = smearEvent->GetTrack(part);
	 
	  erhic::Pid pid = truthPart->Id();
	  double px = truthPart->GetPx();
	  double py = truthPart->GetPy();
	  double pz = truthPart->GetPz();
	  double e = truthPart->GetE();
	  short status = truthPart->GetStatus();

	  cout<<part+1<<"  "<<status<<"  "<<pid<<"  "<<px<< "  "<<py<<"  "<<pz<<"  "<<e<<endl;
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
