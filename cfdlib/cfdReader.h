/****************************************************************************/
/*                                                                          */
/* File:      cfdReader.h 	                                            */
/*                                                                          */
/* Purpose:   to read cdf Files                                             */
/*                                                                          */
/* Author:    Philip J. Broser                                              */
/*            Max-Planck-Institut fuer medizinische Forschung               */
/*            Jahnstrasse 29                                                */
/*	      D-69120 Heidelberg                                            */
/*                                                                          */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail: philip.broser@iwr.uni-heidelberg.de                               */
/*                                                                          */
/* History:   01.08.02 begin                                                */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/

#ifndef CFDREADER
#define CFDREADER
#include <stdio.h>
#include <stream.h>


#ifdef sgi
#define bigEndian 
#endif

#ifdef linux
#define littleEndian
#endif


#define INT int 
#define LONG long 
#define FLOAT float 
#define UNSIGNED unsigned
#define CHAR char
#define SHORT short
#define IGNOREFIRSTTHREE_X  1
#define dataType UNSIGNED CHAR

#define FILENAMELENGTH 100

/* Definition of Header, ACV and ASV according to V 3.8.0 */ 
typedef struct ConFocHeader {
UNSIGNED short version;			// program version
CHAR	name[14];		// data file name
CHAR	user[16];		// user name
CHAR	time[16];		// time of acquisition
CHAR	date[16];		// date of acquisition
UNSIGNED LONG	start;			// 64 time at start of grab
UNSIGNED LONG	stop;			// 68 time at end of grab
LONG	zpos;                   // 72 z position in 1000's of microns
LONG  hdrsize;
LONG	rsvd[44];		// 76 reserved
} CFH;



typedef struct Acv {
LONG buflen;			
LONG pixbuflen;
struct {				
  UNSIGNED	pol:1;			
  UNSIGNED	clksrc:1;		
  UNSIGNED	waveform:2;	
  UNSIGNED	trigsrc:1;		
  UNSIGNED	trigpol:1;		
  UNSIGNED	trigmod:2;	
  UNSIGNED	LineScan:1;	
  UNSIGNED	fTacqTimeBase:1;	
  UNSIGNED	rsvd1:6;
  UNSIGNED	rsvd2:16;
} flags;

LONG	darate;		
LONG	dirate;		/*  5 Pixel clock rate in mHz*/
LONG	anaoff;		/*  6 analog offset in microseconds */
struct {				/*  7 pixels x and y */
  UNSIGNED x:16;		/* was sxpix */
  UNSIGNED y:16;		/* was sypix */
} pixels;
struct {				/*  8 */
  UNSIGNED	x:16;		/* was pppx */
  UNSIGNED	y:16;		/* was pppy */
} ppp;
struct {				/*  9 */
  UNSIGNED	scan:16;
  UNSIGNED	yretrace:16;
} scan;
struct {				/* 10 */
  UNSIGNED	images:16;
  UNSIGNED	chans:16;
} num;
LONG	reserv[4];	/* reserved up to 14 max*/

struct {				/* 15 */
  UNSIGNED	x:16;		/* maximum x dimension, targetted was mxsize*/
  UNSIGNED	y:16;		/* maximum y dimension, targetted was mysize*/
} msize;
FLOAT	scandur;		/* 16 target scan duration (msec) */
FLOAT	retracedur;	/* 17 target retrace duration (msec) */
LONG	ZStepSize;	/* 18 Z axis increment in nm */
UNSIGNED LONG DwellTime;	/* 19 Time in ms to wait during Z/T series scan */
UNSIGNED LONG cflags;		/*	20 config flag copies */
struct {							/* 21 */
  UNSIGNED	num:16;		/* Number of images averaged/step */
  UNSIGNED	toss:16;		/* Number of images to toss before average */
} avg;
LONG AcqTimeInt;			/* 22 Time between acquisitions */
LONG	TacqCounter;		/* 23 number of sets of movies in file, valid Version 3.7.010 */
LONG	XStepSize;			/* 24 X axis increment in nm if Sutter stage used */
LONG	YStepSize;			/* 25 Y axis increment in nm if Sutter stage used */
LONG	rsvd[39];			/* 26 buffer to 64 longs (256) */
} ACV;
   



typedef struct Asv {
	struct {				/*  1 Ranges for scan */
		UNSIGNED	x:16;
		UNSIGNED	y:16;
	} range;
	struct {				/*  2 Offsets for scan */
		UNSIGNED	x:16;
		UNSIGNED	y:16;
	} offset;
	struct {				/*  3 Parking position and orientation */
		UNSIGNED	park:16;
		UNSIGNED	orient:16;
	} scan;
	struct {				/*  4 Flags */
		UNSIGNED INT	discrete:1;		/* Slow scan value flag */
		UNSIGNED INT	ScanAvgMode:1;
		UNSIGNED INT	rsvd1:14;
		UNSIGNED INT	rsvd2:16;
	} flags;
	FLOAT	zoom_fac;	/*  5 Zoom factor */
	LONG	reserv[9];	/*  6 reserved */

	struct {
		UNSIGNED	Gain:16;	/* 15 Analog module 0 gain, XPG*/
		UNSIGNED	Offset:16;	/* Analog module 0 offset, XPG */
	} Acm0;
	struct {
		UNSIGNED	Gain:16;	/* 16 Analog module 1 gain, XPG */
		UNSIGNED	Offset:16;	/* Analog module 1 offset, XPG */
	} Acm1;
	LONG	rsvd[48];   /*	17 */
} ASV;


#include "../errorcode.h"


class CFDFile
{
 public:
  CFDFile();
  ~CFDFile();

 INT readCFDFile(CHAR *filename,INT flags);//default chanel is one!
 INT readCFDFile(CHAR *filename,INT flags, int channel);
 
 INT closeCFDFile();
 INT NUMBER_OF_PIXELS_X() {return acv.pixels.x;};
 INT NUMBER_OF_PIXELS_Y() {return acv.pixels.y;};
 INT NUMBER_OF_PIXELS_Z() {return acv.num.images;};
 float ZOOM() {return asv.zoom_fac;};
 float STEPSIZE() {return acv.ZStepSize;};
 dataType* DATA_POINTER() {return data;};
 
  
 friend ostream &operator<< (ostream &ostr, CFDFile &cfd);  
  
 private:
 int ReadCFDData8bitOneChannel(void);
 int ReadCFDData8bitTwoChannel(int channel);
 int ReadCFDData16bitOneChannel();
 
    FILE        *file;
    char        filename[FILENAMELENGTH];
    CFH         Header;
    ACV         acv;
    ASV         asv;
    dataType    *data;
};


#endif



