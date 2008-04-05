include ../CFDProcessing.conf

INC = 
BIN = 
LIB = $(QTBASE)/lib
CFLAGS =  $(GCFLAGS)  -I$(INC) 
CPPFLAGS = $(GCPPFLAGS) $(INC)
LDFLAGS = -L$(LIB)
LIBS= -lqt

OBJECTS = cfdReader.o

CFDLIB   = cfdlib.a

all: $(CFDLIB)


$(CFDLIB): $(OBJECTS)
	ar -r $(CFDLIB) $(OBJECTS)  
	ranlib $(CFDLIB)


.c.o: 
	$(CC) $(CFLAGS) $< 


.cpp.o: 
	$(CPPC) $(CPPFLAGS) $<


clean:
	rm -f $(OBJECTS)


