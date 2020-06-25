# Simple python script that runs the pythiaeRHIC executable
# Tor the "official"
# EICSmear example see: 
# $EICDIRECTORY/PACKAGES/PYTHIA-RAD-CORR/STEER-FILES-Other/ep_noradcor.20x250.quicktest 

import os
import sys

def runPythia(outfile, protonEnergy, electronEnergy, minQ2, 
              nEvents, basePath, processID):
    """
    This function takes several arguments and runs the pythia truth generation
    with the parameters given as arguments and the "default file", which 
    contains a number of pythia parameters tuned to various HERA data.

    outfile        - output text file to contain the pythia events
    protonEnergy   - proton beam energy
    electronEnergy - lepton beam energy
    minQ2          - minimum Q2 of events
    nEvents        - number of events to process
    basePath       - basePath provided by runSimWorkflow.py 
                     to the jetSubstructure/ directory
    processID      - processId to identify output if running batch jobs
    """

    # Get the default pythia parameters that we don't want to fuss with
    # parameters tuned to HERA data
    defaultPythiaFile = open(basePath + "pythiaGeneration/pythiaeRHICSettingsHera.txt", "r")
    defaultPythia = defaultPythiaFile.read()
    
    # Construct the header with the arguments we do want to fuss with
    output = outfile + " ! output file name\n"
    leptonBeam = "11 ! lepton beam type\n"
    energies = protonEnergy + "," + electronEnergy + " ! proton and electron beam energy\n"
    events = nEvents + ",1 ! Number of events, number of events to print to stdout\n"
    
    # Write all the pythia gen info to a dummyfile to be passed 
    # to the erhic executable
    pythiafilename = "pythiafile_" + processID + ".txt"
    finalPythiaFile = open(pythiafilename,"w")
    finalPythiaFile.write(output)
    finalPythiaFile.write(leptonBeam)
    finalPythiaFile.write(energies)
    finalPythiaFile.write(events)
    
    # Add the x, q, and y ranges to run minimum bias pythia (other than minq2)
    finalPythiaFile.write("1e-09, 0.99       ! xmin and xmax\n")
    finalPythiaFile.write("1e-04,1.00        ! ymin and ymax\n")
    finalPythiaFile.write(minQ2 +",20000         ! Q2min and Q2max\n")
    
    # Add all the other (HERA tuned) parameters
    finalPythiaFile.write(defaultPythia)
    finalPythiaFile.close()
    
    # Execute the pythiaeRHIC generation executable in EICSmear
    executable = "$EICDIRECTORY/bin/pythiaeRHIC < " + pythiafilename + " > logfile_" + processID + ".log"
    os.system(executable)
    
    # Convert the ugly text file to a root tree with truth information
    import ROOT
    ROOT.gSystem.Load("libeicsmear")
    ROOT.BuildTree(outfile, ".", -1,"logfile_"+processID+".log")
    
    # Remove the dummy files we created once we are finished with it
    os.system("rm " + pythiafilename)
    os.system("rm " + outfile)
    os.system("rm logfile_"+processID+".log")
