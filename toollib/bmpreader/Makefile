include ../../CFDProcessing.conf

INC = -I. -I..
BIN = 
LIB = 
CPPFLAGS = -m64  -DUSE_UNIX  -O3 -c $(INC)
CFLAGS = -m64 -DUSE_UNIX  -c -O3

LDFLAGS =  -L$(LIB)
LIBS= -lm

MODULES = 
OBJECTS = bmp.o pixel.o image.o vector.o

NEWLIB = bmpreader.a

all: $(NEWLIB)

$(NEWLIB): $(OBJECTS)
	ar -r $(NEWLIB) $(OBJECTS)  
	ranlib $(NEWLIB)

.cpp.o: 
	$(CPPC) $(CPPFLAGS) $< 

.c.o: 
	$(CC) $(CFLAGS) $< 
 


clean:
	rm -f $(OBJECTS) $(NEWLIB)


