include ../CFDProcessing.conf

INC = -I.
BIN = 
LIB = 
CPPFLAGS =    $(GCPPFLAGS) $(INC)

LDFLAGS = $(GLDFLAGS) -L$(LIB)
LIBS= -lm

OBJECTS =   dxdatacube.o  datacube.o momentcube.o volume.o cfddatacube.o testcubes.o  stencil3d27.o stencil.o DataCubeXMLReader.o DataCubeReaderHelper.o ParserUtilities.o Parser.o nraDataCubeTIFFReader.o

NEWLIB = datalib.a

all: $(NEWLIB)

$(NEWLIB): $(OBJECTS)
	ar -r $(NEWLIB) $(OBJECTS)  
	ranlib $(NEWLIB)



.cpp.o: 
	$(CPPC) $(CPPFLAGS) $< 



clean:
	rm -f $(OBJECTS) 


