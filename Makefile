#CFLAGS = -O3 -fopenmp -fprefetch-loop-arrays -funroll-loops 
#CFLAGS = -g -O3 -fopenmp -Wfatal-errors -fprefetch-loop-arrays -funroll-loops
CFLAGS = -g -fopenmp -Wfatal-errors 

# Create /obj/ directory if none exists
# Thanks to stackoverflow for this bit of makefile magic...
# see http://stackoverflow.com/questions/652524/how-do-i-create-a-directory-in-a-makefile
TOPDIR := $(shell pwd)
OBJDIR := $(TOPDIR)/obj
$(shell [ -d "$(OBJDIR)" ] || mkdir -p $(OBJDIR))


# Compiler, object, library, and header definitions...
CC = g++
LIBS = -ljsoncpp
INCLUDES = -I /opt/local/include/ \
      -I /usr/include -I /usr/local/include 
OBJS = obj/pathUtils.o obj/Problem.o \
       obj/Models.o obj/AliasSet.o obj/Biochemistry.o obj/Biomass.o \
       obj/Compartment.o obj/Compound.o obj/Cue.o obj/Media.o obj/ModelCompartment.o \
       obj/ModelCompound.o obj/ModelReaction.o obj/Reaction.o obj/Reagent.o
       
HDRS =  src/pathUtils.h src/Problem.h \
	src/Models.h src/AliasSet.h src/Biochemistry.h src/Biomass.h \
	src/Compartment.h src/Compound.h src/Cue.h src/Media.h src/ModelCompartment.h \
	src/ModelCompound.h src/ModelReaction.h src/Reaction.h src/Reagent.h
        

all: Main

# Note - you require a "cc" file to compile into an object file using this command...will it work without one?
obj/%.o: src/%.cc $(HDRS)
	$(CC) -c $< $(CFLAGS) $(INCLUDES) -o $@ 

# Every program should be listed here (follow example):

Main: obj/main.o $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJS) obj/main.o $(LIBS)

clean:
	rm obj/*.o

clout:
	rm outputs/*

