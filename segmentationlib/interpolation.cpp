
#include "interpolation.h"


    
interpolation::interpolation(Vector<int> smallSize, DataCube *cube):sizeSC(3)
{
  tDC=cube;
  
  for(int i=1; i<=3; i++)
    sizeSC[i]=smallSize[i];
}


interpolation::~interpolation()
{
}



double interpolation::phi (int j, int i, int h, int k, Vector<int> size)
{
  switch(j)
    {
    case 1: return (1.0-(double)i/((double)size[1]/2.0))*(1.0-(double)h/((double)size[2]/2.0))*(1.0-(double)k/((double)size[3]/2.0));
    case 2: return (double)i/((double)size[1]/2.0)*(1.0-(double)h/((double)size[2]/2.0))*(1.0-(double)k/((double)size[3]/2.0));
    case 3: return (double)i/((double)size[1]/2.0)*(double)h/((double)size[2]/2.0)*(1.0-(double)k/((double)size[3]/2.0));
    case 4: return (1.0-(double)i/((double)size[1]/2.0))*(double)h/((double)size[2]/2.0)*(1.0-(double)k/((double)size[3]/2.0));
    case 5: return (1.0-(double)i/((double)size[1]/2.0))*(1.0-(double)h/((double)size[2]/2.0))*(double)k/((double)size[3]/2.0);
    case 6: return (double)i/((double)size[1]/2.0)*(1.0-(double)h/((double)size[2]/2.0))*(double)k/((double)size[3]/2.0);
    case 7: return (double)i/((double)size[1]/2.0)*(double)h/((double)size[2]/2.0)*(double)k/((double)size[3]/2.0);
    case 8: return (1.0-(double)i/((double)size[1]/2.0))*(double)h/((double)size[2]/2.0)*(double)k/((double)size[3]/2.0);    
    }
    return -1.0;
}



int 
interpolation:: 
interpolateDataCube ()
{
  int x,y,z;
  double helpFieldValue, value;
  double weights[8][sizeSC[1]/2][sizeSC[2]/2][sizeSC[3]/2];
  
  Vector<int> size(3),start(3);
  size=tDC->GetSize();
  start=tDC->GetStart();
 

  
  // Calculation of the weigths for the points of one small cube
  for (int j=1; j<=8; j++)
    {   
      for (int i=0; i<sizeSC[1]/2; i++)
	{
	  for (int h=0; h<sizeSC[2]/2; h++)
	    {
	      for (int l=0; l<sizeSC[3]/2; l++)
		{
		  weights[j-1][i][h][l]=phi(j,i,h,l,sizeSC);
		}
	    }
	}
    }




  for (int i=start[1]; i<start[1]+size[1]; i++)
    {
      //cout<<endl;
      for (int h=start[2]; h<start[2]+size[2]; h++)
	{
	  //cout<<endl<<endl;
	  for (int l=start[3]; l<start[3]+size[3]; l++)
	    {
	      x=((int)i/(sizeSC[1]/2))*(sizeSC[1]/2);
	      y=((int)h/(sizeSC[2]/2))*(sizeSC[2]/2);
	      z=((int)l/(sizeSC[3]/2))*(sizeSC[3]/2);
	      
		helpFieldValue=0.0;
		for (int j=1; j<=8; j++)
 		  {
		    switch(j)
		      {
		      case 1: value=(*tDC)(x,y,z); break;
		      case 2:
			{
			  if (x+sizeSC[1]/2>=start[1]+size[1])
			    value=(*tDC)(x,y,z);
			   if (x+sizeSC[1]/2<start[1]+size[1])
			    value=(*tDC)(x+sizeSC[1]/2, y, z);
			   break;
			}
		      case 3:
			{
			  if(x+sizeSC[1]/2>=start[1]+size[1] && y+sizeSC[2]/2>=start[2]+size[2])
			    value=(*tDC)(x,y,z);
			  if(x+sizeSC[1]/2<start[1]+size[1] && y+sizeSC[2]/2>=start[2]+size[2])
			    value=(*tDC)(x+sizeSC[1]/2,y,z);
			  if(x+sizeSC[1]/2>=start[1]+size[1] && y+sizeSC[2]/2<start[2]+size[2])
			    value=(*tDC)(x,y+sizeSC[2]/2,z);
			  if(x+sizeSC[1]/2<start[1]+size[1] && y+sizeSC[2]/2<start[2]+size[2])
			    value=(*tDC)(x+sizeSC[1]/2,y+sizeSC[2]/2,z);
			  break;
			}
		      case 4:
			{
			  if (y+sizeSC[2]/2>=start[2]+size[2])
			    value=(*tDC)(x,y,z);
			  else  value=(*tDC)(x, y+sizeSC[2]/2,z);
			  break;
			}
		      case 5:
			{
			  if (z+sizeSC[3]/2>=start[3]+size[3])
			    value=(*tDC)(x,y,z);
			  if (z+sizeSC[3]/2<start[3]+size[3])
			    value=(*tDC)(x,y,z+sizeSC[3]/2);
			  break;
			}
		      case 6:
			{			
			  if(x+sizeSC[1]/2>=start[1]+size[1] && z+sizeSC[3]/2>=start[3]+size[3])
			    value=(*tDC)(x,y,z);
			  if(x+sizeSC[1]/2<start[1]+size[1] && z+sizeSC[3]/2>=start[3]+size[3])
			    value=(*tDC)(x+sizeSC[1]/2,y,z);
			  if(x+sizeSC[1]/2>=start[1]+size[1] && z+sizeSC[3]/2<start[3]+size[3])
			    value=(*tDC)(x,y,z+sizeSC[3]/2);
			  
			  if(x+sizeSC[1]/2<start[1]+size[1] && z+sizeSC[3]/2<start[3]+size[3])
			    value=(*tDC)(x+sizeSC[1]/2,y, z+sizeSC[3]/2);
			  break;
			}

		      case 7:
			{			
			  if(x+sizeSC[1]/2>=start[1]+size[1] && y+sizeSC[2]/2>=start[2]+size[2] && z+sizeSC[3]/2>=start[3]+size[3])
			    value=(*tDC)(x,y,z);
			  
			  if(x+sizeSC[1]/2<start[1]+size[1] && y+sizeSC[2]/2>=start[2]+size[2] && z+sizeSC[3]/2>=start[3]+size[3])
			    value=(*tDC)(x+sizeSC[1]/2,y,z);
			  if(x+sizeSC[1]/2>=start[1]+size[1] && y+sizeSC[2]/2<start[2]+size[2] && z+sizeSC[3]/2>=start[3]+size[3])
			    value=(*tDC)(x, y+sizeSC[2]/2,z);
			  if(x+sizeSC[1]/2>=start[1]+size[1] && y+sizeSC[2]/2>=start[2]+size[2] && z+sizeSC[3]/2<start[3]+size[3])
			    value=(*tDC)(x, y, z+sizeSC[3]/2);
			  
			  if(x+sizeSC[1]/2>=start[1]+size[1] && y+sizeSC[2]/2<start[2]+size[2] && z+sizeSC[3]/2<start[3]+size[3])
			    value=(*tDC)(x, y+sizeSC[2]/2, z+sizeSC[3]/2);
			  if(x+sizeSC[1]/2<start[1]+size[1] && y+sizeSC[2]/2>=start[2]+size[2] && z+sizeSC[3]/2<start[3]+size[3])
			    value=(*tDC)(x+sizeSC[2]/2, y, z+sizeSC[3]/2);
			  if(x+sizeSC[1]/2<start[1]+size[1] && y+sizeSC[2]/2<start[2]+size[2] && z+sizeSC[3]/2>=start[3]+size[3])
			    value=(*tDC)(x+sizeSC[2]/2, y+sizeSC[2]/2, z);
			  
			  if(x+sizeSC[1]/2<start[1]+size[1] && y+sizeSC[2]/2<start[2]+size[2] && z+sizeSC[3]/2<start[3]+size[3])
			    value=(*tDC)(x+sizeSC[2]/2, y+sizeSC[2]/2, z+sizeSC[3]/2);
			  break;
			}
		      case 8:
			{			
			  if(y+sizeSC[2]/2>=start[2]+size[2] && z+sizeSC[3]/2>=start[3]+size[3])
			    value=(*tDC)(x,y,z);
			  if(y+sizeSC[2]/2<start[2]+size[2] && z+sizeSC[3]/2>=start[3]+size[3])
			    value=(*tDC)(x,y+sizeSC[2]/2,z);
			  if(y+sizeSC[2]/2>=start[2]+size[2] && z+sizeSC[3]/2<start[3]+size[3])
			    value=(*tDC)(x,y,z+sizeSC[3]/2);
			  if(y+sizeSC[2]/2<start[2]+size[2] && z+sizeSC[3]/2<start[3]+size[3])
			    value=(*tDC)(x, y+sizeSC[2]/2, z+sizeSC[3]/2);
			  break;
			}
		      }		   
		    helpFieldValue=helpFieldValue+weights[j-1][i-x][h-y][l-z]*value;
		  }
		
		(*tDC)(i,h,l)=(NeuraDataType) helpFieldValue;
		//cout<<(*tDC)(i,h,l)<<"  ";
	      }
	  }
      }
	return 0;
 }
