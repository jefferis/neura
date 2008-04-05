INC = 
BIN = 
LIB = 
CFLAGS= -c -O3
CPPFLAGS = -c -O3 $(INC)
LDFLAGS = 
LIBS=  -lm -ltiff 

OBJECTS = example.o
PROGNAME= $(APPL)
MODULES = ../toollib/toollib.a ../datalib/datalib.a

  

all: example

.c.o: example.cpp
	gcc $(CFLAGS) $< 

.cpp.o:
	g++ $(CPPFLAGS) $<

../toollib/moments.c:
	make -C../toollib

../toollib/diffusion.c:
	make -C../toollib

../toollib/segmentation.c:
	make -C../toollib



example:  $(OBJECTS)  example.cpp ../toollib/moments.c ../toollib/diffusion.c ../toollib/segmentation.c
	make -C../toollib
	g++ $(OBJECTS) $(MODULES)  $(LDFLAGS) $(LIBS) -o example
	
	



clean:
	rm $(OBJECTS) 




