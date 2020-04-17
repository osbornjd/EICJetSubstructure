#include "include/SmearedEvent.h"

#include <fastjet/Selector.hh>
#include <fastjet/ClusterSequence.hh>

#include <iostream>

void SmearedEvent::setSmearedParticles()
{

  for(int part = 0; part < m_smearEvent->GetNTracks(); ++part)
    {
      /// Skip the beam
      if( part < 3 )
	continue;
      
      const Smear::ParticleMCS *particle = m_smearEvent->GetTrack(part);
      const Particle *truthParticle = m_truthEvent->GetTrack(part);
      
      /// only want final state particles
      if(truthParticle->GetStatus() != 1)
	continue;

      /// If truth particle wasn't smeared (e.g. out of acceptance), skip
      if(particle == NULL)
	continue;
      
      m_particles.push_back(fastjet::PseudoJet(particle->GetPx(),
					       particle->GetPy(),
					       particle->GetPz(),
					       particle->GetE()));
    }

  return;
}

std::vector<fastjet::PseudoJet> SmearedEvent::getReconstructedJets(JetDef jetDef)
{
  /// Create the cluster sequence
  fastjet::ClusterSequence cs(m_particles, jetDef.getJetDef());
  
  std::vector<fastjet::PseudoJet> allRecoJets = fastjet::sorted_by_pt(cs.inclusive_jets());

  /// Make eta/pt selections
  fastjet::Selector selectPt = fastjet::SelectorPtMin(jetDef.getMinJetPt());
  fastjet::Selector selectEta = fastjet::SelectorAbsRapMax(jetDef.getMaxJetRapidity());
  fastjet::Selector select = selectPt and selectEta;
  
  std::vector<fastjet::PseudoJet> selectJets = select(allRecoJets);

  return selectJets;

}
