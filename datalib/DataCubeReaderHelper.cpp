/*  
    DataCubeReaderHelper.cpp

    Author: Alexander Heusel
    Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen
    Universitaet Heidelberg
    Im Neuenheimer Feld 368
    D-69120 Heidelberg

    History: 27.02.2002

*/


#include "DataCubeReaderHelper.h"

#include <iomanip>
#include <sstream>


///////////////////////////////////////////////////////////
bool split( vector<string>& Result, const string& strLine, const string&  strSeparator )
///////////////////////////////////////////////////////////
{
    Result.clear();
    unsigned int begIdx = strLine.find_first_not_of( strSeparator );
    unsigned int endIdx = strLine.find( strSeparator, begIdx );
    
    while( endIdx != string::npos )
    {
        Result.push_back( strLine.substr( begIdx, endIdx - begIdx ) );
        begIdx = endIdx + 1;
        endIdx = strLine.find( strSeparator, begIdx );
    }
    Result.push_back( strLine.substr( begIdx ) );
    return true;
}

///////////////////////////////////////////////////////////
bool FindMatchingBracketsInFile(ifstream& oIn,
                                    string& strResult, 
                                    string& strPredecessor)
///////////////////////////////////////////////////////////
{
    string strBuff;
    stringstream oStream(ios:: in | ios::out);
    char chBuff;

    // Read predecessor.
    while ( oIn.get(chBuff) )
    {	
        strBuff = chBuff;
        if(strBuff != "<")
            oStream << strBuff;
        else
            break;
    }
    strPredecessor = oStream.str();
    oStream.str("");
    
    long lBracketCounter = 1;
    // Read contents of the bracket.
    while ( oIn.get(chBuff) )
    {
        strBuff = chBuff;
        if( strBuff == "<" )
            lBracketCounter++;
        if( strBuff == ">" )
            lBracketCounter--;
        if( lBracketCounter == 0 )
            break;
        oStream << strBuff;
    }
    strResult = oStream.str();
    
    
    return true;
} 

///////////////////////////////////////////////////////////
bool FindMatchingBracketsInString(const string& strIn, 
                                    string& strResult, 
                                    string& strRemainder, 
                                    string& strPredecessor)
///////////////////////////////////////////////////////////
{   
    
    string strBuff;
    stringstream oIn(ios::in | ios::out);
    stringstream oOut(ios::in | ios::out);
    char chBuff;
        
    oIn.str(strIn);

    // Read predecessor.
    while ( oIn.get(chBuff) )
    {	
        strBuff = chBuff;
        if(strBuff != "<")
            oOut << strBuff;
        else
            break;
    }
    strPredecessor = oOut.str();
    oOut.str("");
    
    long lBracketCounter = 1;
    // Read contents of the bracket.
    while ( oIn.get(chBuff) )
    {
        strBuff = chBuff;
        if( strBuff == "<" )
            lBracketCounter++;
        if( strBuff == ">" )
            lBracketCounter--;
        if( lBracketCounter == 0 )
            break;
        oOut << strBuff;
    }
    strResult = oOut.str();
    oOut.str("");
    
    // Read the remainder
    while ( oIn.get(chBuff) )
    {
        strBuff = chBuff;
        oOut << strBuff;
    }
    strRemainder = oOut.str();

    return true;
}

///////////////////////////////////////////////////////////
bool FindTagInFile(ifstream& oIn, 
                    const string& strTag, 
                    string& strPredecessor)
///////////////////////////////////////////////////////////
{
    string strResult;
    bool boolRes = FindMatchingBracketsInFile(oIn, strResult, strPredecessor);
    while ( strResult != strTag )
        boolRes = FindMatchingBracketsInFile(oIn, strResult, strPredecessor);
    return true;
}

