

#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

#include <iostream>


using TLorentzVectorVec = std::vector<TLorentzVector>;
/// A JetConstPair is a pair of a jet 4 vector and vector of jet constituents
using JetConstPair = std::pair<TLorentzVector, std::vector<TLorentzVector>>;
using JetConstVec = std::vector<JetConstPair>;

void setupTree();
void instantiateHistos();
void loop();
void write();

TLorentzVector *truthExchangeBoson;
JetConstVec *truthJets, *recoJets, *recoSDJets;
std::vector<std::vector<JetConstPair>> *matchedJets, *matchedSDJets;
double recx, recy, recq2, truey, truex, trueq2;

TFile *infile, *outfile;
TTree *jettree;
TH2 *truerecx, *truerecy, *truerecq2;
TH2 *trueQ2x, *trueQ2pT;
TH2 *truejetpteta, *truejetptphi;


const int nxbins = 41;
const int nq2bins = 93;
const int nptbins = 14;

float ptbins[nptbins+1]={4,5,6,7,8,9,10,11,12,14,16,18,20,24,30};

float xbins[nxbins+1] = {0.0001,0.0002,0.0003,0.0004,0.0005,0.0006,0.0007,
			 0.0008,0.0009,0.001,0.002,0.003,0.004,0.005,0.006,
			 0.007,0.008,0.009,0.01,0.015,0.02,0.03,0.04,0.05,0.06,0.07,
			 0.08,0.09,0.1,0.14,0.18,0.22,0.26,0.3,0.35,0.4,0.45,
			 0.5,0.6,0.7,0.8,0.9};

float qbins[nq2bins+1] = {9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
			  26,27,28,29,30,31,32,33,34,35,36,37,38,40,42,44,46,
			  48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,
			  85,90,95,100,110,120,130,140,150,160,170,180,190,200,
			  210,220,230,240,250,260,270,280,290,300,310,320,330,340,
			  350,375,400,425,450,475,500,525,550,575,600,625,650,675,700};
