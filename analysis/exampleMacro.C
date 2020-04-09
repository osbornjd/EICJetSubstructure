


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
      cout<<q2<<"   "<<trueq2<<endl;

      cout<<"Processing event number " <<eventNumber << " with :"
	  <<q2<<"   "<<processId<<endl;
      std::vector<const erhic::VirtualParticle*> truthParticles
	=truthEvent->GetTracks();;
      int nTruth = truthEvent->GetNTracks();
      for(int part = 0; part < nTruth; part++)
	{
	  const erhic::VirtualParticle *truthPart = truthParticles.at(part);

	  erhic::Pid pid = truthPart->Id();
	  double px = truthPart->GetPx();
	  double py = truthPart->GetPy();
	  double pz = truthPart->GetPz();
	  double e = truthPart->GetE();
	  
	  cout<<part+1<<"  "<<pid<<"  "<<px<< "  "<<py<<"  "<<pz<<"  "<<e<<endl;

	}
    }
  

}
