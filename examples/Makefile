include ../CFDProcessing.conf

INC = -I../mathlib -I../datalib
BIN = 
LIB = 
CPPFLAGS = $(GCPPFLAGS) $(INC) -D$(ARCH)
LDFLAGS =$(GLDFLAGS)  $(LIB)
LIBS=  -lm 

OBJECTS =
PROGNAME= $(APPL)
MODULES = ../segmentationlib/segmentationlib.a  ../reconstructionlib/reconstruction.a  ../mathlib/mathlib.a ../segmentationlib/segmentationlib.a   ../datalib/datalib.a  ../cfdlib/cfdlib.a 


all:	shellReconstruction 

.c.o: 
	cc $(CFLAGS) $< 

.cpp.o:
	g++ $(CPPFLAGS) $<

vectortest:  vectortest.o
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	make -C ../reconstructionlib
	g++  vectortest.o $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS)  -o vectortest




cfdfilter:  cfdfilter.o
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	make -C ../reconstructionlib
	g++  cfdfilter.o  $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS) -o cfdfilter


segmentation:  segmentation.o
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	make -C ../reconstructionlib
	make -C ../segmentationlib
	g++  segmentation.o $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS)  -o segmentation




reconstruction:  reconstruction.o
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	make -C ../reconstructionlib
	g++  reconstruction.o $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS)  -o reconstruction

shellReconstruction: shellReconstruction.o
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	make -C ../reconstructionlib
	g++  shellReconstruction.o $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS)  -o shellReconstruction

inttest: inttest.o $(OBJECTS) 
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	g++ inttest.o $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS)  -o inttest 

inttest3d: inttest3d.o $(OBJECTS) 
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	g++ inttest3d.o  $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS) -o inttest3d 

momenttest: momenttest.o $(OBJECTS) 
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	g++ momenttest.o $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS)  -o momenttest

momenttest3d: momenttest3d.o $(OBJECTS) 
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	g++ momenttest3d.o $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS)  -o momenttest3d 


fv_3d27_test: fv_3d27_test.o $(OBJECTS) 
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	g++ fv_3d27_test.o  $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS) -o fv_3d27_test

fv_3d27_stat_test: fv_3d27_stat_test.o $(OBJECTS) 
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	g++ fv_3d27_stat_test.o $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS)  -o fv_3d27_stat_test

nld_test: nld_test.o $(OBJECTS) 
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	g++ nld_test.o $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS)  -o nld_test

nld_run: nld_run.o $(OBJECTS) 
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	g++ nld_run.o $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS)  -o nld_run

listtest: listtest.o $(OBJECTS) 
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	g++ listtest.o $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS)  -o listtest

matrixtest: matrixtest.o $(OBJECTS) 
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	g++ matrixtest.o  $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS) -o matrixtest

anisotest: anisotest.o $(OBJECTS) 
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	g++ anisotest.o $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS)  -o anisotest

sorttest: sorttest.o $(OBJECTS) 
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	g++ sorttest.o $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS)  -o sorttest

winkeltest: winkeltest.o $(OBJECTS) 
	make -C ../datalib	
	make -C ../mathlib
	make -C ../cfdlib
	g++ winkeltest.o $(MODULES) $(OBJECTS) $(LDFLAGS) $(LIBS)  -o winkeltest

clean:	shellReconstruction.o



