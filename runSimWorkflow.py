import os
import sys
import subprocess

# This script runs the whole workflow and generates, and smears, pythia events
# The output are two files in the directory where you run this script
# given by the arguments collected for the truth and smeared files. These
# root files can be "befriended" for analysis. 
# To run this, it is expected you have the correct EIC environment setup 
# on RCF by sourcing the eic .csh setup script

# To use this file, change the below python path insert to the directory
# paths the top directory which contains this script. Needs to end in /
basePath = '/direct/phenix+u/workarea/dillfitz/EIC/EICJetSubstructure/'


# set the python path to include the subdirectories with the other scripts
sys.path.insert(1, basePath + 'pythiaGeneration/')
sys.path.insert(1, basePath + 'smear/')

import runPythiaeRhic
import smearTruth


# collect arguments
smearedfile = sys.argv[1]
truthfile = sys.argv[2]
protonEnergy = sys.argv[3]
electronEnergy = sys.argv[4]
minQ2 = sys.argv[5]
nEvents = sys.argv[6]


# generate events
runPythiaeRhic.runPythia(truthfile,
                         protonEnergy,
                         electronEnergy,
                         minQ2,
                         nEvents,
                         basePath)

# run smearing
smearTruth.smear(truthfile+".root", smearedfile+".root", basePath)
