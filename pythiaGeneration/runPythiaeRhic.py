# Simple python script that runs the pythiaeRHIC executable
# See the enclosed directories for examples, or for the "official"
# EICSmear example see: 
# $EICDIRECTORY/PACKAGES/PYTHIA-RAD-CORR/STEER-FILES-Other/ep_noradcor.20x250.quicktest 

# arguments: 
# output text file to contain the pythia events
# proton beam energy
# lepton beam energy
# number of events to process


import os
import sys

def runPythia(outfile, protonEnergy, electronEnergy, minQ2, nEvents, basePath):
    """
    This function takes several arguments and runs the pythia truth generation
    with the parameters given as arguments and the "default file", which 
    contains a number of pythia parameters tuned to various data.
    outfile        - string of truth output root file
    protonEnergy   - proton beam energy
    electronEnergy - electron beam energy
    nEvents        - number of events to run
    """

    # get the default pythia parameters that we don't want to fuss with
    defaultPythiaFile = open(basePath + "pythiaGeneration/pythiaeRHICSettingsHera.txt", "r")
    defaultPythia = defaultPythiaFile.read()
    
    # construct the header with the arguments 
    output = outfile + " ! output file name\n"
    leptonBeam = "11 ! lepton beam type\n"
    energies = protonEnergy + "," + electronEnergy + " ! proton and electron beam energy\n"
    events = nEvents + ",1 ! Number of events, number of events to print to stdout\n"
    
    # write all the stuff to a dummyfile to be passed to the erhic executable
    finalPythiaFile = open("pythiafile.txt","w")
    finalPythiaFile.write(output)
    finalPythiaFile.write(leptonBeam)
    finalPythiaFile.write(energies)
    finalPythiaFile.write(events)
    
    # add the x, q, and y, which we might want to add as arguments later
    finalPythiaFile.write("1e-09, 0.99       ! xmin and xmax\n")
    finalPythiaFile.write("1e-04,1.00        ! ymin and ymax\n")
    finalPythiaFile.write(minQ2 +",20000         ! Q2min and Q2max\n")
    
    # add all the other parameters
    finalPythiaFile.write(defaultPythia)
    finalPythiaFile.close()
    
    executable = "$EICDIRECTORY/bin/pythiaeRHIC < pythiafile.txt"
    os.system(executable)
    
    # Convert the ugly text file to a root tree with truth information
    import ROOT
    ROOT.gSystem.Load("libeicsmear")
    ROOT.BuildTree(outfile, ".", -1)
    
    # remove the files we created once we are finished with it
    os.system("rm pythiafile.txt")
    os.system("rm " + outfile)
    
