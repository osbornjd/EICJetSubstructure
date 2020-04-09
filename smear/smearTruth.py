# Short script to smear out the truth pythia MC. Smears with default 
# detector based around sPHENIX solenoid
# take an input file (truth pythia file) and output file (smeared output)
# as arguments

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
    ROOT.gROOT.ProcessLine(".L " + basePath + "smear/ePHENIXDetector.cpp")
    ROOT.gROOT.ProcessLine("SmearTree(BuildEphoenix(true), \""+inputfile+"\", \""+outputfile+"\")")
