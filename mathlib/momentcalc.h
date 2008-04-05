/****************************************************************************/
/*                                                                          */
/* File:      momentcalc.h                                                  */
/*                                                                          */
/* Purpose:   classes to calculate integrants for moments                   */
/*                                                                          */
/* Author:    Roland Schulte                                                */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     roland.schulte@iwr.uni-heidelberg.de                          */
/*                                                                          */
/* History:   07.11.02 begin                                                */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
#ifndef MOMENTCALC
#define MOMENTCALC

#include "integrant.h"
#include "../datalib/vector.h"
//#include "moments.h"
#include <math.h>

 
class VolumeIntegrant : public Integrant {

 public:

  virtual NeuraDataType g(NeuraDataType pixel, const Vector<int> &coord);
   
};


class MassIntegrant : public Integrant{

 public: 

  virtual NeuraDataType g(NeuraDataType pixel, const Vector<int> &coord);
   
};


class Focal_point_x : public Integrant{

public:

  virtual NeuraDataType g(NeuraDataType pixel, const Vector<int> &coord);
   

};


class Focal_point_y : public Integrant{

 public:

  virtual NeuraDataType g(NeuraDataType pixel, const Vector<int> &coord);
   
};



class Focal_point_z : public Integrant{

public:

  virtual NeuraDataType g(NeuraDataType pixel, const Vector<int> &coord);


};

class Inertian_tensor : public Integrant{

 public:

  static Vector<NeuraDataType> focal_point;

  virtual void prepare();

  virtual NeuraDataType g(NeuraDataType pixel, const Vector<int> &coord){return 0.0;}

  void SetFocalPoint(Vector<NeuraDataType> fp);
  
 
}; 

class Inertian_tensor11 : public Inertian_tensor{

 public:

  virtual NeuraDataType g(NeuraDataType pixel, const Vector<int> &coord);
  

};

class Inertian_tensor12 : public Inertian_tensor{

 public:

  virtual NeuraDataType g(NeuraDataType pixel, const Vector<int> &coord);   

};

class Inertian_tensor22 : public Inertian_tensor{

 public:

  virtual NeuraDataType g(NeuraDataType pixel, const Vector<int> &coord);
  
};

class Inertian_tensor13 : public Inertian_tensor{

 public:

  virtual NeuraDataType g(NeuraDataType pixel, const Vector<int> &coord);   

};

class Inertian_tensor23 : public Inertian_tensor{

 public:

  virtual NeuraDataType g(NeuraDataType pixel, const Vector<int> &coord);   

};

class Inertian_tensor33 : public Inertian_tensor{

 public:

  virtual NeuraDataType g(NeuraDataType pixel, const Vector<int> &coord);
  
};


#endif
