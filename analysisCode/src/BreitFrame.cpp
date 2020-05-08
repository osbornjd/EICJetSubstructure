#include "include/BreitFrame.h"

void BreitFrame::labToBreitTruth( TLorentzVector *l )
{

  TLorentzVector q_lab = m_truthEvent->ExchangeBoson()->PxPyPzE();
  TLorentzVector P_lab = m_truthEvent->BeamHadron()->PxPyPzE();
  double E_breit =  2*m_truthEvent->GetTrueX()*P_lab.E() + q_lab.E();
  TVector3 beta(  2*m_truthEvent->GetTrueX()*P_lab.Vect() + q_lab.Vect() );
  beta *= 1/E_breit;
  TVector3 z_axis(0,0,-1);
  l->Boost(-beta);
  q_lab.Boost(-beta);
  // Double check signage or if inverse rotation is needed... 
  l->Rotate(q_lab.Vect().Angle(z_axis), q_lab.Vect().Cross(z_axis).Unit());

}

void BreitFrame::labToBreitSmear( TLorentzVector *l )
{
  TLorentzVector lprime(m_smearEvent->ScatteredLepton()->Get4Vector());
  TLorentzVector lo(m_smearEvent->BeamLepton()->Get4Vector());

  TLorentzVector q = lo - lprime;

  TLorentzVector P = m_smearEvent->BeamHadron()->PxPyPzE();

  double E_breit =  2*m_smearEvent->GetX()*P.E() + q.E();
  TVector3 beta(  2*m_smearEvent->GetX()*P.Vect() + q.Vect() );
  beta *= 1/E_breit;
  TVector3 z_axis(0,0,-1);
  l->Boost(-beta);
  q.Boost(-beta);
  // Double check signage or if inverse rotation is needed... 
  l->Rotate(q.Vect().Angle(z_axis), q.Vect().Cross(z_axis).Unit());

}
