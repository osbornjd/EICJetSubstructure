ObjSuf = o
SrcSuf = cpp
ExeSuf = exe
DllSuf = so
OutPutOpt = -o

# Define the root and fastjet flags, in case we need them
ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --libs)
ROOTGLIBS = $(shell root-config --glibs)
FASTJETFLAGS = $(shell fastjet-config --cxxflags --libs --plugins)

# Directories
HEADRDIR = $(ROOT_INCLUDE_PATH)

# debug option: -g
# no exceptions: -fno-exceptions

# common g++ flags to compile with
CXX = g++
CXXFLAGS = -O3 -Wall -g -std=c++11
LD = g++
LDFLAGS = -O3 -Wall -g
ARCHFLAGS =

# extra libraries in eic install directory
EXTRALIBS = -L/afs/rhic.bnl.gov/eic/restructured/env/pro/lib -leicsmear -lfastjet -lfastjettools -lRecursiveTools

# set up the compile flags and libraries
LIBS = $(ROOTLIBS) 
LIBS += $(EXTRALIBS)
CXXFLAGS += $(ROOTCFLAGS) -I$(HEADRDIR) $(LIBS)
CXXFLAGS += -Wno-unused-variable -Wno-sign-compare

# to add a class just add the corresponding .o library here
MODULES = src/EventLoop.o src/TruthEvent.o src/SmearedEvent.o


all: EventLoop

EventLoop: $(MODULES)
	@echo "Linking ..."
	$(LD) $(ARCHFLAGS) $(LDFLAGS) $(MODULES) $(ROOTLIBS) $(EXTRALIBS) -o EventLoop 
	@echo "linking done"

clean:
	@rm -rf $(OBJS) $(PROGRAM) src/*.o EventLoop


#------------------------------------------------------------------------
