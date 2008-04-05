/*
 *  morphology.c
 *  DistanceMap
 *
 *  Created by Philip J. Broser on Thu Jul 22 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

#include "morphology.h"

#include <math.h>
#include <stdio.h>


unsigned char BinaryMaxInNeibouhood(ucharDataType data, int x0, int y0, int r)
{
    int x,y;
    for (y=y0-r;y<(y0+r+1);y++)
	for (x=x0-r;x<(x0+r+1);x++)	    
	    if (GetBinaryDataAtPoint(data,x,y)) return 1;
    
    
    return 0;
}




int binaryClose(ucharDataType data, ucharDataType target)
{
    int x,y;
    int r=1;
    for ( y=r;y<data.sizeY-r;y++)
    {
#ifdef INFO
	int percent= (int) ((y*100.0)/(data.sizeY));
	if ( ((float) (percent/10.0)) ==  ((int) (percent/10)) ) {printf("percent done: %d of segmentation\n",percent);};
#endif
	for ( x=r;x<data.sizeX-r;x++)
	{
	    SetBinaryDataAtPoint(target,x,y,BinaryMaxInNeibouhood(data,x,y,r));
	    	    
	};
    };
    
    
    return 0;
}
