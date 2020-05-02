#include "include/TruthEvent.h"

#include "include/BreitFrame.h"

#include <fastjet/Selector.hh>
#include <fastjet/ClusterSequence.hh>
#include <fastjet/contrib/SoftDrop.hh>

#include <iostream>


void TruthEvent::processEvent()
{
  setScatteredLepton();
  setTruthParticles();

}


void TruthEvent::setScatteredLepton()
{
  m_scatLepton = m_truthEvent->ScatteredLepton();
  if(m_verbosity > 1)
    {
      std::cout<< "Scattered lepton : "<< m_scatLepton->GetPx() << " "
	       << m_scatLepton->GetPy() << " " << m_scatLepton->GetPz()
	       << " " << m_scatLepton->GetE() << std::endl;
    }
}

void TruthEvent::setTruthParticles()
{

  for(int part = 0; part < m_truthEvent->GetNTracks(); ++part)
    {
      /// Skip the beam
      if( part < 3 )
	continue;
      
      const Particle *truthParticle = m_truthEvent->GetTrack(part);
      
      /// only want final state particles
      if(truthParticle->GetStatus() != 1)
	continue;
      
      /// Skip the scattered electron, since it is special
      if(truthParticle->GetE() == m_scatLepton->GetE())
	continue;
      if(m_verbosity > 2)
	{
	  std::cout << "Lab Truth  : " <<truthParticle->Id() 
		    << " " <<truthParticle->GetPx() << " " 
		    << truthParticle->GetPy() << " " << truthParticle->GetPz()
		    << " " << truthParticle->GetE() << std::endl;	  
	}

      // Transform Particle 4 Vectors to the Breit Frame 
      TLorentzVector *partFourVec = new TLorentzVector( truthParticle->PxPyPzE() );
      labToBreit( partFourVec, m_truthEvent);
      
      if(m_verbosity > 0)
	{
	  std::cout << "Breit Truth  : " <<truthParticle->Id() 
		    << " " <<partFourVec->Px() << " " 
		    << partFourVec->Py() << " " << partFourVec->Pz()
		    << " " << partFourVec->E() << std::endl;	  
	}


      m_particles.push_back(fastjet::PseudoJet(partFourVec->Px(),
					       partFourVec->Py(),
					       partFourVec->Pz(),
					       partFourVec->E()));
    }

  return;
}

PseudoJetVec TruthEvent::getTruthJets(fastjet::ClusterSequence *cs, 
				       JetDef jetDef)
{
  /// Create the cluster sequence
  cs = new fastjet::ClusterSequence(m_particles, jetDef.getJetDef());
  
  PseudoJetVec allTruthJets = fastjet::sorted_by_pt(cs->inclusive_jets());

  if(m_verbosity > 1)
    {
      std::cout << "Finding jets for jet pT>" << jetDef.getMinJetPt() 
		<< " and |eta|<" << jetDef.getMaxJetRapidity() << std::endl;
    }
  
  /// Make eta/pt selections
  fastjet::Selector selectPt = fastjet::SelectorPtMin(jetDef.getMinJetPt());
  fastjet::Selector selectEta = fastjet::SelectorAbsRapMax(jetDef.getMaxJetRapidity());
  fastjet::Selector select = selectPt and selectEta;
  
  PseudoJetVec selectJets = select(allTruthJets);

  return selectJets;

}

PseudoJetVec TruthEvent::getTruthSoftDropJets(PseudoJetVec truthJets, SoftDropJetDef sdJetDef)
{

  fastjet::contrib::SoftDrop sd(sdJetDef.getSoftDrop());
  
  PseudoJetVec softDropJets;
 
  for(int jet = 0; jet < truthJets.size(); ++jet)
    {
      fastjet::PseudoJet softDropJet = sd(truthJets[jet]);
      softDropJets.push_back(softDropJet);
    }

  return softDropJets;

}




