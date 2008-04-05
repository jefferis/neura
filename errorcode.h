/****************************************************************************/
/*                                                                          */
/* File:      errorcode.h 	                                            */
/*                                                                          */
/* Purpose:   encodes all Error and Macro definition                        */
/*            err(ErrNr.) outputstream for error decoding                   */
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
#ifndef ERRORCODE
#define ERRORCODE
#include <stream.h>
#include <iostream>
#include <iomanip>

#ifndef FILENAMELENGTH
#define FILENAMELENGTH 100
#endif

/* Error messages */
#define  OK                   0

/*cfdReader.h*/
#define CANNOTREADFILE     2001
#define NOCFDFILE          2002
#define NOMEMORY           2003

/*cfddatacube.h*/
#define CANTREADDATAFILE   3001
#define CFDFILESDONTFIT    3002
#define WRONGPARAMETERVALUE 3003

/*datacube.h*/
#define FLAG_INITIALIZED     1001
#define FLAG_MEMORYALLOCATED 1002
#define CANTMALLOC           1003
#define CANTREADNIHFILE	     1004
#define NONIHFILE            1005
#define DATAMISSING          1006
#define CANTWRITERAW         1007
#define CANTREADRAW          1008
#define CANTREADTIFF         1009
#define CANTWRITETIFF        1010


/*transform.h*/
#define WRONGDIMENSION       5001


/*integrate.h*/
#define UNDEFINED_GEOMETRIE  4001
#define GEOM_OUT_OF_RANGE    4002
#define UNIMPLEMENTED_VOL_DIM 4003
#define DCDIM_NEQ_VOLDIM     4004

/*moments.h*/
#define WRONG_RESVECTOR_LENGTH 6001
#define CUBE_WITHOUT_MIDPOINT 6002
#define CUBE_WITHOUT_MIDELEM 6003
#define SIZE_NOT_POW_OF_TWO 6004

/* stencil classes */
#define OUT_OF_STENCIL_RANGE 7001

/* fv  _3d27.h*/
#define UNKNOWN_KIND_OF_MOMENTS 8001
#define UNKNOWN_GEOMETRY_TYPE 8002
#define INTEGRATION_DOMAIN_TOO_BIG 8003

/* DataCubeXMLReader.h */
#define XML_DCBREADER_IOERROR 9001
#define XML_DCBREADER_FORMATERROR 9002


/* Reconstruction */
#define OPPERATION_NOT_PERMITTED 10001
#define COULDNOTFINDTERMINATION  10002
#define THISCOMPARTMENTISALLREADYATERMINATION 10003
#define THISCOMPARTMENTISNOTCONNECTED 10004
#define CANTWRITEHOCFILE 10005


/*Multicompartmentlist.h*/
#define CANTFINDCELLBODY 11001
#define POSITIONOFCELLBODYNOTVALID 11002




/* nld.h */
#define SIZE_NOT_POW_OF_TWO_PLUS_ONE 12001

/* NRA tiffReader.h */
#define NO_SPACEINGS_TAGS 13001
#define UNABLE_TO_OPEN_TIFF 13002
#define TIFFSTACK_INCONSISTENT 13003
#define TIFFTYPE_NOT_SUPPORTED 13004





static ostream& errorcodeCallback(ostream &os, int nr)
{
    switch (nr)
    {
        case OK: os << "OK" ;break;

        case CANNOTREADFILE     : os << "cfdReader:CANNOTREADFILE";break;
        case NOCFDFILE          : os << "cfdReader:CANNOTREADFILE";break;
        case NOMEMORY           : os << "cfdReader:NOMEMORY";break;

        case CANTREADDATAFILE   : os << "cfddatacube:CANTREADDATAFILE";break;
        case CFDFILESDONTFIT    : os << "cfddatacube:CFDFILESDONTFIT";break;
        case WRONGPARAMETERVALUE: os << "cfddatacube:WRONGPARAMETERVALUE";break;


        case CANTMALLOC         : os << "datacube:CANTMALLOC";break;
        case CANTREADNIHFILE	: os << "datacube:CANTREADNIHFILE";break;
        case NONIHFILE          : os << "datacube:NONIHFILE";break;
        case DATAMISSING        : os << "datacube:DATAMISSING";break;
        case CANTWRITERAW       : os << "datacube:CANTWRITERAW";break;
        case CANTREADRAW        : os << "datacube:CANTREADRAW";break;

        case UNDEFINED_GEOMETRIE: os << "integration: Invalid flag given for integration domain!\n";break;
        case GEOM_OUT_OF_RANGE  : os << "integration: Specified integration domain is not completely within the datarange!\n";break;

        case  UNIMPLEMENTED_VOL_DIM : os << "integration: specified volume not integrable -> Wrong dimension!\n"; break;

        case DCDIM_NEQ_VOLDIM : os << "integration: tried to integrate datacube on an other dimensional volume!\n"; break;


        case UNKNOWN_KIND_OF_MOMENTS : os << "fv_3d27: unknown kind of Moments specified\n"; break;

        case UNKNOWN_GEOMETRY_TYPE : os << "fv_3d27: unknown kind of GeometryType specified\n"; break;

        case INTEGRATION_DOMAIN_TOO_BIG : os << "fv_3d27: integration-size too big!\n";break;

        case WRONGDIMENSION     : os << "transform:WRONGDIMENSION";break;


        case WRONG_RESVECTOR_LENGTH : os << "moments: specified vector can't store result -> wrong length!\n"; break;

        case CUBE_WITHOUT_MIDPOINT : os << "moments: specified integration size must be odd in all directions!\n"; break;

        case CUBE_WITHOUT_MIDELEM : os << "moments: specified integration size must be even in all directions!\n"; break;

        case SIZE_NOT_POW_OF_TWO : os << "moments: datacube size must be a power of 2 for to use fourier-transformation!\n";break;

        case OUT_OF_STENCIL_RANGE : os << "stencil: acess out of stencil range!\n"; break;

        case XML_DCBREADER_IOERROR : os << "DataCubeXMLReader: unable to open file!\n"; break;

        case XML_DCBREADER_FORMATERROR : os << "DataCubeXMLReader: xml format corrupted!\n"; break;

            
        case CANTFINDCELLBODY : os << "Reconstruction: CANT FIND CELLBODY" ; break;
        case POSITIONOFCELLBODYNOTVALID : os << "Reconstruction: POSITION OF CELLBODY NOT VALID" ; break;
        case COULDNOTFINDTERMINATION : os << "Reconstruction: COULDNOTFINDTERMINATION"; break;
        case THISCOMPARTMENTISALLREADYATERMINATION : os << "Reconstruction: THISCOMPARTMENTISALLREADYATERMINATION";break;
        case THISCOMPARTMENTISNOTCONNECTED : os << "Reconstruction: THISCOMPARTMENTISNOTCONNECTED";break;
        case SIZE_NOT_POW_OF_TWO_PLUS_ONE : os << "NLD: datacube size must be a power of two plus one for to use multigrid solver!/n";break;		
		
		case NO_SPACEINGS_TAGS : os << "nraDataCubeTIFFReader: TIFF - stack does not contain grid spaceing tags!\n" ; break;
		case UNABLE_TO_OPEN_TIFF : os << "nraDataCubeTIFFReader: Unable to open TIFF - file!\n" ; break;
		case TIFFSTACK_INCONSISTENT : os << "nraDataCubeTIFFReader: Slices in TIFF - stack are of different sizes!\n" ; break;
		case TIFFTYPE_NOT_SUPPORTED : os << "nraDataCubeTIFFReader: Type of TIFF - image not supported!\n" ; break;

        default                 : os << "Error Nr." << nr;break;


    }

    return os;
};

class Omanip_int {
    ostream& (*f) (ostream&, int);
    int i;
public:
    Omanip_int(ostream& (*ff) (ostream&,int), int ii) : f(ff), i(ii) {};
    friend ostream& operator<<(ostream& os, Omanip_int m)  {return m.f(os,m.i);}
};


static const Omanip_int errorCode(int i)
{
    return Omanip_int(&errorcodeCallback,i);
};

#endif
