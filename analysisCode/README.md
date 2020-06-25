## Analysis Code

This directory holds the code that is responsible for analyzing the output truth and smeared files that were created by EICSmear. The source code is collected in the `src` directory, and macros to analyze the output are stored in `macros` directory. Note that this has only been tested on RCF and thus it is assumed you are working on an RCF account with the EICSmear libraries available. 


## Building and running

All that is required to build the code is to use the `Makefile` via `make all`. The build can be cleand with `make clean`. Building the code prodcues an executable, which can be run with `./EventLoop`. As indicated in `src/EventLoop.cpp`, the executable takes 4 arguments. An example to run it over the dummy 1 event EICSmear output is:

```
./EventLoop ../MCData/example/truth.root ../MCData/example/smeared.root outputfile.root 0
```

The `truth` and `smeared` are the relative paths to the truth and smeared output as generated from EICSmear after running `runSimWorkflow.py`. The `outputfile` is the output ROOT file that contains a variety of jet trees. The final argument should either be a 0 or 1 depending on whether or not you want to boost the final state particles and do the jet clustering in the Breit frame (1) or the lab frame (0).

You can also take advantage of the `RunAnalysisChain.csh` script, which runs the analysis code and associated macro to generate histograms.


## ROOT output

The output ROOT file has several trees in it. For descriptions of the data types that each tree holds, see `src/include/EventLoop.h` and `setupJetTree()` in `EventLoop.cpp`. 

## Macros

A macro to analyze the output from running `./EventLoop` can be found in `macros/analyzeJets.C`. The macro simply takes as an argument the output ROOT file with the jet trees produced by `./EventLoop`. The final output is a variety of histograms, as described in `macros/HistoManager.h`. Additionally, the macro `macros/AnalyzeHistos.C` can be run on the output of `analyzeJets.C` to create some plots.