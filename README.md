# EIC jet substructure 

This repository is intended to setup a software workflow for using the EIC smear package to generate pythia truth MC and "smear" it using a fast detector simulation. 


### Requirements

This has only been tested on RCF, where access to the EIC environment is present. In order to use this, it is expected you have setup this environment via

```
$ setenv EIC_LEVEL pro 
$ source /afs/rhic.bnl.gov/eic/restructured/etc/eic_cshrc.csh

```

In principle, if you have EICSmear and ROOT installed on your local computer with a similar environment setup to the RCF version, this code would be functional. 

### Usage
All that is necessary to use this is to run the main `runSimWorkflow.py` with the arguments as documented in the file. Alternatively, there is a `.csh` script in `condorEventGen` that runs the same thing - this may be more intuitive for some users. The output is a truth root file, which contains a tree with truth particle information from PYTHIA, and a smeared root file, which contains the particles after undergoing detector smearing.

The detector smearing is set in `smear/ePHENIXDetector.cpp` and `smear/smearHandBook.cxx`.

The output truth and smeared files can be befriended for further analysis within ROOT, as in the directory `analysisCode`.

### Repository setup

The repository has three main analysis steps - event generation, event analysis, and macro. Each step and the relevant location of the code are described in more detail here. An example single DIS event truth and smeared output can be found in `MCData/example`, which corresponds to the event generation output for 1 event. In principle, one could run the analysis chain on this example output (although you won't get any interesting results!).

#### Event generation

The event generation is controlled by the `runSimWorkflow.py` script, which calls separate python scripts in the directories `pythiaGeneration` and `smear` to do the truth event generation and smearing, respectively. For those working on RCF who have access to the condor node farm, condor scripts are available to run the event generation in `condorEventGen`. 
#### Event analysis

After a truth and smeared file are created, the code to analyze the output events is available in the `analysisCode` directory. Please see that directory for another README describing its use. A basic example analysis code with Makefile can be found in the `example` directory, should you want to create your own analysis code.

#### Macros

Macros to draw histograms from the output event analysis trees are stored in `analysisCode/macros`

### Other notes

Don't be alarmed if you see "Error in <TROOT::TVector2>" type errors in the smearing. The smearing process sometimes removes a particle completely (e.g. for efficiency loss) and thus root complains when trying to smear a nonexistent particle.

### Example

To execute the runSimWorkflow.py, you can run the following:

python runSimWorkflow.py smeared truth 100 10 9 1000 /base/path/to/your/jetSubstructure 0

This corresponds to running the workflow and generating a smeared file called “smeared.root”, a truth file called “truth.root”, proton energy = 100 GeV, electron energy = 10 Gev, minimum Q2=9 GeV, and for 1000 events. The path argument is the base path on RCF to wherever you cloned this jetSubstructure repository - for example, `/User/joe/git/jetSubstructure`. The final value is an arbitrary number indicating the process ID - this is relevant for running on condor when running multiple batch jobs.

### Condor Submission

RCF also has a condor batch processing system available. Condor submission jobs are available in the `condorEventGen` directory. The `csh` script can be run with similar arguments to the python `runSimWorkflow.py` script. The condor `.job` file is used to submit jobs to condor and can be used with the usual condor calls. See also the README in the directory `condorEventGen` directory.


### Additional information

For additional information regarding EICSmear, see [here](https://gitlab.com/eic/eic-smear/).