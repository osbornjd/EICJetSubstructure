#!/bin/csh


echo "processing truth file : "$1
echo "processing smear file : "$2
echo "output file : "$3
echo "lab/breit : "$4
echo "global path to analysisCode directory : "$5

setenv EIC_LEVEL dev
source /afs/rhic.bnl.gov/eic/restructured/etc/eic_cshrc.csh

cd $5
pwd
make all

./EventLoop $1 $2 $3 $4

cd macros

root -l analyzeJets.C\(\"../$3\"\)
