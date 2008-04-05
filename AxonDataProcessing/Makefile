include ../CFDProcessing.conf

INC = -I../toollib -I../toollib/bmpreader
BIN = 
LIB = 
CPPFLAGS = -O3 -c $(INC)
LDFLAGS =
LIBS=  -lm 

OBJECTS = axonprocessingcore.o command_line_parser.o AxonPathTracking.o
PROGNAME= $(APPL)
MODULES = ../toollib/bmpreader/bmpreader.a ../toollib/toollib.a  

all: AxonDataProcessing 



.c.o: 
	$(CC) $(CFLAGS) $< 

.cpp.o:
	$(CPPC) $(CPPFLAGS) $<


AxonDataProcessing: AxonDataProcessing.o  $(OBJECTS)
	$(LINKER) AxonDataProcessing.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS)  -o AxonDataProcessing
 

clean:
	rm AxonDataProcessing.o AxonDataProcessing $(OBJECTS) 






