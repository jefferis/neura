/*
 *  NeuraAxApplicationController.cpp
 *  NeurAxApp
 *
 *  Created by Philip J. Broser on Tue Mar 02 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "NeuraAxApplicationController.h"


NeuraAxApplicationController::
NeuraAxApplicationController() : AxonCube(3)
{
    cout << "Create ViewCube" << endl;
    presentView = new ViewCube(256, 256, 1);

}

int 
NeuraAxApplicationController::
LoadAxonCubeFromNih(char *name)
{
    
    cout << "Read File:" << name << endl;
    
    if (AxonCube.ReadFromNIH(name))
    {
	cout << "Error during file reading" << endl;
	return 1;
    
    };
    delete presentView;
    presentView = new ViewCube(&AxonCube,linear);
    
    return 0;
}

int 
NeuraAxApplicationController::
LoadAxonCubeFromTif(char *name)
{
    
    cout << "Read File:" << name << endl;
    if (AxonCube.ReadFromTIFF(name))
    {
	cout << "Error during file reading" << endl;
	return 1;
    
    };
    delete presentView;
    presentView = new ViewCube(&AxonCube,linear);
    
    return 0;
}



ViewCube*
NeuraAxApplicationController::
GetViewCube()
{
    
    return presentView;

}