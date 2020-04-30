#!/bin/csh

setenv HOME /phenix/u/jdosbo

source /etc/csh.login
foreach i (/etc/profile.d/*.csh)
    source $i
end

source $HOME/.cshrc

cd /sphenix/user/jdosbo/EICSmear

source setupEIC.csh

cd git/jetSubstructure

# base path is the directory path to your jetSubstructure git clone
# so that the various files can be accessed

echo "smeared file name: "$1
echo "truth file name: "$2
echo "proton beam energy: "$3
echo "electron beam energy: "$4
echo "Min q2: "$5
echo "nEvents: "$6
echo "base path: "$7

py runSimWorkflow.py $1 $2 $3 $4 $5 $6 $7
