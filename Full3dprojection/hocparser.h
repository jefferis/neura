/*
 *  parser.h
 *  full3dproj
 *
 *  Created by Philip Julian  Broser on Sun Jan 25 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */
#define ERRORREADINGFILE -1
#define CANNOTOPENFILE -1
#define OK 0

//pt3dadd
extern double pt3dVector[4];

//create varibale, access, 
extern char name1[80]; //coressponding
extern char name2[80];
extern double value1, value2;

int openHocFile(char *fileName);
int readTag();
int closeFile();