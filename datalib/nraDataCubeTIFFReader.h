/*
 *  nraDataCubeTIFFReader.h
 *  nraCubeReaderDockyard
 *
 *  Created by Alexander Heusel on Tue Nov 04 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _NRADATACUBETIFFREADER_H_
#define _NRADATACUBETIFFREADER_H_

#include "../nraConfig.h"
#include "../nraAssert.h"
#include "../errorcode.h"

#include "tiffio.h"
#include <math.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>

typedef struct
{
    double GridSpaceing;
    int Size;
    std::string Label;
} nraCubeDescription; 



template <class T>
class nraDataCubeTIFFReader
{
public:
    nraDataCubeTIFFReader() 
    {
    	m_pData = 0L;
		for(int i = 0; i < 3; i++)
		{
	    	m_Description[i].Size = 0;
	    	m_Description[i].Label = "";
	    	m_Description[i].GridSpaceing = 0.0;
    	}
    };
    
    int WriteCube(const std::string& Filename)
	{
	    int iWidth = Dimension(0).Size;
	    int iHeight = Dimension(1).Size;
	    int iZed = Dimension(2).Size;
		
	    long lNumOfElem = (long)(iWidth)*(long)(iHeight)*(long)(iZed);
	    T* pData = GetData();
	    T DataMin = *pData;
	    T DataMax = *pData;
	    for(long i = 0; i < lNumOfElem; i++)
	    {
			if(DataMin > pData[i])
			{
		    	DataMin = pData[i];
			}
			if(DataMax < pData[i])
			{
		    	DataMax = pData[i];
			}
	    }
	    T DataSpan = DataMax - DataMin;
	    T DataElem = (T)(0);

	    long lContainerCounter = lNumOfElem;
	    
	    // Do buffer calculations 
	    // Calculate rows per strip (ca. 8kb each strip).
	    double dRowsPerStrip =  static_cast<double>(8192) / static_cast<double>(iWidth);
	    long lRowsPerStrip = static_cast<long>(ceil(dRowsPerStrip));
	    double dNumOfStrips = static_cast<double>(iHeight) / static_cast<double>(lRowsPerStrip);
	    long lNumOfStrips = static_cast<long>(floor(dNumOfStrips));
	    long lRemainingRows = 0;
	    if(lNumOfStrips == 0)
	    {
			lRowsPerStrip = iHeight;
			lNumOfStrips = 1;
			lRemainingRows = 0;
	    }
	    else
	    {
			lRemainingRows = iHeight % (lNumOfStrips * lRowsPerStrip);
	    }
	    long lBuffSize = lRowsPerStrip * iWidth;
	    long lRemainingBuffSize = lRemainingRows * iWidth;
	    unsigned char* pStripBuff = new unsigned char[lBuffSize];
	    unsigned char* pRemainBuff;
	    if(lRemainingRows > 0)
	    {	
	        pRemainBuff = new unsigned char[lRemainingBuffSize];
	    }
		
	    // Create LUT
	    unsigned short pGrey[256];
	    for(unsigned short i = 0; i < 256; i++)
	    {
			pGrey[i] = i*256;
	    }
		
	    // Open file
	    TIFF* tif = TIFFOpen(Filename.c_str(), "w");
	    if(!tif)	// Error while opening tiff file -> inform function caller
	    {
	    	delete [] pStripBuff;
	    	if(lRemainingRows > 0)
	    	{
	    		delete [] pRemainBuff;
	    	}
	    	return UNABLE_TO_OPEN_TIFF;
	    }
		
		// Get description string and calculate resolution.
		std::string ImageDescription = GetDescriptionString();
		std::string Software = "NeuRA";
		float fXResolution = 1000.0 / Dimension(0).GridSpaceing;
		float fYResolution = 1000.0 / Dimension(1).GridSpaceing;
		
	    // Write slices...
	    for(int i = 0; i < iZed; i++)
	    {
			TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, iWidth);
			TIFFSetField(tif, TIFFTAG_IMAGELENGTH, iHeight);
			TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
			TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL , 1);
			TIFFSetField(tif, TIFFTAG_COMPRESSION, 1);
			TIFFSetField(tif, TIFFTAG_PLANARCONFIG, 1);
			TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, 3);
			TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, lRowsPerStrip);
			TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, 3);
			TIFFSetField(tif, TIFFTAG_SOFTWARE, Software.c_str());
			TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, ImageDescription.c_str());
			TIFFSetField(tif, TIFFTAG_XRESOLUTION, fXResolution);	
			TIFFSetField(tif, TIFFTAG_YRESOLUTION, fYResolution);	
			TIFFSetField(tif, TIFFTAG_COLORMAP, pGrey, pGrey, pGrey);
	
			// Write slice data.
			for (long lStrip = 0; lStrip < lNumOfStrips; lStrip++)
			{
		    	// Copy image data into strip buffer.
		    	for(long i = 0; i < lBuffSize; i++)
		    	{
					DataElem = *pData++;
					DataElem = ((DataElem - DataMin) / DataSpan)*(T)(255);
					pStripBuff[i] = (unsigned char)(DataElem);
					NRA_ASSERT(lContainerCounter--, "Subscript out of range!");
		    	}
		    	// Write stripbuffer.
		    	TIFFWriteRawStrip(tif, lStrip, pStripBuff, lBuffSize);
			}
	
			// If there are remaining rows write them.
			if (lRemainingRows > 0)
			{
		    	// Copy image data into 'remaining rows' buffer.
		    	for(long i = 0; i < lRemainingBuffSize; i++)
		    	{
		    		DataElem = *pData++;
					DataElem = ((DataElem - DataMin) / DataSpan)*(T)(255);
					pRemainBuff[i] = (unsigned char)(DataElem);
					NRA_ASSERT(lContainerCounter--, "Subscript out of range!");
				}
				// Write 'remaining rows' stripbuffer.
				TIFFWriteRawStrip(tif, lNumOfStrips, pRemainBuff, lRemainingBuffSize);
			}
		
			// Next slice
			TIFFWriteDirectory(tif);
	    }
	    TIFFClose(tif);
		
	    if(lRemainingRows > 0)
	    {
			delete [] pRemainBuff;
	    };
	    
	    delete [] pStripBuff;
	    
	    return OK;
		
	};
 
	int ReadCube(const std::string& Filename)
	{
	    
	    long lStripNmbr;
	    long* plStripSize;
	    long lCurrStripSize;
	    long lCurrRowsPerStrip;
	    long lNumOfStrips;
	    long lIdxData;
		
	
	    int iWidth = 0;
	    int iHeight = 0;
	    int iZed = 0;
	    long lNumOfElem = 0;
	    int iConsistWidth = 0;
	    int iConsistHeight = 0;
	    int iStartDir = 0;
	    
	    // Open file
	    TIFF* tif = TIFFOpen(Filename.c_str(), "r");
	    if(!tif) // TIFFOpen failed inform function caller about this
	    {
	    	return UNABLE_TO_OPEN_TIFF;
	    }
	    
	 	// Check for correct image format
	 	unsigned short usPhotometric = 0;
	 	unsigned short usBitsPerSample = 0;
	 	TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &usPhotometric);
	 	TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &usBitsPerSample);
	 	if(usPhotometric == 2 || usBitsPerSample != 8) // Unsupported tiff format -> inform function caller
	 	{
	 		return TIFFTYPE_NOT_SUPPORTED;
	 	}
	 
	 	// Get cube extensions.
	    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &iWidth);
	    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &iHeight);
	    iStartDir = TIFFCurrentDirectory(tif);
	    do
	    {
			iZed++;
			TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &iConsistWidth);
			TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &iConsistHeight);
			if(!(iConsistWidth == iWidth && iConsistHeight == iHeight)) // TIFF files have different sizes in stack -> inform caller
			{
				return TIFFSTACK_INCONSISTENT;
			}
			
	    } while (TIFFReadDirectory(tif));
	    TIFFSetDirectory(tif, iStartDir);
	
	
		
		// Get cube gridspaceing
		bool boolGridspaceingsPresent = false;
		char* pSoftware = 0L;
		char* pImageDescription = 0L;
		std::string Software;
		std::string ImageDescription;
		TIFFGetField(tif, TIFFTAG_SOFTWARE, &pSoftware);
		if(!pSoftware)
		{
			Software = "";
 		}
 		else
 		{
 			Software = pSoftware;
 		}
		if(Software == "NeuRA")
		{
		
			boolGridspaceingsPresent = true;
			TIFFGetField(tif, TIFFTAG_IMAGEDESCRIPTION, &pImageDescription);
			if(!pImageDescription)
			{
				boolGridspaceingsPresent = false;
				Dimension(0).GridSpaceing = 1.0;
				Dimension(1).GridSpaceing = 1.0;
				Dimension(2).GridSpaceing = 1.0;		
 			}
			else
			{
				ImageDescription = pImageDescription;
				SetDescription(ImageDescription);
			}
		}
		else
		{
			boolGridspaceingsPresent = false;
			Dimension(0).GridSpaceing = 1.0;
			Dimension(1).GridSpaceing = 1.0;
			Dimension(2).GridSpaceing = 1.0;		
		}
		std::cout << "here I am3!!" << std::endl << std::flush;
				
				
	    //Create dim description and array
	    lNumOfElem = (long)(iWidth)*(long)(iHeight)*(long)(iZed);
	    
	    T* pData = 0L;
	    pData = new T[lNumOfElem];
	    if(!pData)
	    {
	    	return UNABLE_TO_OPEN_TIFF;
	    }
	    SetData(pData); 
	    Dimension(0).Size = iWidth;
	    Dimension(1).Size = iHeight;
	    Dimension(2).Size = iZed;
	    long lContainerCounter = lNumOfElem;

	    lCurrStripSize = 1;
	    unsigned char* pCurrStrip = new unsigned char[lCurrStripSize];
	    do
	    {		
			// Read slice.
			lIdxData = 0;
			TIFFGetField(tif,TIFFTAG_STRIPBYTECOUNTS, &plStripSize);
			lNumOfStrips = TIFFNumberOfStrips(tif);
			for (lStripNmbr = 0; lStripNmbr < lNumOfStrips; lStripNmbr++)
			{
			    if(lCurrStripSize != plStripSize[lStripNmbr])
			    {
					lCurrStripSize = plStripSize[lStripNmbr];	
					lCurrRowsPerStrip = lCurrStripSize / iWidth;
					delete [] pCurrStrip;
					pCurrStrip = new unsigned char[lCurrStripSize];
			    }
			    TIFFReadRawStrip(tif, lStripNmbr, pCurrStrip, lCurrStripSize);
						
			    // Copy stripdata into array.
			    for(long i = 0; i < lCurrStripSize; i++)
			    {
					*pData++ = (T)(pCurrStrip[i])/(T)(255);
					NRA_ASSERT(lContainerCounter--, "Subscript out of range in [nraDataCubeTIFFReader::ReadCube]!");
			    }
		
			    lIdxData += lCurrRowsPerStrip;
			}
	    } while (TIFFReadDirectory(tif));
		
	    TIFFClose(tif);
	    
	    if(boolGridspaceingsPresent)
	   	{
	    	return OK;
	    }
	    else
	    {
	    	return NO_SPACEINGS_TAGS;
	    }
	};

    T* GetData() const
    {
		return m_pData;
    };

    inline void SetData(T* pData)
    {
		NRA_REQUIRE(pData, "Nullpointer passed in [nraDataCubeReader::SetData]!");
		m_pData = pData;
    };
    
    inline nraCubeDescription& Dimension(int iIdx)
    {
		NRA_REQUIRE(iIdx < 3, "Subscript out of range in [nraDataCubeReader::GetDescription]");
		return m_Description[iIdx];
    };
    
    
private:
    T* m_pData;
    nraCubeDescription m_Description[3];
    
    std::vector<std::string> split(const std::string& strLine, const std::string&  strSeparator )
	{
    	std::vector<std::string> Result;
    	Result.clear();
    	
    	unsigned int begIdx = strLine.find_first_not_of( strSeparator );
    	unsigned int endIdx = strLine.find( strSeparator, begIdx );
 
	    while( endIdx != std::string::npos )
	    {
	        Result.push_back( strLine.substr( begIdx, endIdx - begIdx ) );
	        begIdx = endIdx + 1;
	        endIdx = strLine.find( strSeparator, begIdx );
	    }
	    Result.push_back( strLine.substr( begIdx ) );
	    return Result;
	}
    
    void SetDescription(std::string ImageDescription)
    {
    
    	const int iPosLabelX = 17;
    	const int iPosGridX = 19;
    	const int iPosLabelY = 22;
    	const int iPosGridY = 24;
    	const int iPosLabelZ = 27;
    	const int iPosGridZ = 29;
    	
    	std::vector<std::string> DescParts = split(ImageDescription, "\n");
    	std::string BuffEntry;
    	std::string Crop;
    	
    	unsigned int iBeginIdx = 0;
    	unsigned int iEndIdx = 0;
    	
    	//
    	BuffEntry = DescParts[iPosLabelX];
    	iBeginIdx = BuffEntry.find_first_of(">");
    	iEndIdx = BuffEntry.find_last_of("<");
    	Dimension(0).Label = BuffEntry.substr(iBeginIdx + 1, iEndIdx - iBeginIdx - 1);
    
    	BuffEntry = DescParts[iPosGridX];
    	iBeginIdx = BuffEntry.find_first_of(">");
    	iEndIdx = BuffEntry.find_last_of("<");
		Crop = BuffEntry.substr(iBeginIdx + 1, iEndIdx - iBeginIdx - 1);
		Dimension(0).GridSpaceing = atof(Crop.c_str());
    	
    	//
    	BuffEntry = DescParts[iPosLabelY];
    	iBeginIdx = BuffEntry.find_first_of(">");
    	iEndIdx = BuffEntry.find_last_of("<");
    	Dimension(1).Label = BuffEntry.substr(iBeginIdx + 1, iEndIdx - iBeginIdx - 1);
    
    	BuffEntry = DescParts[iPosGridY];
    	iBeginIdx = BuffEntry.find_first_of(">");
    	iEndIdx = BuffEntry.find_last_of("<");
		Crop = BuffEntry.substr(iBeginIdx + 1, iEndIdx - iBeginIdx - 1);
		Dimension(1).GridSpaceing = atof(Crop.c_str());
    	
    	//
    	BuffEntry = DescParts[iPosLabelZ];
    	iBeginIdx = BuffEntry.find_first_of(">");
    	iEndIdx = BuffEntry.find_last_of("<");
    	Dimension(2).Label = BuffEntry.substr(iBeginIdx + 1, iEndIdx - iBeginIdx - 1);
    
    	BuffEntry = DescParts[iPosGridZ];
    	iBeginIdx = BuffEntry.find_first_of(">");
    	iEndIdx = BuffEntry.find_last_of("<");
		Crop = BuffEntry.substr(iBeginIdx + 1, iEndIdx - iBeginIdx - 1);
		Dimension(2).GridSpaceing = atof(Crop.c_str());
    	
    }
    
    std::string GetDescriptionString()
    {
    	std::stringstream os;
    	
    	// Header
    	os << "<?xml version = \"1.0\" standalone = \"yes\"?>" << std::endl;
    	os << "<!DOCTYPE IMAGEDESCRIPTION[" << std::endl;
    	os << "<!ELEMENT DATACUBE (VERSION, DIMENSION+)>" << std::endl;
    	os << "<!ELEMENT VERSION (MAJOR, MINOR)>" << std::endl;
    	os << "<!ELEMENT MAJOR (#PCDATA)>" << std::endl;
	    os << "<!ELEMENT MINOR (#PCDATA)>" << std::endl;
	    os << "<!ELEMENT DIMENSION (LABEL,SIZE,GRIDSPACEING)>" << std::endl;
	    os << "<!ELEMENT LABEL (#PCDATA)>" << std::endl;
	    os << "<!ELEMENT SIZE (#PCDATA)>" << std::endl;
	    os << "<!ELEMENT GRIDSPACEING (#PCDATA)>" << std::endl;
	   	os << "]>" << std::endl;
	    // Body.
	    os << "<IMAGEDESCRIPTION>" << std::endl;
	    os << "   <VERSION>" << std::endl;
	    os << "       <MAJOR>0</MAJOR>" << std::endl;
	    os << "       <MINOR>1</MINOR>" << std::endl;
	    os << "   </VERSION>" << std::endl;
	    os << "   <DIMENSION> " << std::endl;
	    os << "       <LABEL>" << m_Description[0].Label << "</LABEL>" << std::endl;
	    os << "       <SIZE>" << m_Description[0].Size << "</SIZE>" << std::endl;
	    os << "       <GRIDSPACEING>" << m_Description[0].GridSpaceing << "</GRIDSPACEING>" << std::endl;
	    os << "   </DIMENSION>" << std::endl;
	    os << "   <DIMENSION> " << std::endl;
	    os << "       <LABEL>" << m_Description[1].Label << "</LABEL>" << std::endl;
	    os << "       <SIZE>" << m_Description[1].Size << "</SIZE>" << std::endl;
	    os << "       <GRIDSPACEING>" << m_Description[1].GridSpaceing << "</GRIDSPACEING>" << std::endl;
	    os << "   </DIMENSION>" << std::endl;
	    os << "   <DIMENSION> " << std::endl;
	    os << "       <LABEL>" << m_Description[2].Label << "</LABEL>" << std::endl;
	    os << "       <SIZE>" << m_Description[2].Size << "</SIZE>" << std::endl;
	    os << "       <GRIDSPACEING>" << m_Description[2].GridSpaceing << "</GRIDSPACEING>" << std::endl;
	    os << "   </DIMENSION>" << std::endl;
		os << "</IMAGEDESCRIPTION>" << std::endl;
    	
    	return os.str();
    	
    }


};

#endif

