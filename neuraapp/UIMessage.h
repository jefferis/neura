/*
 *  UIMessage.h
 *  QTViewer03
 *
 *  Created by Alexander Heusel on Fri May 30 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _CUIMESSAGE_H_
#define _CUIMESSAGE_H_

#include "../nraConfig.h"
#include "../nraAssert.h"

#include "../reconstructionlib/viewcube.h"

#include "../nraSmartPointer.h"

class CUIMessage
{
public:
    virtual void PolyDummy(){};
};

class CUIAddMark : public CUIMessage
{
public:
    CUIAddMark(int iTag, int iType, bool boolAccLabel, double dblX, double dblY)
    {
        m_iTag = iTag;
        m_iType = iType;
        m_boolAccLabel = boolAccLabel;
        m_dblX = dblX;
        m_dblY = dblY;
        
    };
    inline int GetTag() const {return m_iTag;};
    inline int GetType() const {return m_iType;};
    inline bool GetAccLabel() const {return m_boolAccLabel;};
    inline double GetX() const {return m_dblX;};
    inline double GetY() const {return m_dblY;};
    
private:
    int m_iTag;
    int m_iType;
    bool m_boolAccLabel;
    double m_dblX;
    double m_dblY;
};

class CUIDestroyMark : public CUIMessage
{
public:
    CUIDestroyMark(int iTag){m_iTag = iTag;};
    inline int GetTag() const {return m_iTag;};

private:
    int m_iTag;
};

class CUIMoveMark : public CUIMessage
{
public:
    CUIMoveMark(int iTag, double dblX, double dblY)
    {
        m_iTag = iTag;
        m_dblX = dblX;
        m_dblY = dblY;
    };
    inline int GetTag() const {return m_iTag;};
    inline double GetX() const {return m_dblX;};
    inline double GetY() const {return m_dblY;};
    
private:
    int m_iTag;
    double m_dblX;
    double m_dblY;    
}; 


class CUISetLabel : public CUIMessage
{
public:
    CUISetLabel(int iTag, nraSharedArray<char> Label){m_iTag = iTag; m_Label = Label;};
    inline int GetTag() const {return m_iTag;};
    inline nraSharedArray<char> GetLabel() {return m_Label;};    

private:
    int m_iTag;
    nraSharedArray<char> m_Label;

}; 

class CUISetSlider : public CUIMessage
{
public:
    CUISetSlider(int iSliderMin, int iSliderMax, int iSliderVal)
    {
        m_iSliderMin = iSliderMin;
        m_iSliderMax = iSliderMax;
        m_iSliderVal = iSliderVal;
    };
    inline int GetMax() const {return m_iSliderMax;};
    inline int GetMin() const {return m_iSliderMin;};
    inline int GetVal() const {return m_iSliderVal;};
    
private:
    int m_iSliderMin;
    int m_iSliderMax;
    int m_iSliderVal;
};

class CUISetSlice : public CUIMessage
{
public:
    CUISetSlice(int iSliceNmbr){m_iSlice = iSliceNmbr;};
    inline int GetSlice() const {return m_iSlice;};
    
private:
    int m_iSlice;
}; 

class CUISetStack : public CUIMessage
{
public:
    CUISetStack(nraSharedPointer<ViewCube> Stack)
    {
        m_Stack = Stack;
    };
    inline nraSharedPointer<ViewCube> GetStack() const{return m_Stack;};

private:
    nraSharedPointer<ViewCube> m_Stack;

}; 

class CUIDisable : public CUIMessage
{
};

#endif




