SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic
CCC = g++
BASEFOLDER =  /opt/ibm/ILOG
CPLEXVERSION = CPLEX_Studio201
ifeq ($(myOS), MacOSX)
	SYSTEM     = x86-64_osx
	LIBFORMAT  = static_pic
	CCC = clang++
	BASEFOLDER = /Applications
endif

CPLEXDIR       = $(BASEFOLDER)/$(CPLEXVERSION)/cplex
CONCERTDIR     = $(BASEFOLDER)/$(CPLEXVERSION)/concert

CCOPT = -m64 -O3 -w -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD -std=c++11

CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CPLEXINCDIR   = $(CPLEXDIR)/include
CONCERTINCDIR = $(CONCERTDIR)/include
CCFLAGS = $(CCOPT) -I$(CONCERTINCDIR) -I$(CPLEXINCDIR) 
CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR)
CCLNFLAGS = -lconcert -lilocplex -lcplex -lm -pthread -ldl
TARGETDIR = .




all : genvrp

# CCC = clang++
# CCFLAGS = -O3 -Wall -std=c++11 
# TARGETDIR=.

OBJS2 = \
        $(TARGETDIR)/main.o \
        $(TARGETDIR)/Genetic.o \
        $(TARGETDIR)/Individual.o \
        $(TARGETDIR)/LocalSearch.o \
        $(TARGETDIR)/Params.o \
        $(TARGETDIR)/Population.o \
        $(TARGETDIR)/Split.o

$(TARGETDIR)/genvrp: $(OBJS2)
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o $(TARGETDIR)/genvrp $(OBJS2) $(CCLNFLAGS)
	
$(TARGETDIR)/Genetic.o: Genetic.h Genetic.cpp
	$(CCC) $(CCFLAGS) -c Genetic.cpp -o $(TARGETDIR)/Genetic.o

$(TARGETDIR)/Individual.o: Individual.h Individual.cpp
	$(CCC) $(CCFLAGS) -c Individual.cpp -o $(TARGETDIR)/Individual.o

$(TARGETDIR)/LocalSearch.o: LocalSearch.h LocalSearch.cpp
	$(CCC) $(CCFLAGS) -c LocalSearch.cpp -o $(TARGETDIR)/LocalSearch.o
	
$(TARGETDIR)/main.o: main.cpp
	$(CCC) $(CCFLAGS) -c main.cpp -o $(TARGETDIR)/main.o

$(TARGETDIR)/Params.o: Params.h Params.cpp
	$(CCC) $(CCFLAGS) -c Params.cpp -o $(TARGETDIR)/Params.o

$(TARGETDIR)/Population.o: Population.h Population.cpp
	$(CCC) $(CCFLAGS) -c Population.cpp -o $(TARGETDIR)/Population.o

$(TARGETDIR)/Split.o: Split.h Split.cpp
	$(CCC) $(CCFLAGS) -c Split.cpp -o $(TARGETDIR)/Split.o

test: genvrp
	./genvrp ../Instances/CVRP/X-n101-k25.vrp mySolution.sol -seed 1 

clean:
	$(RM) \
    $(TARGETDIR)/main.o \
    $(TARGETDIR)/Genetic.o \
    $(TARGETDIR)/Individual.o \
    $(TARGETDIR)/LocalSearch.o \
    $(TARGETDIR)/Params.o \
    $(TARGETDIR)/Population.o \
    $(TARGETDIR)/Split.o \
    $(TARGETDIR)/genvrp
     