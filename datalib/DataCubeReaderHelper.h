/*! \file DataCubeReaderHelper.h
    \brief Helper functions for xml readin and writeout of
           datacube data. 

    Author: Alexander Heusel
    Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen
    Universitaet Heidelberg
    Im Neuenheimer Feld 368
    D-69120 Heidelberg

    History: 27.02.2002

*/
#ifndef DATACUBEREADERHELPER
#define DATACUBEREADERHELPER


#include <vector>
#include <string>
#include <fstream>

using namespace std;

/*! \fn bool split( vector<string>& Result, const string& strLine, const string&  strSeparator )
    \brief splits a string into pieces with regard to a given separator.
    \param Result contains the substrings of the line. All previous data will be overwritten.
    \param strLine Contains the line to be split.
    \param strSeparator Contains the separator string.
    
*/
bool split( vector<string>& Result, const string& strLine, const string&  strSeparator );

/*! \fn bool FindMatchingBracketsInFile(const ifstream& oIn, string& strResult, string& strPredecessor)
    \param oIn Filestream to search.
    \param strResult String that will contain the result.
    \param strPredecessor Will contain the predecessor of the matched bracket.
    
    This function finds the first match of brackets.
    E.g.: "--<<<><>>>" returns 
    strResult: "<<><>>"
    strPredecessor: "--"
    
*/
bool FindMatchingBracketsInFile(ifstream& oIn,
                            string& strResult, 
                            string& strPredecessor);

/*! \fn bool FindMatchingBracketsInString(const string& strIn, string& strResult,  string& strRemainder, string& strPredecessor)
    \param strIn String to search.
    \param strResult String that will contain the result.
    \param strRemainder Will contain the remainder of the string.
    \param strPredecessor Will contain the predecessor of the matched bracket.
    
    This function finds the first match of brackets in a
    string and returns the constained string and the 
    remainder. E.g.: "---<<<><>>---->*****" returns
    strResult: "<<><>>----"
    strRemainder: "*****"
    strPredecessor: "---"
    
*/
bool FindMatchingBracketsInString(const string& strIn, 
                                    string& strResult, 
                                    string& strRemainder, 
                                    string& strPredecessor);

/*! \fn bool FindTagInFile(const ifstream& oIn, const string& strIn, string& strPredecessor)
    \param oIn Filestream to search.
    \param strIn Tag to find.
    \param strPredecessor Will contain the predecessor of the tag found.
    
    This function finds the next tag specified in strTag
    and returns the predecessor.
*/
bool FindTagInFile(ifstream& oIn, 
                    const string& strIn, 
                    string& strPredecessor);



#endif
