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

  // Jet Def Stuff 
  double R = 0.7;
  JetDef jetDef(antikt_algorithm, R);

  /// soft drop conditions
  double zcut = 0.1;
  double beta = 0;
  SoftDropJetDef softdrop_def(beta, zcut, R);
  
  TruthEvent event;
  SmearedEvent smearevent;
  
  //mctree->AddFriend("Smeared", smearedFile.c_str());
  erhic::EventPythia* truthEvent(NULL);
  Smear::Event* smearEvent(NULL);

  mctree->SetBranchAddress("event", &truthEvent);
  mctree->SetBranchAddress("eventS", &smearEvent);

  for (int ev = 0; ev < 10; ev++)
    {
      mctree->GetEntry(ev);
      event.ProcessEvent( truthEvent, jetDef, softdrop_def );
    }

  // Smear::Event* smearEvent(NULL);
  

}
