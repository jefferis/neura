include CFDProcessing.conf

setenv MAKEFLAGS= -j

all: 
	echo $(MAKEFLAGS)
	$(MAKE) _CFDLIB
	$(MAKE) _DATALIB
	$(MAKE) _MATHLIB
	$(MAKE) _RECONSTRUCTIONLIB 
	$(MAKE) _SEGMENTATIONLIB
	$(MAKE) _COMMANDLINETOOLS
#	$(MAKE) _neuraapp
#	$(MAKE) _DOC


_DATALIB:
	$(MAKE) -C datalib

_MATHLIB:
	$(MAKE) -C mathlib

_RECONSTRUCTIONLIB:
	$(MAKE) -C reconstructionlib


_SEGMENTATIONLIB:
	$(MAKE) -C segmentationlib

_COMMANDLINETOOLS:
	$(MAKE) -C commandlinetools 


_DXSUPPORT:
	@echo DXSupport: $(CONTROLLDXSUPPORT) 
ifeq ($(CONTROLLDXSUPPORT),YES)
	$(MAKE) -C DXsupport
endif


_CFDLIB:
	$(MAKE) -C cfdlib


_neuraapp: 
	$(MAKE) -C neuraapp 




###for documentation http://www.stack.nl/~dimitri/doxygen/
_DOC:	
ifeq ($(CONTROLLDOC),YES)
	doxygen Doxyfile
endif


clean:
	$(MAKE) clean -C cfdlib
	$(MAKE) clean -C mathlib
	$(MAKE) clean -C datalib
	$(MAKE) clean -C reconstructionlib
	$(MAKE) clean -C segmentationlib
	$(MAKE) clean -C commandlinetools
	$(MAKE) clean -C neuraapp

