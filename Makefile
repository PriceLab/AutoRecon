#CFLAGS = -O3 -fopenmp -fprefetch-loop-arrays -funroll-loops 
#CFLAGS = -g -O3 -fopenmp -Wfatal-errors -fprefetch-loop-arrays -funroll-loops
CFLAGS = -g -fopenmp -fPIC

CC = g++
LIBS = `pkg-config --libs libxml-2.0` -lglpk `pkg-config --libs gsl` -lmysqlclient
LIBDIRS = -Llib -L/usr/lib64/mysql
INCLUDES =-Isrc `pkg-config --cflags libxml-2.0` -I/usr/include/glpk -I/usr/include/mysql

OBJS = obj/DataStructures.o obj/XML_loader.o obj/shortestPath.o obj/kShortest.o \
       obj/pathUtils.o obj/RunK.o obj/visual01.o obj/Grow.o obj/Exchanges.o \
       obj/ETC.o obj/Modularity.o obj/Components.o obj/genericLinprog.o obj/Printers.o \
       obj/Paths2Model.o obj/Annotations.o obj/MyConstants.o obj/score.o obj/TableLoader.o \
	obj/vector_helpers.o \
       obj/Metabolite.o obj/Reaction.o obj/Stoich.o obj/PathStuff.o
       
HDRS = src/DataStructures.h src/Grow.h src/pathUtils.h src/RunK.h src/visual01.h \
	src/kShortest.h src/shortestPath.h src/XML_loader.h src/Exchanges.h src/ETC.h \
	src/Modularity.h src/Components.h src/genericLinprog.h src/Printers.h \
	src/Paths2Model.h src/Annotations.h src/MyConstants.h src/score.h \
	src/TableLoader.h \
	src/vector_helpers.h \
        src/Metabolite.h src/Reaction.h src/Stoich.h src/PathStuff.h


all: FbaTester-NC FbaTester

# Note - you require a "cc" file to compile into an object file using this command...will it work without one?
obj/%.o: src/%.cc $(HDRS)
	$(CC) -c $< $(CFLAGS) $(INCLUDES) -o $@ 

clean:
	rm -f obj/*.o lib/*.so

clout:
	rm outputs/*

lib/libautorecon.so: $(OBJS)
	@echo
	@echo Making shared autorecon.so:
	$(CC) -shared -Wl,-soname,libautorecon.so -o lib/libautorecon.so $(OBJS)
	@echo

# Every program should be listed here (follow example):

FbaTester: obj/zFbaTester.o lib/libautorecon.so
#	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJS) obj/zFbaTester.o $(LIBS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ obj/zFbaTester.o $(LIBDIRS) $(LIBS) -lautorecon

FbaTester-NC: obj/zFbaTester-NC.o lib/libautorecon.so
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ obj/zFbaTester-NC.o $(LIBS) -l autorecon

PathUtilsTester: obj/zPathUtilsTester.o $(OBJS)
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} obj/zPathUtilsTester.o ${LIBS}

ETC_check-NC: obj/zETC_check-NC.o $(OBJS)
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} obj/zETC_check-NC.o ${LIBS}

BottleneckTester-MB: obj/zBottleneckTester-MB.o $(OBJS)
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} obj/zBottleneckTester-MB.o ${LIBS}

MfaTester: obj/zShortestMfaTester.o $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJS) obj/zShortestMfaTester.o $(LIBS)

VisualTester: obj/zVisualTester.o $(OBJS)
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} obj/zVisualTester.o ${LIBS}

ModularityCheck-NC: obj/zModularityCheck.o $(OBJS)
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} obj/zModularityCheck.o ${LIBS}

TableTester: obj/zNewRxnTableTester.o $(OBJS)
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} obj/zNewRxnTableTester.o ${LIBS}

DataStore: obj/zDataStore.o $(OBJS) lib/libautorecon.so
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ obj/zDataStore.o $(LIBDIRS) $(LIBS) -lautorecon

