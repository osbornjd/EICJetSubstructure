#ifndef JETDEF_H
#define JETDEF_H

#include <fastjet/ClusterSequence.hh>
#include <fastjet/Selector.hh>

#include <vector>

class JetDef {
  
 public:
  JetDef(){}
  JetDef(fastjet::JetAlgorithm alg, double R)  
    : m_R(R)
    , m_jetAlg(alg)
    , m_jetDef(alg, R)   
  {}
  ~JetDef(){}
  
  void setRadius(float rad){m_R = rad;}
  void setMinJetPt(float jetPt){m_minJetPt = jetPt;}
  void setMinJetRapidity(float rap){m_absRapidity = rap;}
  void setJetAlg(fastjet::JetAlgorithm alg){m_jetAlg = alg;}
  void setJetDef(fastjet::JetDefinition def){m_jetDef = def;}
  void setRecombScheme(fastjet::RecombinationScheme scheme){m_scheme = scheme;}

 protected:
  double m_R;
  double m_minJetPt;
  double m_absRapidity;

  /// Fastjet jet definition
  fastjet::JetAlgorithm m_jetAlg;
  fastjet::JetDefinition m_jetDef;
  /// Default to 4 vector e recombination scheme
  fastjet::RecombinationScheme m_scheme = fastjet::RecombinationScheme::E_scheme;
};


#endif
