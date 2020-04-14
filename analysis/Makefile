#-----------------------------------------------------------------------------

PROGRAM = ExampleMacro

HDRS = ExampleMacro.h\
 
OBJS = ExampleMacro.o\
        dict_$(PROGRAM).o

#-----------------------------------------------------------------------------
ObjSuf = o
SrcSuf = cpp
ExeSuf = exe
DllSuf = so
OutPutOpt = -o

ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --libs)
ROOTGLIBS = $(shell root-config --glibs)

FASTJETCFLAGS = $(shell fastjet-config --cxxflags)
FASTJETLIBS = $(shell fastjet-config --libs)
FASTJETPLUGINS = $(shell fastjet-config --plugins)

# Directories
HEADRDIR = $(ROOT_INCLUDE_PATH)

# debug option: -g
# no exceptions: -fno-exceptions

CXX = g++
CXXFLAGS = -O3 -Wall -g -std=c++11
LD = g++
LDFLAGS = -O3 -Wall -g
SOFLAGS = -shared
ARCHFLAGS =

EXTRALIBS = $(shell fastjet-config --cxxflags --libs --plugins)
# EXTRALIBS += -lRecursiveTools
EXTRALIBS = -L/afs/rhic.bnl.gov/eic/restructured/env/pro/lib -leicsmear -lfastjet -lfastjettools -lRecursiveTools
LIBS = $(ROOTLIBS) 
LIBS += $(EXTRALIBS)
CXXFLAGS += $(ROOTCFLAGS) -I$(HEADRDIR) $(LIBS)
CXXFLAGS += -Wno-unused-variable -Wno-sign-compare

GLIBS = $(ROOTGLIBS)

all: $(PROGRAM).so standalone

%/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $*.cpp
 
$(PROGRAM).so: $(OBJS)
	@echo "Linking $(PROGRAM).so ..."
	$(LD) $(ARCHFLAGS) $(LDFLAGS) $(SOFLAGS) $(OBJS) $(ROOTLIBS) -o$(PROGRAM).so $(EXTRALIBS) 
	@echo "linking done"

standalone: standalone.o
	@echo "Linking standalone ..."
	$(LD) $(ARCHFLAGS) $(LDFLAGS) standalone.o $(ROOTLIBS) $(EXTRALIBS) -o standalone
	@echo "linking done"

dict_$(PROGRAM).cpp: $(HDRS)
	@echo "Generating dictionary ..."
	rootcint -f dict_$(PROGRAM).cpp -c -p -I$(HEADRDIR) $(HDRS) Linkdef.h 

clean:
	@rm -f $(OBJS) $(PROGRAM).so dict_* *.o
 

#------------------------------------------------------------------------
