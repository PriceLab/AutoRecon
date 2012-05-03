#CFLAGS = -O3 -fopenmp -fprefetch-loop-arrays -funroll-loops 
#CFLAGS = -g -O3 -fopenmp -Wfatal-errors -fprefetch-loop-arrays -funroll-loops
CFLAGS = -g -fopenmp -fPIC

# Create /obj/ directory if none exists
# Thanks to stackoverflow for this bit of makefile magic...
# see http://stackoverflow.com/questions/652524/how-do-i-create-a-directory-in-a-makefile
TOPDIR := $(shell pwd)
OBJDIR := $(TOPDIR)/obj
$(shell [ -d "$(OBJDIR)" ] || mkdir -p $(OBJDIR))

# Compiler, object, library, and header definitions...
CC = g++
LIBDIRS = -Llib -L/usr/lib64 -L/usr/lib64/mysql `pkg-config --libs libxml-2.0` `pkg-config --libs gsl`
LIBS =  -lglpk -lmysqlclient -lgsl -lautorecon
INCLUDES =-Isrc `pkg-config --cflags libxml-2.0` -I/usr/include/glpk
OBJS = obj/DataStructures.o obj/XML_loader.o \
       obj/shortestPath.o obj/kShortest.o obj/pathUtils.o \
       obj/RunK.o obj/visual01.o obj/Grow.o obj/Exchanges.o \
       obj/ETC.o obj/Modularity.o obj/Components.o obj/genericLinprog.o \
       obj/Printers.o obj/Paths2Model.o obj/Annotations.o obj/MyConstants.o \
       obj/score.o obj/TableLoader.o
       
HDRS =  src/DataStructures.h src/Grow.h src/pathUtils.h \
        src/RunK.h src/visual01.h src/kShortest.h src/shortestPath.h src/XML_loader.h \
        src/Exchanges.h src/ETC.h src/Modularity.h src/Components.h src/genericLinprog.h \
	src/Printers.h src/Paths2Model.h src/Annotations.h src/MyConstants.h src/score.h src/TableLoader.h

LIB_AR = lib/libautorecon.so
lib/libautorecon.so: $(OBJS) 
	$(CC) -shared -Wl,-soname,libautorecon.so -o $(LIB_AR) $(OBJS)
	        

all: FbaTester-NC FbaTester

# Note - you require a "cc" file to compile into an object file using this command...will it work without one?
obj/%.o: src/%.cc $(HDRS)
	$(CC) -c $< $(CFLAGS) $(INCLUDES) -o $@ 

clean:
	rm obj/*.o $(LIB_AR)

clout:
	rm outputs/*

# Every program should be listed here (follow example):

FbaTester: obj/zFbaTester.o $(LIB_AR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ obj/zFbaTester.o $(LIBDIRS) $(LIBS)

FbaTester-NC: obj/zFbaTester-NC.o $(LIB_AR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ obj/zFbaTester-NC.o $(LIBDIRS) $(LIBS)

PathUtilsTester: obj/zPathUtilsTester.o $(LIB_AR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(LIB_AR) obj/zPathUtilsTester.o $(LIBS)

ETC_check-NC: obj/zETC_check-NC.o $(LIB_AR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ obj/zETC_check-NC.o $(LIBS)

BottleneckTester-MB: obj/zBottleneckTester-MB.o $(LIB_AR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ obj/zBottleneckTester-MB.o $(LIBS)

MfaTester: obj/zShortestMfaTester.o $(LIB_AR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ obj/zShortestMfaTester.o $(LIBDIRS) $(LIBS)

VisualTester: obj/zVisualTester.o $(LIB_AR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ obj/zVisualTester.o $(LIBDIRS) $(LIBS)

ModularityCheck-NC: obj/zModularityCheck.o $(LIB_AR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ obj/zModularityCheck.o $(LIBDIRS) $(LIBS)

TableTester: obj/zNewRxnTableTester.o $(LIB_AR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ obj/zNewRxnTableTester.o $(LIBDIRS) $(LIBS)