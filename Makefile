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
LIBS = `pkg-config --libs libxml-2.0` -lglpk `pkg-config --libs gsl` \
      -lm -lpthread -ljsoncpp	
INCLUDES =-Isrc `pkg-config --cflags libxml-2.0` -I /opt/local/include/ \
      -I /usr/include -I /usr/local/include 
OBJS = obj/DataStructures.o obj/XML_loader.o \
       obj/shortestPath.o obj/kShortest.o obj/pathUtils.o \
       obj/RunK.o obj/Exchanges.o \
       obj/Modularity.o  \
       obj/Printers.o obj/MyConstants.o \
       obj/TableLoader.o obj/FileLoader.o obj/Models.o 
       
HDRS =  src/DataStructures.h src/pathUtils.h \
        src/RunK.h src/kShortest.h src/shortestPath.h \
	src/XML_loader.h src/Exchanges.h src/Modularity.h \
	src/Printers.h \
	src/MyConstants.h \
	src/TableLoader.h src/FileLoader.h src/Models.h 
        

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

