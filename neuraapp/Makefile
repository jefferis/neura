include ../CFDProcessing.conf

APPL= neura
STATICAPPL= neuraStatic
INC = -I../mathlib -I../datalib -I../reconstructionlib
BIN = 
LIB = 
CPPFLAGS = $(GCPPFLAGS) $(QTCPP) $(QTINC) $(INC) 

LDFLAGS = $(QTLDFLAGS) $(GLDFLAGS)  

LIBS= $(QTLIBS) $(LIB) 

OBJECTS = main.o \
	nraMainWindow.o \
	nraApplication.o \
	nraApplicationModel.o \
	nraCubeView.o \
	nraCubeMark.o \
	CReconstructionItem.o \
	UIMessage.o \
	nraDlgTIFFAlert.o \
	nraDlgPreferences.o \
	nraDlgMarkLabel.o
OBJMOC = moc_nraMainWindow.o \
	moc_nraApplication.o \
	moc_nraApplicationModel.o \
	moc_nraCubeView.o \
	moc_nraDlgTIFFAlert.o \
	moc_nraDlgPreferences.o \
	moc_nraDlgMarkLabel.o

PROGNAME= $(APPL)
STATICPROGNAME= $(STATICAPPL)
MODULES = ../reconstructionlib/reconstruction.a ../segmentationlib/segmentationlib.a ../mathlib/mathlib.a ../datalib/datalib.a ../cfdlib/cfdlib.a  

all: $(PROGNAME) 

.c.o: 
	$(CC) $(CFLAGS) $< 

.cpp.o:
	$(CPPC) $(CPPFLAGS) $<


$(PROGNAME): $(MODULES) $(OBJMOC) $(OBJECTS) 
	$(MAKE) -C ../datalib	
	$(MAKE) -C ../mathlib
	$(MAKE) -C ../cfdlib
	$(MAKE) -C ../reconstructionlib
	$(LINKER) $(OBJECTS) $(OBJMOC) $(MODULES)  $(LIBS) $(LDFLAGS) -o $(PROGNAME) 
	test -d neura.app/Contents/MacOS/ || mkdir -p neura.app/Contents/MacOS/
	$(LINKER) $(OBJECTS) $(OBJMOC) $(MODULES)  $(LIBS) $(LDFLAGS) -o $(PROGNAME).app/Contents/MacOS/$(PROGNAME) 
	touch main.cpp


$(STATICPROGNAME): $(OBJECTS) $(OBJMOC)
	$(MAKE) -C ../datalib	
	$(MAKE) -C ../mathlib
	$(MAKE) -C ../cfdlib
	$(MAKE) -C ../reconstructionlib
	test -d neura.app/Contents/MacOS/ || mkdir -p neura.app/Contents/MacOS/
	$(LINKER) $(OBJECTS) $(OBJMOC) $(MODULES)  $(LIBS) $(QTLDFLAGSSTATIC) $(GLDFLAGS) -o $(PROGNAME).app/Contents/MacOS/$(PROGNAME)
 







moc_nraMainWindow.cpp: $(MOC) nraMainWindow.h
	$(MOC) nraMainWindow.h -o moc_nraMainWindow.cpp

moc_nraApplication.cpp: $(MOC) nraApplication.h
	$(MOC) nraApplication.h -o moc_nraApplication.cpp

moc_nraApplicationModel.cpp: $(MOC) nraApplicationModel.h
	$(MOC) nraApplicationModel.h -o moc_nraApplicationModel.cpp

moc_nraCubeView.cpp: $(MOC) nraCubeView.h
	$(MOC) nraCubeView.h -o moc_nraCubeView.cpp

moc_nraDlgTIFFAlert.cpp: $(MOC) nraDlgTIFFAlert.h
	$(MOC) nraDlgTIFFAlert.h -o moc_nraDlgTIFFAlert.cpp

moc_nraDlgPreferences.cpp: $(MOC) nraDlgPreferences.h
	$(MOC) nraDlgPreferences.h -o moc_nraDlgPreferences.cpp

moc_nraDlgMarkLabel.cpp: $(MOC) nraDlgMarkLabel.h
	$(MOC) nraDlgMarkLabel.h -o moc_nraDlgMarkLabel.cpp

clean:
	rm -r $(OBJECTS) $(OBJMOC) moc_nraMainWindow.cpp moc_nraApplication.cpp moc_nraApplicationModel.cpp moc_nraCubeView.cpp moc_nraDlgTIFFAlert.cpp moc_nraDlgPreferences.cpp  moc_nraDlgMarkLabel.cpp $(PROGNAME).app
	$(MAKE) clean -C ../reconstructionlib 


