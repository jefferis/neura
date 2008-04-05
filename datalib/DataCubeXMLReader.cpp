/* 
    DataCubeXMLReader.cpp
    
    Author: Alexander Heusel
    Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen
    Universitaet Heidelberg
    Im Neuenheimer Feld 368
    D-69120 Heidelberg

    History: 27.02.2002

*/
  
#include "DataCubeXMLReader.h"



CDataCubeHeader::CDataCubeHeader()
{
    Init();
}

CDataCubeHeader::CDataCubeHeader(const long& lNumOfDims)
{
    Init();
    SetNumOfDimensions(lNumOfDims);
}

CDataCubeHeader::~CDataCubeHeader()
{
    if (boolValid)
        delete [] pDimensions;
}

void CDataCubeHeader::Init()
{
    lNumOfDimensions = 0;
    Precision = 5; 
    pDimensions = NULL;
    boolValid = false;
}

bool CDataCubeHeader::IsValid()
{
    return boolValid;
}

DimDescription& CDataCubeHeader::Dimension(const long& lIndex)
{
    return pDimensions[lIndex];
}

void CDataCubeHeader::SetNumOfDimensions(const long& lNumOfDims)
{
    if (boolValid)
    {
        for( long i = 0; i < lNumOfDimensions; i++)
            delete [] pDimensions[i].Label;
        delete [] pDimensions;
    }
    else
        boolValid = true;
    lNumOfDimensions = lNumOfDims;
    pDimensions = new DimDescription[lNumOfDimensions];
}

long CDataCubeHeader::GetNumOfDimensions()
{
    return lNumOfDimensions;
}

///////////////////////////////////////////////////////////
int ReadXMLDataCube( const char* strFileName, CDataCubeHeader& Header, double*& pData )
///////////////////////////////////////////////////////////
{
    
    pData = NULL; 				// Set pData to zero pointer.
        
    ifstream is( strFileName );			// Open file and check for success.
    if ( !is.is_open() ) return XML_DCBREADER_IOERROR;


    // Pickle file header and find out if we have the correct version.
    string strBuff;
    vector<string> vecstrBuff;
    string strResult;
    string strPredecessor; 
    string strRemainder;
    
    bool boolRes = FindMatchingBracketsInFile( is, strResult, strPredecessor );
    if ( strResult != "?xml version = \"1.0\" standalone = \"yes\"?" )
        return XML_DCBREADER_FORMATERROR;

    boolRes = FindMatchingBracketsInFile( is, strResult,  strPredecessor );
    strBuff = strResult;
    boolRes = FindMatchingBracketsInString( strBuff, strResult, strRemainder, strPredecessor);
    if( strPredecessor != "!DOCTYPE DATACUBE[\n" )
        return XML_DCBREADER_FORMATERROR;
        
    boolRes = FindMatchingBracketsInFile( is, strResult,  strPredecessor );
    
    // Check Version.
    // Major number.
    boolRes = FindTagInFile(is, "/MAJOR", strPredecessor);
    if ( strPredecessor != "0" )
        return XML_DCBREADER_FORMATERROR;
    // Minor number.
    boolRes = FindTagInFile(is, "/MINOR", strPredecessor);
    if ( strPredecessor != "2" )
        return XML_DCBREADER_FORMATERROR;
        
    boolRes = FindTagInFile(is, "/VERSION", strPredecessor);
    
    // Get DATA description.
    boolRes = FindMatchingBracketsInFile( is, strResult,  strPredecessor );
    boolRes = split( vecstrBuff, strResult, "\"" );
    long lNumOfDims = atoi(vecstrBuff[1].c_str());
    Header.SetNumOfDimensions(lNumOfDims); 
    Header.Precision = atoi(vecstrBuff[5].c_str());
    long lNumOfPages = atoi(vecstrBuff[3].c_str());
        
    // Get description of dimensions and determine total number of elements.
    long lNumOfElem = 1;

    for ( int i = 0; i < lNumOfDims; i++)
    {
        boolRes = FindTagInFile(is, "DIMENSION", strPredecessor);
        boolRes = FindTagInFile(is, "/LABEL", strPredecessor);
        Header.Dimension(i).Label = new char[strPredecessor.size() + 1];
        Header.Dimension(i).Label[strPredecessor.size()] = 0;
        strPredecessor.copy(Header.Dimension(i).Label, strPredecessor.size());
        boolRes = FindTagInFile(is, "/SIZE", strPredecessor);
        Header.Dimension(i).Size = atoi(strPredecessor.c_str());
        lNumOfElem *= Header.Dimension(i).Size;
        boolRes = FindTagInFile(is, "/GRIDSPACEING", strPredecessor);
        Header.Dimension(i).GridSpaceing = atof(strPredecessor.c_str());
    }
    boolRes = FindTagInFile(is, "/DIMENSION", strPredecessor);
    
    // Read pages.
    long lLengthBuff;
    char* chBuff;	// Pointer for character buffer.
    char chSingle;
    pData = new double[lNumOfElem];
    long lDataIdx = 0;
    long lElemPerPage = 0;
    if (Header.GetNumOfDimensions() > 1)
        lElemPerPage = Header.Dimension(0).Size * Header.Dimension(1).Size;
    else
        lElemPerPage = Header.Dimension(0).Size;
    for (long lPage = 0; lPage < lNumOfPages; lPage++)
    {
        // Search for <PAGE PAGESIZE = "XXX"> Tag, allocate enough space
        // for buffer read in and place instream pointer on the beginning
        // of the pagedata.
        boolRes = FindMatchingBracketsInFile( is, strResult,  strPredecessor );
        boolRes = split(vecstrBuff, strResult, "\"" );
        lLengthBuff = atoi(vecstrBuff[1].c_str()) + 1;
        chBuff = new char[lLengthBuff];
        while( is.get(chSingle) )
        {
            strBuff = chSingle;
            if (strBuff == "\"") break;
        }

        is.read(chBuff, lLengthBuff);
        chBuff[lLengthBuff - 1] = 0;
        strBuff = chBuff;
        boolRes = split( vecstrBuff, strBuff, "," );
        for (long lElem = 0; lElem < lElemPerPage; lElem++)
            pData[lDataIdx++] = atof(vecstrBuff[lElem].c_str());
        delete [] chBuff;
        boolRes = FindTagInFile(is, "/PAGE", strPredecessor);
    }
    
    return XML_DCBREADER_SUCCESS;
}

///////////////////////////////////////////////////////////
int WriteXMLDataCube( const char* strFileName, CDataCubeHeader& Header , double* pData )
///////////////////////////////////////////////////////////
{
    
    // Open file and check success.
    ofstream os( strFileName );
    if ( !os.is_open() ) return XML_DCBREADER_IOERROR;
    
    // Create stringstream and string for page assembly.
    stringstream streamPageBuff;
    string strPageBuff;

    // Calc document geometry.
    long lNumOfDims = Header.GetNumOfDimensions();		// Shortcut.
    long lPrecision = Header.Precision;				// dto.
    long lNumOfPages = 1;					// Calculate total number of pages.
    if ( lNumOfDims > 2 )
    {
        for (int i = 2 ; i < lNumOfDims ; i++)
            lNumOfPages *= Header.Dimension(i).Size;
    }
    long lLinesPerPage = 0;					// Calculate total number of lines per page.
    if ( lNumOfDims > 1 )
        lLinesPerPage = Header.Dimension(1).Size - 1;
    long lElemPerLine = Header.Dimension(0).Size - 1;		// Calculate number of elements per line.
                                                               

    // Write document.
    // Header.
    os << "<?xml version = \"1.0\" standalone = \"yes\"?>" << endl;
    os << "<!DOCTYPE DATACUBE[" << endl;
    os << "<!ELEMENT DATACUBE (VERSION,DATA+)>" << endl;
    os << "<!ELEMENT VERSION (MAJOR, MINOR)>" << endl;
    os << "<!ELEMENT MAJOR (#PCDATA)>" << endl;
    os << "<!ELEMENT MINOR (#PCDATA)>" << endl;
    os << "<!ELEMENT DATA (DIMENSION+,PAGE+)>" << endl;
    os << "<!ELEMENT DIMENSION (LABEL,SIZE,GRIDSPACEING)>" << endl;
    os << "<!ELEMENT LABEL (#PCDATA)>" << endl;
    os << "<!ELEMENT SIZE (#PCDATA)>" << endl;
    os << "<!ELEMENT GRIDSPACEING (#PCDATA)>" << endl;
    os << "<!ELEMENT PAGE (#PCDATA)>" << endl;
    os << "<!ATTLIST DATA" << endl;
    os << "    DIMENSIONS CDATA #REQUIRED" << endl;
    os << "    PAGES      CDATA #REQUIRED" << endl;
    os << "    PRECISION  CDATA #REQUIRED>" << endl;
    os << "<!ATTLIST PAGE" << endl;
    os << "    PAGESIZE   CDATA #REQUIRED>" << endl;
    os << "]>" << endl;
    // Body.
    os << "<DATACUBE>" << endl;
    os << "   <VERSION>" << endl;
    os << "       <MAJOR>0</MAJOR>" << endl;
    os << "       <MINOR>2</MINOR>" << endl;
    os << "   </VERSION>" << endl;
    os << "   <DATA ";
    os <<         "DIMENSIONS = \"" << lNumOfDims  << "\" ";
    os <<         "PAGES = \""      << lNumOfPages << "\" ";
    os <<         "PRECISION = \""  << lPrecision  << "\">" << endl;
    // Write data.
    // Dimensions
    for ( int i = 0; i < lNumOfDims;  i++ )
        {
    os << "       <DIMENSION>" << endl;
    os << "           <LABEL>"<< Header.Dimension(i).Label << "</LABEL>" << endl;
    os << "           <SIZE>" << Header.Dimension(i).Size << "</SIZE>" << endl;
    os << "           <GRIDSPACEING>"<< Header.Dimension(i).GridSpaceing << "</GRIDSPACEING>" << endl;
    os << "       </DIMENSION>" << endl;
        }
    // Pages.
    streamPageBuff.str("");
    streamPageBuff << setiosflags( ios::scientific ) << setprecision(lPrecision);
    long lIdx = 0;
    for ( long lPage = 0; lPage < lNumOfPages; lPage++ )
    {
        for ( long lLine = 0; lLine < lLinesPerPage; lLine++ )
        {
            for ( long lElem = 0; lElem < lElemPerLine; lElem++ )
            {
                streamPageBuff << pData[lIdx++] << ", ";
            }
            streamPageBuff << pData[lIdx++] << "," << endl;
        }
        for ( long lElem = 0; lElem < lElemPerLine; lElem++ )
        {
            streamPageBuff << pData[lIdx++] << ", ";
        }
        streamPageBuff << pData[lIdx++] << endl;
        strPageBuff = streamPageBuff.str();
        streamPageBuff.str("");
    os << "       <PAGE PAGESIZE = \""<< strPageBuff.size() << "\">" << endl;
    os << "\"" << strPageBuff << "\"" << endl;
    os << "       </PAGE>" << endl;
    }
    // Write document ending.
    os << "   </DATA>" << endl;
    os << "</DATACUBE>" << endl;

    return XML_DCBREADER_SUCCESS;
}

