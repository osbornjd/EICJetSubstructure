## Analysis example

This gives an example analysis code that will take in a truth and smeared root file as produced by `runSimWorkflow.py` and then prints out some 4 vectors of the particles in the event and create some jets. You can build the example by running 

```
$ make all
$ ./standalone
```

The code doesn't do anything except print stuff out and is intended to only be a skeleton of how to work with EICSmear at first. One could in principle setup code that collects information into tuples and then writes them out to root files for further analysis.