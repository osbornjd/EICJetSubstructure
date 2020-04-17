#ifndef SOFTDROPJETDEF_H
#define SOFTDROPJETDEF_H

#include "JetDef.h"
#include <fastjet/contrib/SoftDrop.hh>

class SoftDropJetDef : public JetDef
{
 public:
  SoftDropJetDef(){}
  SoftDropJetDef(double zcut, double beta, double R)
    : JetDef()
    , m_R(R)
    , m_zcut(zcut)
    , m_beta(beta)
    , m_sd(new fastjet::contrib::SoftDrop(m_beta, m_zcut, m_R))
  {}
  ~SoftDropJetDef(){}

  void setZCut(float zcut){m_zcut = zcut;}
  void setBeta(float beta){m_beta = beta;}
  
  fastjet::contrib::SoftDrop getSoftDrop(){return *m_sd;}

 private:
  double m_R;
  double m_zcut;
  double m_beta;
  /// Must be a pointer because default SD constructor does not exist
  fastjet::contrib::SoftDrop *m_sd;

};


#endif
