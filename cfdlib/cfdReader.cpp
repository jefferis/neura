/****************************************************************************/
/*                                                                          */
/* File:      cfdReader.c 	                                            */
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
#include <stdio.h>
#include "cfdReader.h"

#ifdef sgi
#define bigEndian 
#endif

#ifdef linux
#define littleEndian
#endif


ostream &operator<<(ostream &ostr, CFDFile &cfd)
{
  ostr << "Filename:" << cfd.filename << endl;
  ostr <<  cfd.NUMBER_OF_PIXELS_X() << "x" <<  cfd.NUMBER_OF_PIXELS_Y() << "x" << cfd.NUMBER_OF_PIXELS_Z() << endl;
  return ostr;
}
  



static INT 
swap(UNSIGNED CHAR *data)
{
  unsigned char help;

  help=data[0];
  data[0]=data[3];
  data[3]=help;
  
  help=data[1];
  data[1]=data[2];
  data[2]=help;

  return 0;
}



union TransformFloat
{
  UNSIGNED CHAR in[4];
  FLOAT out;
} transformFloat;

static INT 
readFloat(FILE *DataPointer,FLOAT *readData)
{
  INT i;
  UNSIGNED CHAR data;
  for (i=0;i<4;i++)
    {
      if (fread(&data,sizeof(data),1,DataPointer)!=1) return 1; 
      transformFloat.in[i]=data;
      }
  
  #ifdef bigEndian 
  swap(transformFloat.in);
  #endif

  *readData=transformFloat.out;
  return 0;
}  

union TransformLoang
{
  UNSIGNED CHAR in[4];
  LONG out;
} transformLong;

static INT 
readLong(FILE *DataPointer,LONG *readData)
{
  INT i;
  UNSIGNED CHAR data;
  for (i=0;i<4;i++)
    {
      if (fread(&data,sizeof(data),1,DataPointer)!=1) return 1; 
      transformLong.in[i]=data;
      }

  #ifdef bigEndian 
  swap(transformLong.in);
  #endif
  
  *readData=transformLong.out;
  return 0;
}  

union TransformULoang
{
  UNSIGNED CHAR in[4];
  LONG out;
} transformULong;


static INT 
readULong(FILE *DataPointer,UNSIGNED LONG *readData)
{
  INT i;
  UNSIGNED CHAR data;
  for (i=0;i<4;i++)
    {
      if (fread(&data,sizeof(data),1,DataPointer)!=1) return 1; 
      transformULong.in[i]=data;
      }

  #ifdef bigEndian 
  swap(transformULong.in);
  #endif
  
  *readData=transformULong.out;
  return 0;
}  

union TransformUShort
{
  UNSIGNED CHAR in[2];
  UNSIGNED SHORT out;
} transformUShort;


static INT 
readUShort(FILE *DataPointer,UNSIGNED SHORT *readData)
{
  INT i;
  UNSIGNED CHAR data;
  UNSIGNED SHORT help;
  for (i=0;i<2;i++)
    {
      if (fread(&data,sizeof(data),1,DataPointer)!=1) return 1; 
      transformULong.in[i]=data;
      }

  #ifdef bigEndian 
  help=transformUShort.in[0];
  transformUShort.in[0]=transformUShort.in[1];
  transformUShort.in[1]=help;
  #endif
  
  *readData=transformUShort.out;
  return 0;
}  







static INT 
readUChar(FILE *DataPointer,UNSIGNED CHAR *readData)
{
  if (fread(readData,sizeof(UNSIGNED CHAR),1,DataPointer)!=1) return 1; 
  return 0;
}


   
#define readValue(V,P) if (fread(&V,sizeof(V),1,P)!=1) return 1;
#define readField(F,P) if (fread(F,sizeof(F[0]),sizeof(F)/sizeof(F[0]),P)!=sizeof(F)/sizeof(F[0])) return 1;
#define readLONG(L,P) if (readLong(P,&L)) return 1;
#define readULONG(L,P) if (readULong(P,&L)) return 1;
#define readFLOAT(F,P) if (readFloat(P,&F)) return 1;
#define readUCHAR(C,P) if (readUChar(P,&C)) return 1;  
#define readUSHORT(S,P) if (readUShort(P,&S)) return 1;


static INT 
ReadHeader(FILE *DataPointer,CFH *Header)
{
  readUSHORT((Header->version),DataPointer)
  readField(Header->name,DataPointer);
  readField(Header->user,DataPointer);
  readField(Header->time,DataPointer);
  readField(Header->date,DataPointer);
  readULONG(Header->start,DataPointer);
  readULONG(Header->stop,DataPointer);
  readLONG(Header->zpos,DataPointer);
  readLONG(Header->hdrsize,DataPointer);
  readField(Header->rsvd,DataPointer);
  return 0;
} 


static INT 
ReadACV(FILE *DataPointer, ACV* acv)
{
  //UNSIGNED bitfield;
  //UNSIGNED bitfield1;
  UNSIGNED LONG longDataField;
  //UNSIGNED LONG uLongDataField;
  readLONG(acv->buflen,DataPointer);
  readLONG(acv->pixbuflen,DataPointer);
  
  /*sturct flags*/
  readULONG(longDataField,DataPointer);
  acv->flags.pol=longDataField & 1; 
  acv->flags.clksrc=   (longDataField >>1) & 1;
  acv->flags.waveform= (longDataField >>1) & 3;
  acv->flags.trigsrc=  (longDataField >>2) & 1;
  acv->flags.trigpol=  (longDataField >>1) & 1;
  acv->flags.trigmod=  (longDataField >>1) & 3;
  acv->flags.LineScan= (longDataField >>2) & 1;
  acv->flags.fTacqTimeBase=  (longDataField >>1) & 1;;
  acv->flags.rsvd1=  (longDataField >>1) & (1+2+4+8+16+32);
  acv->flags.rsvd2= longDataField;
  /*end of structure flags*/
  
  readLONG(acv->darate,DataPointer);
  readLONG(acv->dirate,DataPointer);
  readLONG(acv->anaoff,DataPointer);
  

  readULONG(longDataField,DataPointer);
  /*struct pixels*/
  acv->pixels.y=(UNSIGNED INT) ((UNSIGNED LONG) longDataField & 65535);
  acv->pixels.x=(UNSIGNED INT) ((UNSIGNED LONG) (longDataField >> 16) & 65535); 
  /*end of pixels*/

  /*struct ppp*/
  readULONG(longDataField,DataPointer);
  acv->ppp.x=(UNSIGNED INT) ((UNSIGNED LONG) longDataField & 65535);
  acv->ppp.y=(UNSIGNED INT) ((UNSIGNED LONG) (longDataField >> 16) & 65535);
  /*end of ppp*/

  /*struct scan*/
  readULONG(longDataField,DataPointer);
  acv->scan.scan=(UNSIGNED INT) ((UNSIGNED LONG) longDataField & 65535);
  acv->scan.yretrace=(UNSIGNED INT) ((UNSIGNED LONG) (longDataField >> 16) & 65535);
  /*end of scan*/
  

  /*struct num*/
  readULONG(longDataField,DataPointer);
  acv->num.images=(UNSIGNED INT) ((UNSIGNED LONG) longDataField & 65535);
  acv->num.chans=(UNSIGNED INT) ((UNSIGNED LONG) (longDataField >> 16) & 65535);
  /*end of num*/

  readField(acv->reserv,DataPointer);

  /*struct msize*/
  readULONG(longDataField,DataPointer);
  acv->msize.x=(UNSIGNED INT) ((UNSIGNED LONG) longDataField & 65535);
  acv->msize.y=(UNSIGNED INT) ((UNSIGNED LONG) (longDataField >> 16) & 65535);
  /*end of msize*/

  readFLOAT(acv->scandur,DataPointer);
  readFLOAT(acv->retracedur,DataPointer);
  readLONG(acv->ZStepSize,DataPointer);
  readULONG(acv->DwellTime,DataPointer);
  readULONG(acv->cflags,DataPointer);
  
  
  /*struct avg*/
  readULONG(longDataField,DataPointer);
  acv->avg.num=(UNSIGNED INT) ((UNSIGNED LONG) longDataField & 65535);
  acv->avg.toss=(UNSIGNED INT) ((UNSIGNED LONG) (longDataField >> 16) & 65535);
  /*end of avg*/

  readLONG(acv->AcqTimeInt,DataPointer);
  readLONG(acv->TacqCounter,DataPointer);
  readLONG(acv->XStepSize,DataPointer);
  readLONG(acv->YStepSize,DataPointer);
  
  readField(acv->rsvd,DataPointer);

  return 0;
}


static INT 
ReadASV(FILE *DataPointer, ASV* asv)
{
  //UNSIGNED bitfield;
  //INT intval;
  UNSIGNED LONG longDataField;
  /*struct scan range*/
  //readValue(longDataField,DataPointer);
  readULONG(longDataField,DataPointer);
  asv->range.x=(UNSIGNED INT) ((UNSIGNED LONG) longDataField & 65535);
  asv->range.y=(UNSIGNED INT) ((UNSIGNED LONG) (longDataField >> 16) & 65535);
  /*end of scanrange*/

  /*struct offset*/
  //readValue(longDataField,DataPointer);
  readULONG(longDataField,DataPointer);
  asv->offset.x=(UNSIGNED INT) ((UNSIGNED LONG) longDataField & 65535);
  asv->offset.y=(UNSIGNED INT) ((UNSIGNED LONG) (longDataField >> 16) & 65535);
  /*end of offset*/

  /*struct scan*/
  //readValue(longDataField,DataPointer);
  readULONG(longDataField,DataPointer);
  asv->scan.park=(UNSIGNED INT) ((UNSIGNED LONG) longDataField & 65535);
  asv->scan.orient=(UNSIGNED INT) ((UNSIGNED LONG) (longDataField >> 16) & 65535);
  /*end of scan*/

  /*struct flags*/
  //readValue(longDataField,DataPointer);
  readULONG(longDataField,DataPointer);
  asv->flags.discrete=longDataField & 1;
  asv->flags.ScanAvgMode= (longDataField >> 1) & 1;
  asv->flags.rsvd1=(longDataField >> 1) & 16383;
  asv->flags.rsvd2=(longDataField >> 14);
  /*end struct flags*/

  //readValue(asv->zoom_fac,DataPointer);
  readFLOAT(asv->zoom_fac,DataPointer);
  readField(asv->reserv,DataPointer);

  /*struct Acm0*/
  //readValue(longDataField,DataPointer);
  readULONG(longDataField,DataPointer);
  asv->Acm0.Gain=(UNSIGNED INT) ((UNSIGNED LONG) longDataField & 65535);
  asv->Acm0.Offset=(UNSIGNED INT) ((UNSIGNED LONG) (longDataField >> 16) & 65535);
  /*end Acm0*/
  
  /*struct Acm1*/
  //readValue(longDataField,DataPointer);
  readULONG(longDataField,DataPointer);
  asv->Acm1.Gain=(UNSIGNED INT) ((UNSIGNED LONG) longDataField & 65535);
  asv->Acm1.Offset=(UNSIGNED INT) ((UNSIGNED LONG) (longDataField >> 16) & 65535);
  /*end Acm0*/
  
  readField(asv->rsvd,DataPointer);

  return 0;
}

static INT 
openFile(FILE **DataPointer,CHAR name[FILENAMELENGTH])
{
  *DataPointer=fopen(name,"r");
  if (*DataPointer==NULL)
    { 
      char dummy[80];
      cout << "Filename:" <<  name << endl;
      sprintf(dummy,"\nIO Message");
      perror(dummy);
      return 1;
    }
  return 0;
}

#define IGNOREFIRSTTHREE_X  1

#define dataType UNSIGNED CHAR
#define DX_Datatype TYPE_UBYTE


INT 
CFDFile::
ReadCFDData8bitOneChannel() 
{
  cout << "Read one channel 8bit image" << endl;
  dataType readData;
  INT counter;
  //dataType *Temp;
  LONG numerOfVoxel=NUMBER_OF_PIXELS_X()*NUMBER_OF_PIXELS_Y()*NUMBER_OF_PIXELS_Z();
  data=new(dataType[numerOfVoxel*sizeof(dataType)]);
  if (data==NULL) return NOMEMORY;
  if (fseek(file,Header.hdrsize,0)) return 1;
  for (counter=0;counter<numerOfVoxel;counter++)
    {
	readUCHAR(readData,file);
	data[counter]=readData;
    };
  return (ferror(file));
}

INT
CFDFile::
ReadCFDData8bitTwoChannel(int channel) // at the moment only two channels implemented
{
    int counter;
    cout << "Read two channel 8bit image, read channel:" << channel << endl;
    dataType readData;
    LONG numerOfVoxel=NUMBER_OF_PIXELS_X()*NUMBER_OF_PIXELS_Y()*NUMBER_OF_PIXELS_Z();
    data=new(dataType[numerOfVoxel*sizeof(dataType)]);
    if (data==NULL) return NOMEMORY;
    if (fseek(file,Header.hdrsize,0)) return 1;
    for (counter=0;counter<numerOfVoxel;counter++)
    {
        readUCHAR(readData,file);
        if (channel==1) data[counter]=readData;
        readUCHAR(readData,file);
        if (channel==2) data[counter]=readData;
    };
    return (ferror(file));
}

INT
CFDFile::
ReadCFDData16bitOneChannel() // at the moment only two channels implemented
{
	//not yet implemented
	cout << "16bit Images not yet implemented!";
	exit(1);
}









INT static closeFile(FILE *DataPointer)
{
  return fclose(DataPointer);
}



CFDFile::
CFDFile()
{};

CFDFile::
~CFDFile()
{};


INT 
CFDFile::
readCFDFile(char tFilename[FILENAMELENGTH],INT flags)
{
  sprintf(filename,tFilename);
  if (openFile(&file,filename) != 0) return CANNOTREADFILE;
  if (ReadHeader(file,&Header)) return NOCFDFILE;
  if (ReadACV(file,&(acv))) return NOCFDFILE;
  if (ReadASV(file,&(asv))) return NOCFDFILE;
  cout << "Num of chanells:" << acv.num.chans << endl;
  switch (acv.num.chans)
  {
  case 1: case 2:if (ReadCFDData8bitOneChannel()) return NOCFDFILE;break;
  case 3:if (ReadCFDData8bitTwoChannel(1)) return NOCFDFILE;break;
  case 8:if (ReadCFDData16bitOneChannel()) return NOCFDFILE;break;
  }
  
  return OK;
}

INT
CFDFile::
readCFDFile(char tFilename[FILENAMELENGTH],INT flags,int channel)
{
    sprintf(filename,tFilename);
    if (openFile(&file,filename) != 0) return CANNOTREADFILE;
    if (ReadHeader(file,&Header)) return NOCFDFILE;
    if (ReadACV(file,&(acv))) return NOCFDFILE;
    if (ReadASV(file,&(asv))) return NOCFDFILE;
    switch (acv.num.chans)
 	 {
  		case 1: case 2:if (ReadCFDData8bitOneChannel()) return NOCFDFILE;break;
  		case 3:if (ReadCFDData8bitTwoChannel(channel)) return NOCFDFILE;break;
  		case 8:if (ReadCFDData16bitOneChannel()) return NOCFDFILE;break;
  	}
    return OK;
}



INT 
CFDFile::
closeCFDFile()
{
    closeFile(file);
    delete [] data;
    return 0;
}



