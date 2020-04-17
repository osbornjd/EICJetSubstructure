/// Fastjet includes
#include <fastjet/ClusterSequence.hh>
#include <fastjet/Selector.hh>
#include <fastjet/contrib/SoftDrop.hh>

/// Root includes
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>

/// EIC smear includes
#include <eicsmear/erhic/EventPythia.h>
#include <eicsmear/smear/EventS.h>

/// Local includes
#include "include/TruthEvent.h"
#include "include/SmearedEvent.h"
#include "include/JetDef.h"
#include "include/SoftDropJetDef.h"

#include <iostream>

int main(int argc, char **argv)
{
  /// Collect arguments
  std::string mcFile = argv[1];
  std::string smearedFile = argv[2];

  TFile mc(mcFile.c_str());
  TTree *mctree = (TTree*)mc.Get("EICTree");
  
  TruthEvent event;
  SmearedEvent smearevent;
  JetDef jetdef;
  SoftDropJetDef sd(0.1, 2, 0.5);
  
  mctree->AddFriend("Smeared", smearedFile.c_str());
  erhic::EventPythia* truthEvent(NULL);
  Smear::Event* smearEvent(NULL);

  mctree->SetBranchAddress("event", &truthEvent);
  mctree->SetBranchAddress("eventS", &smearEvent);

  for(int event = 0; event < mctree->GetEntries(); ++event)
    {
      mctree->GetEntry(event);

    }


}
