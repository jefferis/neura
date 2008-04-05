include ../CFDProcessing.conf


INC = -I.
BIN = 
LIB = 
CPPFLAGS = -m64  -O3 -c $(INC)
CFLAGS = -m64 -c -O3

LDFLAGS =  -L$(LIB)
LIBS= -lm

MODULES = 
OBJECTS = datatypes.o morphology.o tophat.o thining.o moments.o tiffhandling.o diffusion.o segmentation.o 





NEWLIB = toollib.a

all: $(NEWLIB)


$(NEWLIB): $(OBJECTS)
	ar -r $(NEWLIB) $(OBJECTS)  
	ranlib $(NEWLIB)


.cpp.o: 
	$(CPPC) $(CPPFLAGS) $< 

.c.o: 
	$(CC) $(CFLAGS) $< 


clean:
	rm -f $(OBJECTS)


