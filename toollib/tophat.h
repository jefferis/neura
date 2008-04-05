/*
 *  tophat.h
 *  DistanceMap
 *
 *  Created by Philip J. Broser on Mon Aug 23 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */
#include "datatypes.h"

//range: value of 9 is sensible
int Tophat(densityDataType* data, int range, densityDataType* tophat);
//range: value of 9 is sensible
int Tophat2(densityDataType* data, int range, densityDataType* tophat);
