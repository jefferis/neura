include ../CFDProcessing.conf

INC = -I../quocmesh/include -I.
BIN = 
LIB = 
CPPFLAGS = $(GCPPFLAGS) $(INC)

LDFLAGS = $(GLDFLAGS) -L$(LIB)
LIBS= -lm

MODULES = ../mathlib/mathlib.a
OBJECTS = reconstruction.o viewcube.o reconstructionoptions.o  multicompartmentlist.o compartment.o compartmentlist.o mark.o primitive.o skeletonization.o nervecell.o 




NEWLIB = reconstruction.a

all: $(NEWLIB)


$(NEWLIB): $(OBJECTS)
	ar -r $(NEWLIB) $(OBJECTS)  
	ranlib $(NEWLIB)


.cpp.o: 
	$(CPPC) $(CPPFLAGS) $< 



clean:
	rm -f $(OBJECTS)
