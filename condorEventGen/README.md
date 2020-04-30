### Condor job running

These scripts allow the user to submit jobs to run the simulation workflow on condor, available through RCF. You can alternatively run a single job with the `.csh` script.

### Changes that must be made to scripts
The scripts have some hard coded paths that are necessary for condor to be able to find things. Therefore a few things must be changed if you want to use the scripts.

1. In `run_runSimWorkflow.csh`, change the `HOME` environment variable to your own `HOME` directory.
2. In `run_runSimWorkflow.job`, change the `basepath` variable to your own directory where the `jetSubstructure` repository was cloned