/*
 *  NeuraAxApplicationController.h
 *  NeurAxApp
 *
 *  Created by Philip J. Broser on Tue Mar 02 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NEURAAXAPPLICATIONCONTROLLER
#define NEURAAXAPPLICATIONCONTROLLER

#include "../reconstructionlib/viewcube.h"
#include "../datalib/datacube.h"





class NeuraAxApplicationController
{
public:
    NeuraAxApplicationController();
    int LoadAxonCubeFromNih(char *name);
	int LoadAxonCubeFromTif(char *name);
    ViewCube *GetViewCube();
    
    
private:
    ViewCube *presentView;
    DataCube AxonCube;


};




#endif