include ../CFDProcessing.conf

INC =  
BIN = 
LIB = 
CFLAGS = $(GCFLAGS) -D$(ARCH)
CPPFLAGS = $(GCPPFLAGS) -D$(ARCH)
LDFLAGS = -L$(LIB)
LIBS= -m

OBJECTS = transform.o statistic.o integration.o moments.o integrant.o momentcalc.o sortedlistarray.o filteroptions.o fv_3d27.o nld.o memory.o 

MATHLIB = mathlib.a

all: $(MATHLIB)


$(MATHLIB): $(OBJECTS)
	ar -r $(MATHLIB) $(OBJECTS) 
	ranlib $(MATHLIB)



.c.o: 
	$(CC) $(CFLAGS) $< 

.cpp.o:
	$(CPPC) $(CPPFLAGS) $<


clean:
	rm -f $(OBJECTS) mathlib.a


