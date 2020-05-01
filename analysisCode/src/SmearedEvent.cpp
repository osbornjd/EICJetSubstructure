#include "include/SmearedEvent.h"

#include <fastjet/Selector.hh>
#include <fastjet/ClusterSequence.hh>
#include <fastjet/contrib/SoftDrop.hh>

#include <iostream>


void SmearedEvent::processEvent()
{
  setScatteredLepton();
  setSmearedParticles();
}


void SmearedEvent::setScatteredLepton()
{
  m_scatLepton = m_smearEvent->ScatteredLepton();
  if(m_verbosity > 1)
    {
      std::cout<< "Scattered lepton : "<< m_scatLepton->GetPx() << " "
	       << m_scatLepton->GetPy() << " " << m_scatLepton->GetPz()
	       << " " << m_scatLepton->GetE() << std::endl;
    }
}

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
      if(particle == NULL || particle->GetE() == 0)
	continue;
      
      /// Skip the scattered electron, since it is special
      if(particle->GetE() == m_scatLepton->GetE())
	continue;
      if(m_verbosity > 0)
	{
	  std::cout << "Truth  : "<<truthParticle->Id() 
		    << " " <<truthParticle->GetPx() << " " 
		    << truthParticle->GetPy() << " " << truthParticle->GetPz()
		    << " " << truthParticle->GetE() << std::endl;
	  
	  std::cout << "Smeared : " << particle->GetPx() << " " 
		    << particle->GetPy() << " " << particle->GetPz() << " " 
		    << particle->GetE() << std::endl;
	}

      m_particles.push_back(fastjet::PseudoJet(particle->GetPx(),
					       particle->GetPy(),
					       particle->GetPz(),
					       particle->GetE()));
    }

  return;
}

PseudoJetVec SmearedEvent::getRecoJets(fastjet::ClusterSequence *cs, 
				       JetDef jetDef)
{
  /// Create the cluster sequence
  cs = new fastjet::ClusterSequence(m_particles, jetDef.getJetDef());
  
  PseudoJetVec allRecoJets = fastjet::sorted_by_pt(cs->inclusive_jets());

  if(m_verbosity > 1)
    {
      std::cout << "Finding jets for jet pT>" << jetDef.getMinJetPt() 
		<< " and |eta|<" << jetDef.getMaxJetRapidity() << std::endl;
    }
  
  /// Make eta/pt selections
  fastjet::Selector selectPt = fastjet::SelectorPtMin(jetDef.getMinJetPt());
  fastjet::Selector selectEta = fastjet::SelectorAbsRapMax(jetDef.getMaxJetRapidity());
  fastjet::Selector select = selectPt and selectEta;
  
  PseudoJetVec selectJets = select(allRecoJets);

  return selectJets;

}

PseudoJetVec SmearedEvent::getRecoSoftDropJets(PseudoJetVec recoJets, SoftDropJetDef sdJetDef)
{

  fastjet::contrib::SoftDrop sd(sdJetDef.getSoftDrop());
  
  PseudoJetVec softDropJets;
 
  for(int jet = 0; jet < recoJets.size(); ++jet)
    {
      fastjet::PseudoJet softDropJet = sd(recoJets[jet]);
      softDropJets.push_back(softDropJet);
    }

  return softDropJets;

}


std::vector<PseudoJetVec> SmearedEvent::matchTruthRecoJets(
	    PseudoJetVec truthjets,
	    PseudoJetVec recojets)
{
  PseudoJetVec matchJetPair;
  std::vector<PseudoJetVec> matchedVector;
  

  for(int reco = 0; reco < recojets.size(); ++reco)
    {
      matchJetPair.clear();
      
      fastjet::PseudoJet recojet = recojets[reco];
      double mindR = 9999;
      fastjet::PseudoJet matchedTruthJet;
      for(int truth = 0; truth < truthjets.size(); ++truth)
	{
	  fastjet::PseudoJet truthjet = truthjets[truth];
	  double dR = recojet.delta_R(truthjet);
	  if(dR < mindR)
	    {
	      mindR = dR;
	      matchedTruthJet = truthjet;
	      
	      if(m_verbosity > 1)
		std::cout << "Found matched jet with dr " << mindR<<std::endl;
	    }
	}
      
      if(m_verbosity > 1)
	{
	  std::cout << "recojet: " << recojet.px() << "  " << recojet.py() 
		    << "  " << recojet.pz() << "  " << recojet.e() << std::endl;
	  std::cout << "truejet: " << matchedTruthJet.px() << "  " 
		    << matchedTruthJet.py() << "  " << matchedTruthJet.pz() 
		    << "  " << matchedTruthJet.e() << std::endl;
	}
      
      matchJetPair.push_back(matchedTruthJet);
      matchJetPair.push_back(recojet);
      matchedVector.push_back(matchJetPair);
    }

  return matchedVector;

}


