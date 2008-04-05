include ../CFDProcessing.conf

INC = -I.
BIN = 
LIB = 
CPPFLAGS = $(GCPPFLAGS) $(INC)

LDFLAGS = $(GLDFLAGS) -L$(LIB)
LIBS= -lm

MODULES = 
OBJECTS = segmentation.o interpolation.o threshold.o regiongrowing.o point3d.o




NEWLIB = segmentationlib.a

all: $(NEWLIB)


$(NEWLIB): $(OBJECTS)
	ar -r $(NEWLIB) $(OBJECTS)  
	ranlib $(NEWLIB)


.cpp.o: 
	$(CPPC) $(CPPFLAGS) $< 



clean:
	rm -f $(OBJECTS)


