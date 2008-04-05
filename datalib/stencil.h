/****************************************************************************/
/*                                                                          */
/* File:      stencil.h                                                     */
/*                                                                          */
/* Purpose:   datastructur for stencils                                     */
/*                                                                          */
/* Author:    Roland Schulte                                                */
/*            Interdisziplinaeres Zentrum fuer Wissenschaftliches Rechnen   */
/*            Universitaet Heidelberg                                       */
/*            Im Neuenheimer Feld 368                                       */
/*            D-69120 Heidelberg                                            */
/*                                                                          */
/* EMail:     roland.schulte@iwr.uni-heidelberg.de                          */
/*                                                                          */
/* History:   05.12.02 begin                                                */
/*                                                                          */
/* Remarks:                                                                 */
/*                                                                          */
/****************************************************************************/
#ifndef STENCIL
#define STENCIL

class Stencil{


 public:
  
  //returns the dimension of the stencil
  int GetDim();

   
 protected:
  
  int dim; // dimension of the stencil
  
  
};

#endif
