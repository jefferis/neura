#include "filteroptions.h"

FilterOptions::
FilterOptions()
{
 
  tau = 2.0;       
  time_steps = 4;   
  epsilon = 0.01;   
  levels = 1;       
  
  fixed_coeffs = YES; 
  anicoeff1 = 1.0;
  anicoeff2 = 1.0; 
  anicoeff3 = 0.0; 
  dependence_type = PERONA_MALIK;         
  lambda = 0.1;     
  gt = CUBE;                   
  integration_size_x = 10;
  integration_size_y = 10;
  integration_size_z = 10;          
  ip_flag = IP_BND;  
  
}
