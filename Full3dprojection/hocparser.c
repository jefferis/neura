/*
 *  parser.c
 *  full3dproj
 *
 *  Created by Philip Julian  Broser on Sun Jan 25 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */
#include <stdio.h>
#include "lex.yy.c"
#include "hocparser.h"
FILE *hocFile;
double pt3dVector[4];

//create varibale, access, 
char name1[80]; //coressponding
char name2[80];
double value1, value2;


// yylval;
// OPENBRACKET 1
// CLOSEBRACKET 2
// SEPERATOR 3 
// DEFFENITION 4
// NUMBER 5


int openHocFile(char *fileName)
{
    hocFile=fopen(fileName,"r");
    if (hocFile==NULL) 
    {
	printf("FILE IO Error\n");
	return CANNOTOPENFILE;
    }
	yyin=hocFile;
    return 0;
}

int read4Vector()
{
    
    if(yylex()!=OPENBRACKET) return ERRORREADINGFILE;
    
    if(yylex()!=NUMBER) return ERRORREADINGFILE;
    pt3dVector[0]=yylval;
    if(yylex()!=SEPERATOR) return ERRORREADINGFILE;
    
    if(yylex()!=NUMBER) return ERRORREADINGFILE;
    pt3dVector[1]=yylval;
    if(yylex()!=SEPERATOR) return ERRORREADINGFILE;
    
    if(yylex()!=NUMBER) return ERRORREADINGFILE;
    pt3dVector[2]=yylval;
    if(yylex()!=SEPERATOR) return ERRORREADINGFILE;
    
    if(yylex()!=NUMBER) return ERRORREADINGFILE;
    pt3dVector[3]=yylval;
        
    if(yylex()!=CLOSEBRACKET) return ERRORREADINGFILE;

    return PT3DADD; 
}



int pt3dadd()
{
    return read4Vector();
}

int accessS()
{
    if (yylex()!=VARIABLE) return ERRORREADINGFILE;
    strcpy (name1,yytext);
    return ACCESS;
}

int createS()
{
    if (yylex()!=VARIABLE) return ERRORREADINGFILE;
    strcpy (name1,yytext);
    return CREATE;
}

int pt3dclear()
{
    if(yylex()!=OPENBRACKET) return ERRORREADINGFILE;
    if(yylex()!=CLOSEBRACKET) return ERRORREADINGFILE;
    return PT3DCLEAR;
}

int defention()

{
    strcpy (name1,yytext); // copy name of variable
    if(yylex()!=DEFFENITION) return ERRORREADINGFILE;
    if(yylex()!=NUMBER) return ERRORREADINGFILE;
    value1=yylval;
    return DEFFENITION;
}

int connectS()
{
    if (yylex()!=VARIABLE) return ERRORREADINGFILE;
    strcpy (name1,yytext);
    if(yylex()!=OPENBRACKET) return ERRORREADINGFILE;
    if(yylex()!=NUMBER) return ERRORREADINGFILE;
    value1=yylval;
    if(yylex()!=CLOSEBRACKET) return ERRORREADINGFILE;
    if(yylex()!=SEPERATOR) return ERRORREADINGFILE;
    if (yylex()!=VARIABLE) return ERRORREADINGFILE;
    strcpy (name2,yytext);
    if(yylex()!=OPENBRACKET) return ERRORREADINGFILE;
    if(yylex()!=NUMBER) return ERRORREADINGFILE;
    value2=yylval;
    if(yylex()!=CLOSEBRACKET) return ERRORREADINGFILE;
    return CONNECT;
}


int readTag()
{
   
    int tag=yylex();
    switch (tag)
    {
	
	case PT3DADD : return pt3dadd();break;
	case ACCESS  : return accessS(); break;
	case CREATE  : return createS();break;
	case PT3DCLEAR : return pt3dclear();break;
	case VARIABLE :  return defention();break;
	case CONNECT : return connectS();break;
	case ERRORREADINDFILE : return ERRORREADINGFILE;    
	case NEWLINE:   
	default: ;break;
	    
    }
    
    return tag;// returns 0 when no more tag is detected
    
}

int closeFile()
{
    fclose(hocFile);
    return 0;

}





