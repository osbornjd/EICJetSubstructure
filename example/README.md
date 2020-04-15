## Analysis example

This gives an example analysis macro that will take in a truth and smeared root file as produced by `runSimWorkflow.py` and then prints out some 4 vectors of the particles in the event and create some jets. You can build the example by running 

```
$ make all
$ ./standalone
```

The macro doesn't do anything except print stuff out. One could in principle setup a macro that collects information into tuples and then writes them out to root files for further analysis.