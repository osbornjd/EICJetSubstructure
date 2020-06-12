#!/bin/csh

setenv HOME /phenix/u/jdosbo

source /etc/csh.login
foreach i (/etc/profile.d/*.csh)
    source $i
end

source $HOME/.cshrc

setenv EIC_LEVEL dev

source /afs/rhic.bnl.gov/eic/restructured/etc/eic_cshrc.csh

# basepath argument
cd $2
cd analysisCode/macros

root -l analyzeJets.C\(\"$1\"\)
