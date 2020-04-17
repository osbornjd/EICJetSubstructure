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

int main()
{
  TruthEvent event;
  SmearedEvent smearevent;
  JetDef jetdef;
  SoftDropJetDef sd(0.1, 2, 0.5);

}
