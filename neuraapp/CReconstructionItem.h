/*
 *  CReconstructionItem.h
 *  QTViewer03
 *
 *  Created by Alexander Heusel on Tue May 20 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CRECONSTRUCTIONITEM_H_
#define _CRECONSTRUCTIONITEM_H_

#include "../nraConfig.h"
#include "../nraAssert.h"

#include "../nraSmartPointer.h"
 
#include <queue.h>
#include "../reconstructionlib/reconstruction.h"
#include "../datalib/datacube.h"
#include "UIMessage.h"


class CReconItem
{
public:
    CReconItem(nraWeakPointer<Reconstruction>& Recon);
    virtual ~CReconItem(){};
    virtual void AddMark(int iMarkType, double dblX, double dblY) = 0;
    virtual void DestroyMark(int iMarkTag) = 0;
    virtual void MoveMark(int iMarkTag, double dblX, double dblY) = 0;
    virtual void SetLabel(int iMarkTag, nraSharedArray<char> Label) = 0;
    virtual void SetDisplayType(int iDisplayType) = 0;
    virtual void SelectSlice(int iSliceNmbr) = 0;
    virtual void Visible() = 0;
    virtual void Invisible() = 0;
    virtual void Execute(nraSharedPointer<Options>& RecOptions) = 0;
    virtual bool HasCube() = 0;

    // After a call of one of the upper methods this queue
    // contains all messages neccessary to obtain a valid
    // display.
    queue< nraSharedPointer<CUIMessage> > MessageQueue;

    // Connect this reconstruction item with following one, e.g.
    // Loading with Preprocess.
    inline void SetFollowUp(nraWeakPointer<CReconItem> FollowUp) {m_FollowUp = FollowUp;};
    virtual void Update(bool boolValid) = 0;
    
    // Saves the current cube that belongs to a reconstruction
    // item to disk.
    void SaveCube(char* cFileName, FILETYPE FileType);
    
    // Set the preferences (brightness and constrast)
    void SetPrefs(int iBrightness, int iContrast);
    
     
protected:
    void UpdateStack();
    inline bool IsValid() {return m_Stack.IsValid();};
    inline nraSharedPointer<ViewCube> GetStack() {return m_Stack;};
    inline KindOfCubes GetCubeType() const {return m_eCubeType;};
    inline void SetCubeType(KindOfCubes eCubeType) {m_eCubeType = eCubeType;};
    nraWeakPointer<Reconstruction> m_Reconstruction;
    nraWeakPointer<CReconItem> m_FollowUp;
    
    nraSharedPointer<ViewCube> m_Stack;
private:
    
    bool m_boolIsValid;
    KindOfCubes m_eCubeType;
};


class CDummyItem : public CReconItem
{
public:
    CDummyItem(nraWeakPointer<Reconstruction>& Recon) : CReconItem(Recon) {};
    virtual ~CDummyItem() {};
    virtual void AddMark(int iMarkType, double dblX, double dblY)  {};
    virtual void DestroyMark(int iMarkTag) {};
    virtual void MoveMark(int iMarkTag, double dblX, double dblY) {};
    virtual void SetLabel(int iMarkTag, nraSharedArray<char> Label) {};
    virtual void SetDisplayType(int iDisplayType) {};
    virtual void SelectSlice(int iSliceNmbr) {};
    virtual void Visible() {};
    virtual void Invisible() {};
    virtual void Execute(nraSharedPointer<Options>& RecOptions) {};
    virtual void Update(bool boolValid) {};
    virtual bool HasCube() {return false;};
    
};


class CLoadingItem : public CReconItem
{
public:
    CLoadingItem(nraWeakPointer<Reconstruction>& Recon);
    virtual ~CLoadingItem() {};
    virtual void AddMark(int iMarkType, double dblX, double dblY);
    virtual void DestroyMark(int iMarkTag);
    virtual void MoveMark(int iMarkTag, double dblX, double dblY);
    virtual void SetLabel(int iMarkTag, nraSharedArray<char> Label) {};
    virtual void SetDisplayType(int iDisplayType) {};
    virtual void SelectSlice(int iSliceNmbr);
    virtual void Visible();
    virtual void Invisible() {};
    virtual void Execute(nraSharedPointer<Options>& RecOptions);
    virtual void Update(bool boolValid) {};
    virtual bool HasCube()
    {
	NRA_ASSERT(m_Reconstruction.IsValid(), "Invalid reconstruction pointer in CLoadingItem!"); 
	return m_Reconstruction->GetState() == 0;
    };

private:
    void PushMarks();
    void ClearList();
    list< nraSharedPointer<Mark> > m_MarkList;
    int m_iCurrentSlice;

    
};



class CTransformItem : public CReconItem
{
public:
    CTransformItem(nraWeakPointer<Reconstruction>& Recon);
    virtual ~CTransformItem() {};
    virtual void AddMark(int iMarkType, double dblX, double dblY){};
    virtual void DestroyMark(int iMarkTag){};
    virtual void MoveMark(int iMarkTag, double dblX, double dblY){};
    virtual void SetLabel(int iMarkTag, nraSharedArray<char> Label){};
    virtual void SetDisplayType(int iDisplayType) {};
    virtual void SelectSlice(int iSliceNmbr);
    virtual void Visible();
    virtual void Invisible() {};
    virtual void Execute(nraSharedPointer<Options>& RecOptions);
    virtual void Update(bool boolValid);
    virtual bool HasCube() 
    {
		NRA_ASSERT(m_Reconstruction.IsValid(), "Invalid reconstruction pointer in CTransformItem!"); 
		return m_Reconstruction->GetState() > 0;
    };

private:
    int m_iCurrentSlice;
    bool m_boolUpdate;
    bool m_boolHasData;
    
};

class CFFilterItem : public CReconItem
{
public:
    CFFilterItem(nraWeakPointer<Reconstruction>& Recon);
    virtual ~CFFilterItem() {};
    virtual void AddMark(int iMarkType, double dblX, double dblY)  {};
    virtual void DestroyMark(int iMarkTag) {};
    virtual void MoveMark(int iMarkTag, double dblX, double dblY) {};
	virtual void SetLabel(int iMarkTag, nraSharedArray<char> Label) {};
    virtual void SetDisplayType(int iDisplayType) {};
    virtual void SelectSlice(int iSliceNmbr);
    virtual void Visible();
    virtual void Invisible() {};
    virtual void Execute(nraSharedPointer<Options>& RecOptions);
    virtual void Update(bool boolValid);
    virtual bool HasCube()
    {
	NRA_ASSERT(m_Reconstruction.IsValid(), "Invalid reconstruction pointer in CFFilterItem!"); 
	return m_Reconstruction->GetState() > 1;
    };

private:
    int m_iCurrentSlice;
    bool m_boolUpdate;
    bool m_boolHasData;
    
};

class CFSegmItem : public CReconItem
{
public:
    CFSegmItem(nraWeakPointer<Reconstruction>& Recon);
    virtual ~CFSegmItem() {};
    virtual void AddMark(int iMarkType, double dblX, double dblY) {};
    virtual void DestroyMark(int iMarkTag) {};
    virtual void MoveMark(int iMarkTag, double dblX, double dblY) {};
	virtual void SetLabel(int iMarkTag, nraSharedArray<char> Label) {};
    virtual void SetDisplayType(int iDisplayType) {};
    virtual void SelectSlice(int iSliceNmbr);
    virtual void Visible();
    virtual void Invisible() {};
    virtual void Execute(nraSharedPointer<Options>& RecOptions);
    virtual void Update(bool boolValid);
    virtual bool HasCube()
    {
	NRA_ASSERT(m_Reconstruction.IsValid(), "Invalid reconstruction pointer in CFSegmItem!"); 
	return m_Reconstruction->GetState() > 2;
    };

private:
    int m_iCurrentSlice;
    bool m_boolUpdate;
    bool m_boolHasData;
    
};

class CSFilterItem : public CReconItem
{
public:
    CSFilterItem(nraWeakPointer<Reconstruction>& Recon);
    virtual ~CSFilterItem() {};
    virtual void AddMark(int iMarkType, double dblX, double dblY)  {};
    virtual void DestroyMark(int iMarkTag) {};
    virtual void MoveMark(int iMarkTag, double dblX, double dblY) {};
	virtual void SetLabel(int iMarkTag, nraSharedArray<char> Label) {};
    virtual void SetDisplayType(int iDisplayType) {};
    virtual void SelectSlice(int iSliceNmbr);
    virtual void Visible();
    virtual void Invisible() {};
    virtual void Execute(nraSharedPointer<Options>& RecOptions);
    virtual void Update(bool boolValid);
    virtual bool HasCube()
    {
	NRA_ASSERT(m_Reconstruction.IsValid(), "Invalid reconstruction pointer in CSFilterItem!"); 
	return m_Reconstruction->GetState() > 3;
    };

private:
    int m_iCurrentSlice;
    bool m_boolUpdate;
    bool m_boolHasData;
    
};

class CSSegmItem : public CReconItem
{
public:
    CSSegmItem(nraWeakPointer<Reconstruction>& Recon);
    virtual ~CSSegmItem() {};
    virtual void AddMark(int iMarkType, double dblX, double dblY)  {};
    virtual void DestroyMark(int iMarkTag) {};
    virtual void MoveMark(int iMarkTag, double dblX, double dblY) {};
	virtual void SetLabel(int iMarkTag, nraSharedArray<char> Label) {};
    virtual void SetDisplayType(int iDisplayType) {};
    virtual void SelectSlice(int iSliceNmbr);
    virtual void Visible();
    virtual void Invisible() {};
    virtual void Execute(nraSharedPointer<Options>& RecOptions);
    virtual void Update(bool boolValid);
    virtual bool HasCube()
    {
	NRA_ASSERT(m_Reconstruction.IsValid(), "Invalid reconstruction pointer in CSSegmItem!"); 
	return m_Reconstruction->GetState() > 4;
    };

private:
    int m_iCurrentSlice;
    bool m_boolUpdate;
    bool m_boolHasData;
    
};

class CPreprocItem : public CReconItem
{
public:
    CPreprocItem(nraWeakPointer<Reconstruction>& Recon);
    virtual ~CPreprocItem();
    virtual void AddMark(int iMarkType, double dblX, double dblY);
    virtual void DestroyMark(int iMarkTag);
    virtual void MoveMark(int iMarkTag, double dblX, double dblY);
	virtual void SetLabel(int iMarkTag, nraSharedArray<char> Label){};
    virtual void SetDisplayType(int iDisplayType) {};
    virtual void SelectSlice(int iSliceNmbr);
    virtual void Visible();
    virtual void Invisible() {};
    virtual void Execute(nraSharedPointer<Options>& RecOptions);
    virtual void Update(bool boolValid);
    virtual bool HasCube()
    {
	NRA_ASSERT(m_Reconstruction.IsValid(), "Invalid reconstruction pointer in CLoadingItem!"); 
	return m_Reconstruction->GetState() > 5;
    };
    
private:
    // Class for internal storage of marks.
    class CPreprocMark
    {
    public:
        CPreprocMark() {m_dblX = 0; m_dblY = 0 ; m_iSlice = -1; m_iTag = -1; m_boolIsValid = false;};
        inline double GetX() const {return m_dblX;};
        inline double GetY() const {return m_dblY;};
        inline double GetSlice() const {return m_iSlice;};
        inline void SetX(double dblX) {m_dblX = dblX;};
        inline void SetY(double dblY) {m_dblY = dblY;};
        inline void SetSlice(int iSlice) {m_iSlice = iSlice;};
        inline bool IsValid() const {return m_boolIsValid;}
        inline void SetValid(bool boolIsValid) {m_boolIsValid = boolIsValid;};
        inline int GetTag() const {return m_iTag;};
        inline void SetTag(int iTag) {m_iTag = iTag;};
    private:
        double m_dblX;
        double m_dblY;
        int m_iSlice;
        int m_iTag;
        bool m_boolIsValid;
    };
    
    CPreprocMark m_Marks[4];
    int m_iCurrentSlice;
    bool m_boolUpdate;
    bool m_boolHasData;
    
    CPreprocMark* MarkFromTag(int iTag);
    CPreprocMark* GetFirstFreeMark();
    void ClearAllMarks();
    void PushCurrentMarks();
    
};

class CMarkCellBodyItem : public CReconItem
{
public:
    CMarkCellBodyItem(nraWeakPointer<Reconstruction>& Recon);
    virtual ~CMarkCellBodyItem() {};
    virtual void AddMark(int iMarkType, double dblX, double dblY);
    virtual void DestroyMark(int iMarkTag);
    virtual void MoveMark(int iMarkTag, double dblX, double dblY);
	virtual void SetLabel(int iMarkTag, nraSharedArray<char> Label){};
    virtual void SetDisplayType(int iDisplayType) {};
    virtual void SelectSlice(int iSliceNmbr);
    virtual void Visible();
    virtual void Invisible() {};
    virtual void Execute(nraSharedPointer<Options>& RecOptions);
    virtual void Update(bool boolValid);
    virtual bool HasCube() 
    {
	NRA_ASSERT(m_Reconstruction.IsValid(), "Invalid reconstruction pointer in CMarkCellBodyItem!"); 
	return m_Reconstruction->GetState() > 6;
    };

private:
    double m_dblX;
    double m_dblY;
    int m_iZ;
    bool m_boolMarkIsSet;
    int m_iCurrentSlice;
    bool m_boolUpdate;
    bool m_boolHasData;
    
};

class CDetectTermItem : public CReconItem
{
public:
    CDetectTermItem(nraWeakPointer<Reconstruction>& Recon);
    virtual ~CDetectTermItem();
    virtual void AddMark(int iMarkType, double dblX, double dblY);
    virtual void DestroyMark(int iMarkTag);
    virtual void MoveMark(int iMarkTag, double dblX, double dblY){};
	virtual void SetLabel(int iMarkTag, nraSharedArray<char> Label){};
    virtual void SetDisplayType(int iDisplayType);
    virtual void SelectSlice(int iSliceNmbr);
    virtual void Visible();
    virtual void Invisible() {};
    virtual void Execute(nraSharedPointer<Options>& RecOptions);
    virtual void Update(bool boolValid);
    virtual bool HasCube()
    {
	NRA_ASSERT(m_Reconstruction.IsValid(), "Invalid reconstruction pointer in CDetectTermItem!"); 
	return m_Reconstruction->GetState() > 7;
    };

private:
    void PushMarks();
    void ClearList();
    list< nraSharedPointer<Mark> > m_MarkList;
    int m_iCurrentSlice;
    bool m_boolUpdate;
    bool m_boolHasData;
    
};

class CFinalItem : public CReconItem
{
public:
    CFinalItem(nraWeakPointer<Reconstruction>& Recon);
    virtual ~CFinalItem() {};
    virtual void AddMark(int iMarkType, double dblX, double dblY)  {};
    virtual void DestroyMark(int iMarkTag) {};
    virtual void MoveMark(int iMarkTag, double dblX, double dblY) {};
	virtual void SetLabel(int iMarkTag, nraSharedArray<char> Label) {};
    virtual void SetDisplayType(int iDisplayType) {};
    virtual void SelectSlice(int iSliceNmbr);
    virtual void Visible();
    virtual void Invisible() {};
    virtual void Execute(nraSharedPointer<Options>& RecOptions){};
    virtual void Update(bool boolValid);
    virtual bool HasCube()
    {
	NRA_ASSERT(m_Reconstruction.IsValid(), "Invalid reconstruction pointer in CFinalItem!"); 
	return m_Reconstruction->GetState() > 8;
    };

private:
    int m_iCurrentSlice;
    bool m_boolUpdate;
    bool m_boolHasData;
    
};

#endif


