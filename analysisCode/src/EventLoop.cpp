#include <fastjet/ClusterSequence.hh>
#include <fastjet/Selector.hh>
#include <fastjet/contrib/SoftDrop.hh>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <eicsmear/erhic/EventPythia.h>
#include <eicsmear/smear/EventS.h>
#include "include/TruthEvent.h"
#include "include/SmearedEvent.h"
#include "include/JetDef.h"
#include "include/SoftDropJetDef.h"

using namespace std;
using namespace fastjet;

int main()
{
  /// Collect arguments
  //std::string mcFile = argv[1];
  //std::string smearedFile = argv[2];
  TFile mc("../truth.root");
  TTree *mctree = (TTree*)mc.Get("EICTree");
  mctree->AddFriend("Smeared","../smeared.root");

 //TFile mc(mcFile.c_str());
 // TTree *mctree = (TTree*)mc.Get("EICTree");

  JetDef R1jetdef(fastjet::antikt_algorithm, 1.0);
  R1jetdef.setMinJetPt(2.);
  R1jetdef.setMaxJetRapidity(4);
  SoftDropJetDef R1sd(0.01, 2, R1jetdef.getR());  // Jet Def Stuff 
  double R = 0.7;
  JetDef jetDef(antikt_algorithm, R);
 
  
  //mctree->AddFriend("Smeared", smearedFile.c_str());
  erhic::EventPythia* truthEvent(NULL);
  Smear::Event* smearEvent(NULL);

  mctree->SetBranchAddress("event", &truthEvent);
  mctree->SetBranchAddress("eventS", &smearEvent);

  for (int event = 0; event < 2; event++)
    {

      if(event % 10 == 0)
	std::cout<<"Processed " << event << " events" << std::endl;

      mctree->GetEntry(event);

      TruthEvent trueEvent(*truthEvent);
      trueEvent.setVerbosity(0);
      trueEvent.processEvent( );
    }  

}
