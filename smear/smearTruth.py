# Short script to smear out the truth pythia MC. Smears with default 
# handbook detector as defined in smearHandBook.cxx
# Note: smearHandBook.cxx needs to be updated as it is updated by the 
# detector working group, as it is just checked out of the EICSmear package
# here
import ROOT
import sys


def smear(inputfile, outputfile, basePath):
    """
    This function takes an inputfile and outputfile as arguments and runs the
    smearing algorithm
    inputfile  - string of input truth pythia root file, containing EICTree
    outputfile - string of smeared output file, to be "befriended" by truth file
    basePath   - the full path leading to the jetSubstructure directory,
                 defined in runSimWorkflow.py
    """

    # process commands in root
    ROOT.gSystem.Load("libeicsmear")
    ROOT.gROOT.ProcessLine(".L " + basePath + "smear/SmearMatrixDetector_0_1.cxx")
    ROOT.gROOT.ProcessLine("SmearTree(BuildMatrixDetector_0_1(), \""+inputfile+"\", \""+outputfile+"\")")
