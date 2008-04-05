include ../CFDProcessing.conf

INC = -I../mathlib -I../datalib
BIN = 
LIB = 
CPPFLAGS = $(GCPPFLAGS) $(INC) -D$(ARCH) #-mabi=32
CFLAGS = $(GCFLAGS)
LDFLAGS =$(GLDFLAGS)  $(LIB)
LIBS=  -lm -ltiff -lfl 

OBJECTS = cellloader.o hocparser.o
PROGNAME= $(APPL)
MODULES = ../reconstructionlib/reconstruction.a ../segmentationlib/segmentationlib.a ../mathlib/mathlib.a ../datalib/datalib.a ../cfdlib/cfdlib.a  

all: full3dproj

.c.o: 
	$(CC) $(CFLAGS) $< 

.cpp.o:
	$(CPPC) $(CPPFLAGS) $<



lex.yy.c: flexFile
	flex flexFile		


full3dproj:  main.o  lex.yy.o $(OBJECTS)
	$(LINKER) main.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS) -o  full3dproj



clean:
	rm $(OBJECTS) lex.yy.o full3dproj



