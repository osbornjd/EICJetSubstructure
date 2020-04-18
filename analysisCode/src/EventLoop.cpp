
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
  
  TruthEvent event;
  SmearedEvent smearevent;
  JetDef jetdef;
  SoftDropJetDef sd(0.1, 2, 0.5);
  
  //mctree->AddFriend("Smeared", smearedFile.c_str());
  erhic::EventPythia* truthEvent(NULL);
  Smear::Event* smearEvent(NULL);

  mctree->SetBranchAddress("event", &truthEvent);
  mctree->SetBranchAddress("eventS", &smearEvent);

  for (int ev = 0; ev < 5; ev++)
    {
      mctree->GetEntry(ev);
      event.ProcessEvent(truthEvent);
      cout << event.Get_eventNumber() << endl;
      cout << event.Get_true_q2() << endl;
    }

  // Smear::Event* smearEvent(NULL);
  

}
