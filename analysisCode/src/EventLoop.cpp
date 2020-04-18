
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

using namespace std;
using namespace fastjet;

int main()
{
  TruthEvent event;
  SmearedEvent smearevent;
  JetDef jetdef;

  TFile mcf("../truth.root");
  TTree *mctree = (TTree*)mcf.Get("EICTree");
  mctree->AddFriend("Smeared","../smeared.root");

  erhic::EventPythia* truthEvent(NULL);
  mctree->SetBranchAddress("event", &truthEvent);

  for (int ev = 0; ev < 5; ev++)
    {
      mctree->GetEntry(ev);
      event.ProcessEvent(truthEvent);
      cout << event.Get_eventNumber() << endl;
      cout << event.Get_true_q2() << endl;
    }

  // Smear::Event* smearEvent(NULL);
  

}
