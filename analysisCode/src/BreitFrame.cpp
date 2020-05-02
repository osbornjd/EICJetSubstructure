#include "include/BreitFrame.h"

void labToBreit( TLorentzVector *l, erhic::EventPythia* tr_ev )
{

  TLorentzVector q_lab = tr_ev->ExchangeBoson()->PxPyPzE();
  TLorentzVector P_lab = tr_ev->BeamHadron()->PxPyPzE();
  double E_breit =  2*tr_ev->GetTrueX()*P_lab.E() + q_lab.E();
  TVector3 beta(  2*tr_ev->GetTrueX()*P_lab.Vect() + q_lab.Vect() );
  beta *= 1/E_breit;
  TVector3 z_axis(0,0,1);
  l->Boost(-beta);
  q_lab.Boost(-beta);
  // Double check signage or if inverse rotation is needed... 
  l->Rotate(q_lab.Vect().Angle(z_axis), q_lab.Vect().Cross(z_axis).Unit());

}
