#include "segmentation.h"

#include "moments.h"
#include <stdlib.h>

int structuralThresholdingAtPoint(intVectorType point ,densityDataType *sourceData, densityDataType *targetData, float absMin, float deltaEpsilon, float absMax,int scanningRange)
{
	float datum=ACCESSDATAATPOINT((*sourceData), point.x, point.y);  
	if(datum<absMin) 
	{
	ACCESSDATAATPOINT((*targetData), point.x, point.y)=0.0;
	return 0;
	}

	if(datum>absMax) 
	{
	ACCESSDATAATPOINT((*targetData), point.x, point.y)=1.0;
	return 0;
	}
	
	
	float massAroundPoint=calculateMassArroundPoint(*sourceData,  point, scanningRange);
	float volume=(scanningRange-1)*(scanningRange-1);
	float meanSignal=massAroundPoint/volume;
	
	if ((datum)>(meanSignal+deltaEpsilon)) 
	{
	//printf("Mean siganl: %f datum : %f\n",datum, meanSignal);
	//printf("Delta Epsilon:%f\n",deltaEpsilon);
	ACCESSDATAATPOINT((*targetData), point.x, point.y)=0.5;
	return 0;
	}
	
	ACCESSDATAATPOINT((*targetData), point.x, point.y)=0.0;
	return 0;
}


int momentThresholdingAtPoint(intVectorType point ,densityDataType *sourceData, densityDataType *targetData, float structuresignal, float absMin, float deltaEpsilon, float absMax,int scanningRange)
{
float datum=ACCESSDATAATPOINT((*sourceData), point.x, point.y);  
	if(datum<absMin) 
	{
	ACCESSDATAATPOINT((*targetData), point.x, point.y)=0.0;
	return 0;
	}

	if(datum>absMax) 
	{
	ACCESSDATAATPOINT((*targetData), point.x, point.y)=1.0;
	return 0;
	}
	float alpha, structure;
	momentOfInertiaType Inertia; 
	evaluateDirectionAndStructure(&alpha, &structure, &Inertia, *sourceData, point, 4);
	
	float massAroundPoint=calculateMassArroundPoint(*sourceData,  point, scanningRange);
	float volume=(scanningRange-1)*(scanningRange-1);
	float meanSignal=massAroundPoint/volume;


	
	if (structure>structuresignal && (datum)>(meanSignal+deltaEpsilon))
	{
	ACCESSDATAATPOINT((*targetData), point.x, point.y)=structure;
	return 0;

	}
	
	ACCESSDATAATPOINT((*targetData), point.x, point.y)=0.0;
	return 0;

}



int structuralThresholding (densityDataType data, densityDataType target)
{
    
    int scanningRange=5;
	//Segmentation
    int x,y;
    for (y=scanningRange+5;y<data.sizeY-(scanningRange+5);y++)
    {
	int percent= (int) ((y*100.0)/(data.sizeY-(scanningRange+5)));
	if ( ((float) (percent/10.0)) ==  ((int) (percent/10)) ) {printf("percent done: %d of segmentation\n",percent);};
	for (x=scanningRange+5;x<data.sizeX-(scanningRange+10);x++)
	{
	    intVectorType point;
	    point.x=x;
	    point.y=y;
	    structuralThresholdingAtPoint(point ,&data, &target, 0.01, 0.13, 0.5, scanningRange);
	};
    };
    return 0;
}


float meanArroundXY(densityDataType data, int x, int y, int  scanningRange)
{
	int dx, dy;
	int counter=0;
	float sum=0.0;
	for (dy=1-scanningRange;dy<scanningRange;dy=dy+1)
 		for (dx=1-scanningRange;dx<scanningRange;dx=dx+1)
		{
		counter=counter+1;
		sum=sum+ACCESSDATAATPOINT(data, x+dx, y+dy); 
		}

	return sum/counter;
 
}
	
int checkNeigbours(densityDataType data, int x, int y, int  scanningRange, float value, int minNr)
{
	int dx, dy;
	int counter=0;
	for (dy=-1;dy<=1;dy++)
		for (dx=-1;dx<=1;dx++)
			if (ACCESSDATAATPOINT(data, x+dx, y+dy)>value)
				counter=counter+1;
	return (counter>=minNr);
	
}





int structuralThresholdingWithParameter(densityDataType data, ucharDataType target, float globalMinThreshold, float globalMaxThreshold, float epsilon, int scanningRange)
{

// int scanningRange=5;
	//Segmentation
    int x,y;
    for (y=scanningRange+5;y<data.sizeY-(scanningRange+5);y++)
    {
	int percent= (int) ((y*100.0)/(data.sizeY-(scanningRange+5)));
	if ( ((float) (percent/10.0)) ==  ((int) (percent/10)) ) {printf("percent done: %d of segmentation\n",percent);};
	for (x=scanningRange+5;x<data.sizeX-(scanningRange+5);x++)
	{
	    intVectorType point;
	    point.x=x;
	    point.y=y;
	    float datum=ACCESSDATAATPOINT(data, point.x, point.y);  
		if(datum<globalMinThreshold) 
		{
			//printf("Global Min");
			SetBinaryDataAtPoint(target,x,y,0);
			continue;
		}

		if(datum>globalMaxThreshold) 
		{
		//printf("Global Max");
			SetBinaryDataAtPoint(target,x,y,1);
			continue;
		}
	
	float meanSignal=meanArroundXY(data, x, y, scanningRange);
	
	if ((datum)>(meanSignal+epsilon)) 
	{
	//printf("+Mean siganl: %f datum : %f\n", meanSignal, datum);
	//printf("Delta Epsilon:%f\n",epsilon);
	//printf("Check Neigbours");
	if (checkNeigbours(data, x, y, scanningRange, meanSignal+epsilon,3))
		{
		float structure;
		evaluateStructure(&structure, data,  point, scanningRange);
		if (structure>0.4) 
			{
			SetBinaryDataAtPoint(target,x,y,1);
			continue;
			};
		}
		}
		
	//printf("-Mean siganl: %f datum : %f\n",datum, meanSignal);
	//printf("Delta Epsilon:%f\n",epsilon);
	
		SetBinaryDataAtPoint(target,x,y,0);
	
    }; //x loop
    }; //y loop
    
	return 0;
}




/*======================================================================*/
/* OTSU global thresholding routine                                     */
/*   takes a 2D unsigned char array pointer, number of rows, and        */
/*   number of cols in the array. returns the value of the threshold    */
/* Modified by Ruizhen Liu, 02/07/2002					*/
/*======================================================================*/
int otsu (unsigned char *image, int rows, int cols, int x0, int y0, int dx, int dy, int vvv)
{
    unsigned char *np;    // pointer to position in the image we are working with
    int thresholdValue=1; // value we will threshold at
    int ihist[256];       // image histogram
    int i, j, k;          // various counters
    int n, n1, n2, gmin, gmax;
    double m1, m2, sum, csum, fmax, sb;
  // zero out histogram ...
    memset(ihist, 0, sizeof(ihist));
    gmin=255; gmax=0;
  // generate the histogram
    for (i = y0 + 1; i < y0 + dy - 1; i++) {
	np = &image[i*cols+x0+1];
	for (j = x0 + 1; j < x0 + dx - 1; j++) {
	    ihist[*np]++;
	    if(*np > gmax) gmax=*np;
	    if(*np < gmin) gmin=*np;
	    np++; /* next pixel */
	}
    }
  // set up everything
    sum = csum = 0.0;
    n = 0;
    for (k = 0; k <= 255; k++) {
	sum += (double) k * (double) ihist[k];  /* x*f(x) mass moment */
	n   += ihist[k];                        /*  f(x)    mass      */
    }
    if (!n) {
    // if n has no value we have problems...
	fprintf (stderr, "NOT NORMAL thresholdValue = 160\n");
	return (160);
    }
  // do the otsu global thresholding method
    fmax = -1.0;
    n1 = 0;
    for (k = 0; k < 255; k++) {
	n1 += ihist[k];
	if (!n1) { continue; }
	n2 = n - n1;
	if (n2 == 0) { break; }
	csum += (double) k *ihist[k];
	m1 = csum / n1;
	m2 = (sum - csum) / n2;
	sb = (double) n1 *(double) n2 *(m1 - m2) * (m1 - m2);
	/* bbg: note: can be optimized. */
	if (sb > fmax) {
	    fmax = sb;
	    thresholdValue = k;
	}
    }
  // at this point we have our thresholding value
  // debug code to display thresholding values
    if ( vvv & 1 )
	fprintf(stderr,"# OTSU: thresholdValue = %d gmin=%d gmax=%d\n",
		thresholdValue, gmin, gmax);
    return(thresholdValue);
}



float phi1(float mu, float psi)
{
    return (1.0-mu)*(1.0-psi); 
};

float phi2(float mu, float psi)
{
    return mu*(1.0-psi); 
};

float phi3(float mu, float psi)
{
    return (1.0-mu)*psi; 
};

float phi4(float mu, float psi)
{
    return mu*psi; 
};



int OtsuThresholding(densityDataType data, ucharDataType target)
{
    int gridSize=20; //must be divideable by 2
    int globalMaxThreshold=255;
    int globalMinThreshold=50;  //30;
    return OtsuThresholdingWithParameter(data,  target, globalMinThreshold, globalMaxThreshold,  gridSize);
};




/*
Sensible Values for parameter:
 int gridSize=20; //must be divideable by 2
 int globalMaxThreshold=255;
 int globalMinThreshold=50;  //30;
 
*/
int OtsuThresholdingWithParameter(densityDataType data, ucharDataType target, int globalMinThreshold, int globalMaxThreshold, int gridSize)
{

    
    unsigned char *image= (unsigned char *) malloc (data.sizeX*data.sizeY*sizeof (unsigned char));
    
    int rows=data.sizeY; 
    int cols=data.sizeX; 
    int x0=1; 
    int y0=1; 
    int dx=12;
    int dy=12;
    int vvv=0;// No out put exept of errors
    
    int z;
    for (z=0;z<data.sizeX*data.sizeY;z++) image[z]=(unsigned char)  ( (float) 255.0 * data.data[z]);
    
 /*   
    int gridSize=20; //must be divideable by 2
    int globalMaxThreshold=255;
    int globalMinThreshold=50;  //30;
   */ 
    
    
    
    
    int gridX=data.sizeX/gridSize+1;
    int gridY=data.sizeY/gridSize+1;
    
    unsigned char *thresholdValues= (unsigned char*) malloc (sizeof(unsigned char)*gridX*gridY);
    unsigned char thresholdResult;
    
    
    int gY, gX;
    for(gY=1;gY<(gridY-1);gY++) //starts at 1 befor no full element before, stops 1 before because no full elment out there
	for (gX=1;gX<(gridX-1);gX++)
	{
	    x0=gX*gridSize-gridSize/2;//move mid point 
	    y0=gY*gridSize-gridSize/2;
	    dx=gridSize;
	    dy=gridSize;
	    
	    thresholdResult=(unsigned char) otsu (image, rows, cols, x0, y0, dx, dy, vvv);
	    if (thresholdResult>globalMaxThreshold) thresholdResult=globalMaxThreshold;
	    if (thresholdResult<globalMinThreshold) thresholdResult=globalMinThreshold;
	    thresholdValues[gX+gY*gridX]= thresholdResult;
	};      
    
    
    //The boundary threshold has to be calcultaed different
    
    for(gY=1;gY<(gridY-1);gY++) //Boundary is maxium threshold
    {
	gX=gridX-1;
	x0=gX*gridSize-gridSize;//move mid point 
	    y0=gY*gridSize-gridSize/2;
	    dx=gridSize;
	    dy=gridSize;
	    
	    thresholdResult=(unsigned char) otsu (image, rows, cols, x0, y0, dx, dy, vvv);
	    if (thresholdResult>globalMaxThreshold) thresholdResult=globalMaxThreshold;
	    if (thresholdResult<globalMinThreshold) thresholdResult=globalMinThreshold;
	    thresholdValues[(gridX-1)+gY*gridX]= thresholdResult;
	    
    };  
    
    
    for(gY=1;gY<(gridY-1);gY++) //Boundary is maxium threshold
    {
	gX=0;
	x0=gX*gridSize;//move mid point 
	    y0=gY*gridSize-gridSize/2;
	    dx=gridSize;
	    dy=gridSize;
	    
	    thresholdResult=(unsigned char) otsu (image, rows, cols, x0, y0, dx, dy, vvv);
	    if (thresholdResult>globalMaxThreshold) thresholdResult=globalMaxThreshold;
	    if (thresholdResult<globalMinThreshold) thresholdResult=globalMinThreshold;
	    thresholdValues[0+gY*gridX]= thresholdResult;
    };   
    
    
    for (gX=1;gX<(gridX-1);gX++) //Boundary is maxium threshold
    {
	gY=0;
	x0=gX*gridSize-gridSize/2; /*move mid point*/ 
	y0=gY*gridSize;
	dx=gridSize;
	dy=gridSize;
	
	thresholdResult=(unsigned char) otsu (image, rows, cols, x0, y0, dx, dy, vvv);
	if (thresholdResult>globalMaxThreshold) thresholdResult=globalMaxThreshold;
	if (thresholdResult<globalMinThreshold) thresholdResult=globalMinThreshold;
	thresholdValues[gX+0*gridX]= thresholdResult;
	
    };
    
    for (gX=1;gX<(gridX-1);gX++) //Boundary is maxium threshold
    {
	gY=gridY-1;
	x0=gX*gridSize-gridSize/2; /*move mid point*/ 
	y0=gY*gridSize-gridSize;
	dx=gridSize;
	dy=gridSize;
	
	thresholdResult=(unsigned char) otsu (image, rows, cols, x0, y0, dx, dy, vvv);
	if (thresholdResult>globalMaxThreshold) thresholdResult=globalMaxThreshold;
	if (thresholdResult<globalMinThreshold) thresholdResult=globalMinThreshold;
	thresholdValues[gX+(gridY-1)*gridX]= thresholdResult;
	
    };
    
    //The four corners in a other way
    //Corner1
    gX=0;
    gY=0;
    x0=gX*gridSize; /*move mid point*/ 
    y0=gY*gridSize;
    dx=gridSize;
    dy=gridSize;
    thresholdResult=(unsigned char) otsu (image, rows, cols, x0, y0, dx, dy, vvv);
    if (thresholdResult>globalMaxThreshold) thresholdResult=globalMaxThreshold;
    if (thresholdResult<globalMinThreshold) thresholdResult=globalMinThreshold;
    thresholdValues[gX+gY*gridX]= thresholdResult;
    
    //Corner2
    gX=gridX-1;
    gY=0;
    x0=gX*gridSize-gridSize; /*move mid point*/ 
    y0=gY*gridSize;
    dx=gridSize;
    dy=gridSize;
    thresholdResult=(unsigned char) otsu (image, rows, cols, x0, y0, dx, dy, vvv);
    if (thresholdResult>globalMaxThreshold) thresholdResult=globalMaxThreshold;
    if (thresholdResult<globalMinThreshold) thresholdResult=globalMinThreshold;
    thresholdValues[gX+gY*gridX]= thresholdResult;
    
    //Corner3
    gX=0;
    gY=gridY-1;
    x0=gX*gridSize; /*move mid point*/ 
    y0=gY*gridSize-gridSize;
    dx=gridSize;
    dy=gridSize;
    thresholdResult=(unsigned char) otsu (image, rows, cols, x0, y0, dx, dy, vvv);
    if (thresholdResult>globalMaxThreshold) thresholdResult=globalMaxThreshold;
    if (thresholdResult<globalMinThreshold) thresholdResult=globalMinThreshold;
    thresholdValues[gX+gY*gridX]= thresholdResult;
    
    //Corner4
    gX=gridX-1;
    gY=gridY-1;
    x0=gX*gridSize-gridSize; /*move mid point*/ 
    y0=gY*gridSize-gridSize;
    dx=gridSize;
    dy=gridSize;
    thresholdResult=(unsigned char) otsu (image, rows, cols, x0, y0, dx, dy, vvv);
    if (thresholdResult>globalMaxThreshold) thresholdResult=globalMaxThreshold;
    if (thresholdResult<globalMinThreshold) thresholdResult=globalMinThreshold;
    thresholdValues[gX+gY*gridX]= thresholdResult;
    
    //Elementwise Interpolation 
    for( gY=0;gY<(gridY-1);gY++) 
	for ( gX=0;gX<(gridX-1);gX++)
	{
	    x0=gX*gridSize;
	    y0=gY*gridSize;
	    
	    int Node1=thresholdValues[gX+gY*gridX];
	    int Node2=thresholdValues[(gX+1)+gY*gridX];
	    int Node3=thresholdValues[gX+(gY+1)*gridX];
	    int Node4=thresholdValues[(gX+1)+(gY+1)*gridX];
	    
	    
	    int x,y;
	    for (y=0;y<gridSize;y++) 
		for (x=0;x<gridSize;x++) 
		{	
		    float psi=(float) x/gridSize;
		    float mu=(float) y/gridSize;
		    
		    float thresholdResult=(float) phi1(psi,mu)*Node1+phi2(psi,mu)*Node2+phi3(psi,mu)*Node3+phi4(psi,mu)*Node4;
		    
		    //printf("x:%d y:%d\n",x,y);
		    if (image[(x+x0)+(y+y0)*data.sizeX]>thresholdResult)
		    {
			SetBinaryDataAtPoint(target,(x+x0),(y+y0),1);
		    }
		    else
		    {
			SetBinaryDataAtPoint(target,(x+x0),(y+y0),0);
		    };
		};
	    
	};      
    
    
    return 0;
}


int globalThresholding (densityDataType data, ucharDataType segData, float value)
{
    int x,y;
    for ( y=0;y<data.sizeY;y++)
    {
	int percent= (int) ((y*100.0)/(data.sizeY));
	if ( ((float) (percent/10.0)) ==  ((int) (percent/10)) ) {printf("percent done: %d of segmentation\n",percent);};
	for ( x=0;x<data.sizeX;x++)
	{
	    if (ACCESSDATAATPOINT(data, x, y)>value) 
	    {
		SetBinaryDataAtPoint(segData,x,y,1);
	    }
	    else
		SetBinaryDataAtPoint(segData,x,y,0);
	    
	};
    };
    
	return 0;
};




















