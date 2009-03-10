include ../CFDProcessing.conf

INC = -I../mathlib -I../datalib
BIN = 
LIB = 
CPPFLAGS = $(GCPPFLAGS) $(INC) -D$(ARCH) #-mabi=32
LDFLAGS =$(GLDFLAGS)  $(LIB)
LIBS=  -lm 
 
OBJECTS = command_line_parser.o
PROGNAME= $(APPL)
MODULES = ../reconstructionlib/reconstruction.a ../segmentationlib/segmentationlib.a ../mathlib/mathlib.a ../datalib/datalib.a ../cfdlib/cfdlib.a  

all: simple cfdfilter anisofilter rawtonih nihtorawZXchanged segmentor reconstructor nihtoraw

.c.o: 
	$(CC) $(CFLAGS) $< 

.cpp.o:
	$(CPPC) $(CPPFLAGS) $<

imageseqanal: imageseqanal.o $(OBJECTS)
	$(LINKER) imageseqanal.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS) -o imageseqanal

simple: simple.o $(OBJECTS) 
	$(MAKE) -C ../datalib	
	$(MAKE) -C ../mathlib
	$(MAKE) -C ../cfdlib
	$(MAKE) -C ../reconstructionlib
	$(LINKER) simple.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS) -o simple

cfdfilter: cfdfilter.o $(OBJECTS) 
	$(MAKE) -C ../datalib	
	$(MAKE) -C ../mathlib
	$(MAKE) -C ../cfdlib
	$(MAKE) -C ../reconstructionlib
	$(LINKER) cfdfilter.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS) -o cfdfilter

anisofilter: anisofilter.o $(OBJECTS) 
	$(MAKE) -C ../datalib	
	$(MAKE) -C ../mathlib
	$(MAKE) -C ../cfdlib
	$(MAKE) -C ../reconstructionlib
	$(LINKER) anisofilter.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS) -o anisofilter


tifftodx: tifftodx.o $(OBJECTS) 
	$(MAKE) -C ../datalib	
	$(MAKE) -C ../mathlib
	$(MAKE) -C ../cfdlib
	$(MAKE) -C ../reconstructionlib
	$(LINKER) tifftodx.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS) -o tifftodx

preparedata: preparedata.o $(OBJECTS) 
	$(MAKE) -C ../datalib	
	$(MAKE) -C ../mathlib
	$(MAKE) -C ../cfdlib
	$(MAKE) -C ../reconstructionlib
	$(LINKER) preparedata.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS) -o preparedata




rawtonih: rawtonih.o $(OBJECTS)
	$(MAKE) -C ../datalib
	$(MAKE) -C ../mathlib
	$(MAKE) -C ../cfdlib
	$(MAKE) -C ../reconstructionlib
	$(LINKER) rawtonih.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS) $(MODULES) -o rawtonih

nihtoraw: nihtoraw.o $(OBJECTS)
	$(MAKE) -C ../datalib
	$(MAKE) -C ../mathlib
	$(MAKE) -C ../cfdlib
	$(MAKE) -C ../reconstructionlib
	$(LINKER) nihtoraw.o $(OBJECTS) $(MODULES) $(LDFLAGS) $(LIBS) -o nihtoraw

nihtorawZXchanged: nihtorawZXchanged.o $(OBJECTS)
	$(MAKE) -C ../datalib
	$(MAKE) -C ../mathlib
	$(MAKE) -C ../cfdlib
	$(MAKE) -C ../reconstructionlib
	$(LINKER) nihtorawZXchanged.o $(OBJECTS) $(MODULES) $(LDFLAGS) $(LIBS) -o nihtorawZXchanged

segmentor: segmentor.o $(OBJECTS)
	$(MAKE) -C ../datalib
	$(MAKE) -C ../mathlib
	$(MAKE) -C ../cfdlib
	$(MAKE) -C ../reconstructionlib
	$(LINKER) segmentor.o $(OBJECTS) $(MODULES) $(LDFLAGS) $(LIBS) -o segmentor


reconstructor: reconstructor.o $(OBJECTS)
	$(MAKE) -C ../datalib
	$(MAKE) -C ../mathlib
	$(MAKE) -C ../cfdlib
	$(MAKE) -C ../reconstructionlib
	$(LINKER) reconstructor.o $(OBJECTS) $(MODULES) $(LDFLAGS) $(LIBS) -o reconstructor



clean:
	rm $(OBJECTS) anisofilter.o cfdfilter.o nihtoraw.o nihtorawZXchanged.o rawtonih.o reconstructor.o segmentor.o simple.o 
