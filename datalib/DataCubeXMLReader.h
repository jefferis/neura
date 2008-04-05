/*! \file DataCubeXMLReader.h
    \brief XML datacube data readin and writeout.

    Author: Alexander Heusel
    Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen
    Universitaet Heidelberg
    Im Neuenheimer Feld 368
    D-69120 Heidelberg

    History: 27.02.2002

*/

#ifndef DATACUBEXMLREADER
#define DATACUBEXMLREADER

#include "DataCubeReaderHelper.h"

#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>


#define XML_DCBREADER_SUCCESS		0
#define XML_DCBREADER_IOERROR		9001
#define XML_DCBREADER_FORMATERROR	9002


/*! \struct DimDescription
    \brief Describes a dimension and resides in an instance of 
           CDataCubeHeader.
    
    This struct should not be instanced by the user of the class
    CDataCubeHeader. The DimDescription structs are generated
    automaticly when SetNumOfDimensions() is called.
*/
typedef struct
{
    double GridSpaceing;
    int Size;
    char* Label;
} DimDescription;


/*! \class CDataCubeHeader
    \brief This class represents the header of a XML datacube file.

    The destructor of CDataCube destroys all existent
    Lable members in all Dimensions.

*/
class CDataCubeHeader
{
public:
    /*! Constructor */
    CDataCubeHeader();
    /*! Constructor
        /param lNumOfDims Number of dimensions of the array. */    
    CDataCubeHeader(const long& lNumOfDims);
    /*! Destructor */
    ~CDataCubeHeader();
    
    /*! Check for validity of the object. */
    bool IsValid();
    
    
    /*! Access to the DimDescription structures.
        \param lIndex Index of the Dimension zerobased */
    DimDescription& Dimension(const long& lIndex);
    /*! Set number of dimensions
        
        DimDescription structures are automaticly created. */
    void SetNumOfDimensions(const long& lNumOfDims) ;
    /*! Returns number of dimansions */
    long GetNumOfDimensions();
    
    /*! Access to precision */
    long Precision;
    
private:
    void Init();
    DimDescription* pDimensions;
    long lNumOfDimensions;
    bool boolValid;
};

/*! \fn ReadXMLDataCube( const char* strFileName, CDataCubeHeader& Header, double*& pData )
    \brief Reads a XML datacube file.
    \param strFileName Filename of the file to read.
    \param Header Headerobject that will describe the geometry of the data.
    \param pData On success this pointer will point to the filedata.
    
    This function parses the XML datacube file and returns
    a double array.
    IMPORTANT NOTE:
    This parser does NOT correspond to the xml specs, i.e.:
    - It expects that the number of visible and invisible 
      characters surrounded by "" in a PAGE is equal to
      PAGESIZE.
    - XML comments are not allowed.
    - All values in a page have to be separated by a comma
      even if the page is formatted with newlines.
      The XML format was choosen to ease the import by other
    applications.
        
*/
int ReadXMLDataCube( const char* strFileName, CDataCubeHeader& Header, double*& pData );

/*! \fn WriteXMLDataCube( const char* strFileName, CDataCubeHeader& Header , double* pData )
    \brief Writes a XML datacube file.
    \param strFileName Filename of the file to write.
    \param Header Headerobject that describes the geometry of the data.
    \param pData Pointer to the data.
    
    This function writes a double array to a XML file.
*/
int WriteXMLDataCube( const char* strFileName, CDataCubeHeader& Header , double* pData );


#endif


