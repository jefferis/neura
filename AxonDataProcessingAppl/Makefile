include ../CFDProcessing.conf

INC = -I../toollib -I../toollib/bmpreader
BIN = 
LIB = 
CPPFLAGS = -O3 -c $(INC)
LDFLAGS =
LIBS=  -lm 

OBJECTS = axonprocessingcore.o command_line_parser.o
PROGNAME= $(APPL)
MODULES = ../toollib/bmpreader/bmpreader.a ../toollib/toollib.a  

all: rawtoenhanced enhancedtoseg segtoskel skeletonprocess 



.c.o: 
	$(CC) $(CFLAGS) $< 

.cpp.o:
	$(CPPC) $(CPPFLAGS) $<


rawtoenhanced: rawtoenhanced.o $(OBJECTS)
	$(LINKER) rawtoenhanced.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS)  -o rawtoenhanced
 
enhancedtoseg: enhancedtoseg.o $(OBJECTS)
	$(LINKER) enhancedtoseg.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS)  -o enhancedtoseg

segtoskel: segtoskel.o $(OBJECTS)
	$(LINKER) segtoskel.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS)  -o segtoskel

 
skeletonprocess: skeltoprocessed.o $(OBJECTS)
	$(LINKER) skeltoprocessed.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS)  -o skeltoprocessed


cygwin: rawtoenhanced enhancedtoseg segtoskel skeletonprocess
	cp rawtoenhanced.exe execute/rawtoenhanced
	cp enhancedtoseg.exe execute/enhancedtoseg
	cp segtoskel.exe execute/segtoskel
	cp skeltoprocessed.exe execute/skeltoprocessed

executeSingleFileM: rawtoenhanced enhancedtoseg segtoskel skeletonprocess
	cp rawtoenhanced executeSingleFile/rawtoenhanced
	cp enhancedtoseg executeSingleFile/enhancedtoseg
	cp segtoskel executeSingleFile/segtoskel
	cp skeltoprocessed executeSingleFile/skeltoprocessed



main: main.o $(OBJECTS)
	$(LINKER) main.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS)  -o main 


createskeleton: createskeleton.o $(OBJECTS)
	$(LINKER) createskeleton.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS)  -o createskeleton


segmentation: segmentation.o $(OBJECTS)
	$(LINKER) segmentation.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS)  -o segmentation

moment: moment.o $(OBJECTS)
	$(LINKER) moment.o $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS)  -o moment


clean:
	rm rawtoenhanced enhancedtoseg segtoskel skeltoprocessed $(OBJECTS) 






