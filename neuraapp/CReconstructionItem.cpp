/*
 *  CReconstructionItem.cpp
 *  QTViewer03
 *
 *  Created by Alexander Heusel on Tue May 20 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream.h>
#include <math.h>
using namespace std;
#include <qlineedit.h>
#include "nraDlgTIFFAlert.h"
#include <string.h>
#include "CReconstructionItem.h"

CReconItem::CReconItem(nraWeakPointer<Reconstruction>& Recon)
{
    m_eCubeType = ORIGINCUBE;
    m_Reconstruction = Recon;    
}
    
void CReconItem::UpdateStack()
{
	NRA_ASSERT(m_Reconstruction.IsValid(), "Trying to access invalid pointer in [CReconItem::UpdateStack]!");
    // Get displaycube and store it.
    nraSharedPointer<ViewCube> Buff(m_Reconstruction->CreateViewCube(m_eCubeType));
    if(Buff.IsValid())
    {
    	m_Stack = Buff;
	}
	else
	{
		m_Stack = nraSharedPointer<ViewCube>();
	}
}

void CReconItem::SaveCube(char* cFileName, FILETYPE FileType)
{

    NRA_ASSERT(m_Reconstruction.IsValid(), "Invalid reconstruction pointer in [CReconItem::SaveCube]!");
    bool boolHasCube = false;

    switch(m_eCubeType)
    {
    case ORIGIN:
	if(m_Reconstruction->GetState() > 0)
	{
	    boolHasCube = true;
	}
	break;
    case TRANSFORMED:
	if(m_Reconstruction->GetState() > 1)
	{
	    boolHasCube = true;
	}
	break;
    case FILTERED:
	if(m_Reconstruction->GetState() > 2)
	{
	    boolHasCube = true;
	}
	break;
    case SEGMENTED:
	if(m_Reconstruction->GetState() > 3)
	{
	    boolHasCube = true;
	}
	break;
    case FILTEREDSEG:
	if(m_Reconstruction->GetState() > 4)
	{
	    boolHasCube = true;
	}
	break;
    case FINAL:
	if(m_Reconstruction->GetState() > 5)
	{
	    boolHasCube = true;
	}
	break;
    case PREPROCESSED:
	if(m_Reconstruction->GetState() > 6)
	{
	    boolHasCube = true;
	}
	break;
    case COMPPLOT:
	if(m_Reconstruction->GetState() > 7)
	{
	    boolHasCube = true;
	}
	break;
    case RECPLOT:
	if(m_Reconstruction->GetState() > 8)
	{
	    boolHasCube = true;
	}
	break;
    }
    NRA_ASSERT(boolHasCube, "Trying to save nonexisting cube!");
    int iRes = m_Reconstruction->SaveCube(m_eCubeType, cFileName, FileType);
    NRA_ENSURE(iRes == 0, "[Reconstruction::SaveCube] failed to save existing cube!");
}

void CReconItem::SetPrefs(int iBrightness, int iContrast)
{
	m_Reconstruction->SetBrightness(iBrightness);
	m_Reconstruction->SetContrast(iContrast);
}

///////////////////////////////////////////////////////////
// Loading
///////////////////////////////////////////////////////////
CLoadingItem::CLoadingItem(nraWeakPointer<Reconstruction>& Recon) : CReconItem(Recon)
{
    SetCubeType(ORIGINCUBE);
    m_iCurrentSlice = 0;
}


void CLoadingItem::SelectSlice(int iSliceNmbr)
{    
    m_iCurrentSlice = iSliceNmbr;
    nraSharedPointer<CUIMessage> UIMessage(new CUISetSlice(iSliceNmbr));
    MessageQueue.push(UIMessage);
    PushMarks();    
}

void CLoadingItem::AddMark(int iMarkType, double dblX, double dblY)
{
    int iX = static_cast<int>(static_cast<double>(m_Stack->GetSizeX())*dblX);
    int iY = static_cast<int>(static_cast<double>(m_Stack->GetSizeY())*dblY);
    int iZ = m_iCurrentSlice - 1;
    
    Vector<int> Position(3);
    Position[1] = iX;
    Position[2] = iY;
    Position[3] = iZ; 
    
    nraSharedPointer<Mark> NewMark(new Mark(Position, inaktive, 0));
    NewMark->SetType((typesOfMarks) iMarkType);
    m_Reconstruction->AddOrientationMark(*NewMark);
      
    if(NewMark->GetIdOfTermination() > -1)
    {
        // Create copy of mark label
        char strbuff[LABELLENGTH];
        NewMark->GetMarkLabel(strbuff);
        long strlength = strlen(strbuff) + 1;
        nraSharedArray<char> Label(new char[strlength], strlength);
        memcpy(Label.GetPointer(), strbuff, strlength);
    
        // Push messages to queue.
        nraSharedPointer<CUIMessage> UIMessage(new CUIAddMark(NewMark->GetIdOfTermination(), iMarkType, false, dblX, dblY));
        MessageQueue.push(UIMessage);
        UIMessage = nraSharedPointer<CUIMessage>(new CUISetLabel(NewMark->GetIdOfTermination(), Label));
        MessageQueue.push(UIMessage);
        m_MarkList.push_back(NewMark);
    }

}

void CLoadingItem::DestroyMark(int iMarkTag)
{
    list< nraSharedPointer<Mark> >::iterator it;
    it = m_MarkList.begin();
    while(it != m_MarkList.end())
    {
        if((*it)->GetIdOfTermination() == iMarkTag)
        {
            m_MarkList.erase(it);
            break;
        }
        it++;
    }
    m_Reconstruction->RemoveOrientationMark(iMarkTag);
    nraSharedPointer<CUIMessage> UIMessage(new CUIDestroyMark(iMarkTag));
    MessageQueue.push(UIMessage);
}

void CLoadingItem::MoveMark(int iMarkTag, double dblX, double dblY)
{

    list< nraSharedPointer<Mark> >::iterator it;
    it = m_MarkList.begin();
    while(it != m_MarkList.end())
    {
        if((*it)->GetIdOfTermination() == iMarkTag)
        {
            Vector<int> Position(3);
            Position = (*it)->GetPosition();
            Position[1] = static_cast<int>(static_cast<double>(m_Stack->GetSizeX())*dblX);
            Position[2] = static_cast<int>(static_cast<double>(m_Stack->GetSizeY())*dblY);
            (*it)->SetPosition(Position);
            
            nraSharedPointer<CUIMessage> UIMessage(new CUIMoveMark(iMarkTag, dblX, dblY));
            MessageQueue.push(UIMessage);            
            break;
        }
        it++;
    }
    
}


void CLoadingItem::Visible()
{
    if(IsValid())
    {
        nraSharedPointer<CUIMessage> UIMessage(new CUISetSlider(1, m_Stack->GetSizeZ(), m_iCurrentSlice));
        MessageQueue.push(UIMessage);
    }
    else
    {
        nraSharedPointer<CUIMessage> UIMessage(new CUIDisable);
        MessageQueue.push(UIMessage);
    }
}

void CLoadingItem::Execute(nraSharedPointer<Options>& RecOptions)
{
    // Check for the correct options.
    NRA_REQUIRE(RecOptions.IsValid(), "Empty RecOptions pointer received in [CLoadingItem::Execute]!");
    LoadingOptions* pLoadingOptions = dynamic_cast<LoadingOptions*>(RecOptions.GetPointer());
    NRA_REQUIRE(pLoadingOptions, "Wrong option type passed in [CLoadingItem::Execute]!");
    if(pLoadingOptions)
    {
        int ret = m_Reconstruction->SetLoadingOptions(*pLoadingOptions);
        NRA_ASSERT(ret == 0, "[Reconstruction::SetLoadingOptions] failed in [CLoadingItem::Execute]!");
        ret = m_Reconstruction->Load();
        NRA_ASSERT(ret != 1, "[Reconstruction::Load] failed in [CLoadingItem::Execute]!");
        if(ret == NO_SPACEINGS_TAGS && ret != 1)
        {
        	nraDlgTIFFAlert TIFFDialog;
        	double dblGridX = 0.0;
        	double dblGridY = 0.0;
        	double dblGridZ = 0.0;
			bool boolNoSpacingsTags; 
			boolNoSpacingsTags = true;
        	while(boolNoSpacingsTags)
        	{
        		TIFFDialog.exec();
		   		dblGridX = TIFFDialog.lineGridX->text().toDouble();
        		dblGridY = TIFFDialog.lineGridY->text().toDouble();
        		dblGridZ = TIFFDialog.lineGridZ->text().toDouble();
        		if(dblGridX > 0.0 && dblGridY > 0.0 && dblGridZ > 0.0)
        		{
        			boolNoSpacingsTags = false;	
        		}
        	}
			Vector<double> spaceing(3);
			spaceing[1]=dblGridX;
			spaceing[2]=dblGridY;
			spaceing[3]=dblGridZ; 
        	m_Reconstruction->SetSpaceings(spaceing);
        }
        
        UpdateStack();
        if(IsValid())
        {
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);
            
            // Tell the next CReconItem that there is data to proceed on.
            NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CLoadingItem::Execute]!");
	    	m_FollowUp->Update(true);
        }
    }
}

void CLoadingItem::PushMarks()
{
    int iMarkTag;
    int iMarkType;
    double dblX;
    double dblY;
    double dblSizeX = static_cast<double>(m_Stack->GetSizeX());
    double dblSizeY = static_cast<double>(m_Stack->GetSizeY());
    Vector<int> Position(3);
    
    list< nraSharedPointer<Mark> >::iterator it;
    it = m_MarkList.begin();
    
    while(it != m_MarkList.end())
    {
        Position = (*it)->GetPosition();
        iMarkTag = (*it)->GetIdOfTermination();
        iMarkType = (*it)->GetType();
        if((Position[3] + 1) == m_iCurrentSlice) // Slices are 1 based coordinates are zero based.
        {
            // Push mark
            dblX = static_cast<double>(Position[1]) / dblSizeX;
            dblY = static_cast<double>(Position[2]) / dblSizeY;
                        
            nraSharedPointer<CUIMessage> UIMessage(new CUIAddMark(iMarkTag, iMarkType, false, dblX, dblY));
            MessageQueue.push(UIMessage);
            
            // Create copy of mark label
            char strbuff[LABELLENGTH];
            (*it)->GetMarkLabel(strbuff);
            long strlength = strlen(strbuff) + 1;
            nraSharedArray<char> Label(new char[strlength], strlength);
            memcpy(Label.GetPointer(), strbuff, strlength);

            // Push label to message queue
            UIMessage = nraSharedPointer<CUIMessage>(new CUISetLabel(iMarkTag, Label));
            MessageQueue.push(UIMessage);

        }
        it++;
    }

}

void CLoadingItem::ClearList()
{
    m_MarkList.clear();
}


///////////////////////////////////////////////////////////
// Transformation
///////////////////////////////////////////////////////////


CTransformItem::CTransformItem(nraWeakPointer<Reconstruction>& Recon) : CReconItem(Recon)
{
    SetCubeType(ORIGINCUBE);
    m_boolHasData = false;
    m_boolUpdate = false;
    m_iCurrentSlice = 0;
}

void CTransformItem::Visible()
{
    if(m_boolUpdate)
    {
        if(m_boolHasData) // There is new data in Reconstruction.
        {
            m_iCurrentSlice = 1;
            UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);
        }
        else // Data in Reconstruction is invalid
        {
            nraSharedPointer<CUIMessage> UIMessage(new CUIDisable());
            MessageQueue.push(UIMessage);            
        }
        m_boolUpdate = false;
    }
    else
    {
    	if(m_boolHasData)
    	{
    		UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);    		
    		if(m_iCurrentSlice > m_Stack->GetSizeZ())
    		{
    			m_iCurrentSlice = 1;
    		}
    		UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlice(m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        	UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlider(1, m_Stack->GetSizeZ(), m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        }
    }

}

void CTransformItem::SelectSlice(int iSliceNmbr)
{
    m_iCurrentSlice = iSliceNmbr;
    nraSharedPointer<CUIMessage> UIMessage(new CUISetSlice(iSliceNmbr));
    MessageQueue.push(UIMessage);
}

void CTransformItem::Execute(nraSharedPointer<Options>& RecOptions)
{
    // Check for the correct options.
    NRA_REQUIRE(RecOptions.IsValid(), "Empty RecOptions pointer received in [CTransformItem::Execute]!");
    TransformOptions* pTransformOptions = dynamic_cast<TransformOptions*>(RecOptions.GetPointer());
    NRA_REQUIRE(pTransformOptions, "Wrong option type passed in [CTransformItem::Execute]!");
    if(pTransformOptions)
    {
        int ret = m_Reconstruction->SetTransformOptions(*pTransformOptions);
        NRA_ASSERT(ret == 0, "[Reconstruction::SetTransformOptions] failed in [CTransformItem::Execute]!");
        ret = m_Reconstruction->TransformData();
        NRA_ASSERT(ret == 0, "[Reconstruction::TransformData] failed in [CTransformItem::Execute]!");
        if(ret == 0)
        {            
            // Tell the next CReconItem that there is data to proceed on.
	    	NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CTransformItem::Execute]!");
            m_FollowUp->Update(true);
        }
    }

}

void CTransformItem::Update(bool boolValid)
{
    m_boolUpdate = true;
    m_boolHasData = boolValid;
    NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CTransformItem::Update]!");
    m_FollowUp->Update(false);
}

///////////////////////////////////////////////////////////
// First filter
///////////////////////////////////////////////////////////
CFFilterItem::CFFilterItem(nraWeakPointer<Reconstruction>& Recon) : CReconItem(Recon)
{
    SetCubeType(TRANSFORMED);
    m_boolHasData = false;
    m_boolUpdate = false;
    m_iCurrentSlice = 0;
}

void CFFilterItem::Visible()
{
    if(m_boolUpdate)
    {
        if(m_boolHasData) // There is new data in Reconstruction.
        {
            m_iCurrentSlice = 1;
            UpdateStack();
	    	nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);
        }
        else // Data in Reconstruction is invalid
        {
            nraSharedPointer<CUIMessage> UIMessage(new CUIDisable());
            MessageQueue.push(UIMessage);            
        }
        m_boolUpdate = false;
    }
    else
    {
    	if(m_boolHasData)
    	{
    		UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);    		
    		if(m_iCurrentSlice > m_Stack->GetSizeZ())
    		{
    			m_iCurrentSlice = 1;
    		}
    		UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlice(m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        	UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlider(1, m_Stack->GetSizeZ(), m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        }
    }

}

void CFFilterItem::SelectSlice(int iSliceNmbr)
{
    m_iCurrentSlice = iSliceNmbr;
    nraSharedPointer<CUIMessage> UIMessage(new CUISetSlice(iSliceNmbr));
    MessageQueue.push(UIMessage);
}

void CFFilterItem::Execute(nraSharedPointer<Options>& RecOptions)
{
    // Check for the correct options.
    NRA_REQUIRE(RecOptions.IsValid(), "Empty RecOptions pointer received in [CFFilterItem::Execute]!");
    RecFilterOptions* pFFilterOptions = dynamic_cast<RecFilterOptions*>(RecOptions.GetPointer());
    NRA_REQUIRE(pFFilterOptions, "Wrong option type passed in [CFFilterItem::Execute]!");
    if(pFFilterOptions)
    {
        int ret = m_Reconstruction->SetFirstFilterOptions(*pFFilterOptions);
        NRA_ASSERT(ret == 0, "[Reconstruction::SetFirstFilterOptions] failed in [CFFilterItem::Execute]!");
        ret = m_Reconstruction->FirstFilter();
        NRA_ASSERT(ret == 0, "[Reconstruction::FirstFilter] failed in [CFFilterItem::Execute]!");
        if(ret == 0)
        {            
            // Tell the next CReconItem that there is data to proceed on.
	    NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CFFilterItem::Execute]!");
            m_FollowUp->Update(true);
        }
    }

}

void CFFilterItem::Update(bool boolValid)
{
    m_boolUpdate = true;
    m_boolHasData = boolValid;
    NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CFFilterItem::Update]!");
    m_FollowUp->Update(false);
}

///////////////////////////////////////////////////////////
// First segmentation
///////////////////////////////////////////////////////////
CFSegmItem::CFSegmItem(nraWeakPointer<Reconstruction>& Recon) : CReconItem(Recon)
{
    SetCubeType(FILTERED);
    m_boolHasData = false;
    m_boolUpdate = false;
    m_iCurrentSlice = 0;
}

void CFSegmItem::Visible()
{
    if(m_boolUpdate)
    {
        if(m_boolHasData) // There is new data in Reconstruction.
        {
            m_iCurrentSlice = 1;
            UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);
        }
        else // Data in Reconstruction is invalid
        {
            nraSharedPointer<CUIMessage> UIMessage(new CUIDisable());
            MessageQueue.push(UIMessage);            
        }
        m_boolUpdate = false;
    }
    else
    {
    	if(m_boolHasData)
    	{
    		UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);    		
    		if(m_iCurrentSlice > m_Stack->GetSizeZ())
    		{
    			m_iCurrentSlice = 1;
    		}
    		UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlice(m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        	UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlider(1, m_Stack->GetSizeZ(), m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        }
    }

}

void CFSegmItem::SelectSlice(int iSliceNmbr)
{
    m_iCurrentSlice = iSliceNmbr;
    nraSharedPointer<CUIMessage> UIMessage(new CUISetSlice(iSliceNmbr));
    MessageQueue.push(UIMessage);
}

void CFSegmItem::Execute(nraSharedPointer<Options>& RecOptions)
{
    // Check for the correct options.
    NRA_REQUIRE(RecOptions.IsValid(), "Empty RecOptions pointer received in [CFSegmItem::Execute]!");
    SegmentationOptions* pFSegmOptions = dynamic_cast<SegmentationOptions*>(RecOptions.GetPointer());
    NRA_REQUIRE(pFSegmOptions, "Wrong option type passed in [CFSegmItem::Execute]!");
    if(pFSegmOptions)
    {
        int ret = m_Reconstruction->SetFirstSegmentationOptions(*pFSegmOptions);
        NRA_ASSERT(ret == 0, "[Reconstruction::SetFirstSegmentationOptions] failed in [CFSegmItem::Execute]!");
        ret = m_Reconstruction->FirstSegmentation();
        NRA_ASSERT(ret == 0, "[Reconstruction::FirstSegmentation] failed in [CFSegmItem::Execute]!");
        if(ret == 0)
        {            
            // Tell the next CReconItem that there is data to proceed on.
	    	NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CFSegmItem::Execute]!");
            m_FollowUp->Update(true);
        }
    }

}

void CFSegmItem::Update(bool boolValid)
{
    m_boolUpdate = true;
    m_boolHasData = boolValid;
    NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CFSegmItem::Update]!");
    m_FollowUp->Update(false);
}


///////////////////////////////////////////////////////////
// Second filter
///////////////////////////////////////////////////////////
CSFilterItem::CSFilterItem(nraWeakPointer<Reconstruction>& Recon) : CReconItem(Recon)
{
    SetCubeType(SEGMENTED);
    m_boolHasData = false;
    m_boolUpdate = false;
    m_iCurrentSlice = 0;
}

void CSFilterItem::Visible()
{
    if(m_boolUpdate)
    {
        if(m_boolHasData) // There is new data in Reconstruction.
        {
            m_iCurrentSlice = 1;
            UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);
        }
        else // Data in Reconstruction is invalid
        {
            nraSharedPointer<CUIMessage> UIMessage(new CUIDisable());
            MessageQueue.push(UIMessage);            
        }
        m_boolUpdate = false;
    }
    else
    {
    	if(m_boolHasData)
    	{
    		UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);    		
    		if(m_iCurrentSlice > m_Stack->GetSizeZ())
    		{
    			m_iCurrentSlice = 1;
    		}
    		UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlice(m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        	UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlider(1, m_Stack->GetSizeZ(), m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        }
    }

}

void CSFilterItem::SelectSlice(int iSliceNmbr)
{
    m_iCurrentSlice = iSliceNmbr;
    nraSharedPointer<CUIMessage> UIMessage(new CUISetSlice(iSliceNmbr));
    MessageQueue.push(UIMessage);
}

void CSFilterItem::Execute(nraSharedPointer<Options>& RecOptions)
{
    // Check for the correct options.
    NRA_REQUIRE(RecOptions.IsValid(), "Empty RecOptions pointer received in [CSFilterItem::Execute]!");
    RecFilterOptions* pSFilterOptions = dynamic_cast<RecFilterOptions*>(RecOptions.GetPointer());
    NRA_REQUIRE(pSFilterOptions, "Wrong option type passed in [CSFilterItem::Execute]!");
    if(pSFilterOptions)
    {
        int ret = m_Reconstruction->SetSecondFilterOptions(*pSFilterOptions);
        NRA_ASSERT(ret == 0, "[Reconstruction::SetSecondFilterOptions] failed in [CSFilterItem::Execute]!");
        ret = m_Reconstruction->SecondFilter();
        NRA_ASSERT(ret == 0, "[Reconstruction::SecondFilter] failed in [CSFilterItem::Execute]!");
        if(ret == 0)
        {            
            // Tell the next CReconItem that there is data to proceed on.
	    NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CSFilterItem::Execute]!");
            m_FollowUp->Update(true);
        }
    }

}

void CSFilterItem::Update(bool boolValid)
{
    m_boolUpdate = true;
    m_boolHasData = boolValid;
    NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CSFilterItem::Update]!");
    m_FollowUp->Update(false);
}


///////////////////////////////////////////////////////////
// Second segmentation
///////////////////////////////////////////////////////////
CSSegmItem::CSSegmItem(nraWeakPointer<Reconstruction>& Recon) : CReconItem(Recon)
{
    SetCubeType(FILTEREDSEG);
    m_boolHasData = false;
    m_boolUpdate = false;
    m_iCurrentSlice = 0;
}

void CSSegmItem::Visible()
{
    if(m_boolUpdate)
    {
        if(m_boolHasData) // There is new data in Reconstruction.
        {
            m_iCurrentSlice = 1;
            UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);
        }
        else // Data in Reconstruction is invalid
        {
            nraSharedPointer<CUIMessage> UIMessage(new CUIDisable());
            MessageQueue.push(UIMessage);            
        }
        m_boolUpdate = false;
    }
    else
    {
    	if(m_boolHasData)
    	{
     		UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);    		
    		if(m_iCurrentSlice > m_Stack->GetSizeZ())
    		{
    			m_iCurrentSlice = 1;
    		}
    		UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlice(m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        	UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlider(1, m_Stack->GetSizeZ(), m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        }
    }

}

void CSSegmItem::SelectSlice(int iSliceNmbr)
{
    m_iCurrentSlice = iSliceNmbr;
    nraSharedPointer<CUIMessage> UIMessage(new CUISetSlice(iSliceNmbr));
    MessageQueue.push(UIMessage);
}

void CSSegmItem::Execute(nraSharedPointer<Options>& RecOptions)
{
    // Check for the correct options.
    NRA_REQUIRE(RecOptions.IsValid(), "Empty RecOptions pointer received in [CSSegmItem::Execute]!");
    SegmentationOptions* pSSegmOptions = dynamic_cast<SegmentationOptions*>(RecOptions.GetPointer());
    NRA_REQUIRE(pSSegmOptions, "Wrong option type passed in [CSSegmItem::Execute]!");
    
    int ret = m_Reconstruction->SetSecondSegmentationOptions(*pSSegmOptions);
    NRA_ASSERT(ret == 0, "[Reconstruction::SetSecondSegmentationOptions] failed in [CSSegmItem::Execute]!");
    ret = m_Reconstruction->SecondSegmentation();
    NRA_ASSERT(ret == 0, "[Reconstruction::SecondSegmentation] failed in [CSSegmItem::Execute]!");
  
    // Tell the next CReconItem that there is data to proceed on.
    NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CSSegmItem::Execute]!");
    m_FollowUp->Update(true);

}

void CSSegmItem::Update(bool boolValid)
{
    m_boolUpdate = true;
    m_boolHasData = boolValid;
    NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CSSegmItem::Update]!");
    m_FollowUp->Update(false);
}


///////////////////////////////////////////////////////////
// Preprocessing
///////////////////////////////////////////////////////////
CPreprocItem::CPreprocItem(nraWeakPointer<Reconstruction>& Recon) : CReconItem(Recon)
{
    SetCubeType(FINAL);
    m_Marks[0].SetTag(1);
    m_Marks[1].SetTag(2);
    m_Marks[2].SetTag(3);
    m_Marks[3].SetTag(4);
    m_boolUpdate = false;
    m_boolHasData = false;
    m_iCurrentSlice = 0;

}

CPreprocItem::~CPreprocItem()
{
}

void CPreprocItem::AddMark(int iMarkType, double dblX, double dblY)
{
    // Create coordinates for second mark.
    double dblX2;
    double dblY2;
    if(dblX < 0.85)
        dblX2 = dblX + 0.15;
    else
        dblX2 = dblX - 0.15;
    dblY2 = dblY;
        
    // Set both marks.
    CPreprocMark* pMark = GetFirstFreeMark();
    if(pMark)
    {
        pMark->SetX(dblX);
        pMark->SetY(dblY);
        pMark->SetSlice(m_iCurrentSlice); 
        pMark->SetValid(true);
        nraSharedPointer<CUIMessage> UIMessage(new CUIAddMark(pMark->GetTag(), 8, false, dblX, dblY));
        MessageQueue.push(UIMessage);
    }
    pMark = GetFirstFreeMark();
    if(pMark)
    {
        pMark->SetX(dblX2);
        pMark->SetY(dblY2);
        pMark->SetSlice(m_iCurrentSlice); 
        pMark->SetValid(true);
        nraSharedPointer<CUIMessage> UIMessage(new CUIAddMark(pMark->GetTag(), 8, false, dblX2, dblY2));
        MessageQueue.push(UIMessage);
    }
    
}

void CPreprocItem::DestroyMark(int iMarkTag)
{
    int iTag1;
    int iTag2;
    if(iMarkTag < 3)
    {
        iTag1 = 1;
        iTag2 = 2;
    }
    else
    {
        iTag1 = 3;
        iTag2 = 4;    
    }
    CPreprocMark* pMark = MarkFromTag(iTag1);
    if(pMark)
    {
        pMark->SetValid(false);
        nraSharedPointer<CUIMessage> UIMessage(new CUIDestroyMark(iTag1));
        MessageQueue.push(UIMessage);    
    }
    pMark = MarkFromTag(iTag2);
    if(pMark)
    {
        pMark->SetValid(false);
        nraSharedPointer<CUIMessage> UIMessage(new CUIDestroyMark(iTag2));
        MessageQueue.push(UIMessage);    
    }
    
    
}

void CPreprocItem::MoveMark(int iMarkTag, double dblX, double dblY)
{
    CPreprocMark* pMark = MarkFromTag(iMarkTag);
    if(pMark)
    {
        if(pMark->IsValid())
        {
            pMark->SetX(dblX);
            pMark->SetY(dblY);
            nraSharedPointer<CUIMessage> UIMessage(new CUIMoveMark(iMarkTag, dblX, dblY));
            MessageQueue.push(UIMessage);    
        }
    }
    
}


void CPreprocItem::SelectSlice(int iSliceNmbr)
{
    m_iCurrentSlice = iSliceNmbr;
    nraSharedPointer<CUIMessage> UIMessage(new CUISetSlice(iSliceNmbr));
    MessageQueue.push(UIMessage);
    PushCurrentMarks();
}

void CPreprocItem::Visible()
{

    if(m_boolUpdate)
    {
        if(m_boolHasData) // There is new data in Reconstruction.
        {
            m_iCurrentSlice = 1;
            UpdateStack();
            ClearAllMarks();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);
            UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlice(m_iCurrentSlice));
            MessageQueue.push(UIMessage);
        	UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlider(1, m_Stack->GetSizeZ(), m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
            
        }
        else // Data in Reconstruction is invalid
        {
            ClearAllMarks();
            nraSharedPointer<CUIMessage> UIMessage(new CUIDisable());
            MessageQueue.push(UIMessage);            
        }
        m_boolUpdate = false;
    }
    else
    {
    	if(m_boolHasData)
    	{
    		UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);    		
    		if(m_iCurrentSlice > m_Stack->GetSizeZ())
    		{
    			m_iCurrentSlice = 1;
    		}
        	UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlice(m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        	UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlider(1, m_Stack->GetSizeZ(), m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        	PushCurrentMarks();
        }
    }
}

void CPreprocItem::Execute(nraSharedPointer<Options>& RecOptions)
{
    // Ensure that the correct options have been passed
    NRA_REQUIRE(RecOptions.IsValid(), "Empty RecOptions pointer received in [CPreprocItem::Execute]!");
    PreProcessOptions* pPreProcessOptions = dynamic_cast<PreProcessOptions*>(RecOptions.GetPointer());
    NRA_REQUIRE(pPreProcessOptions, "Wrong option type passed in [CPreprocItem::Execute]!");
    
    // Ensure that all marks are set.
    bool boolAllMarksSet = true;
    for(int i = 0; i < 4; i++)
	if(!m_Marks[i].IsValid())
	    boolAllMarksSet = false;
                
    if(boolAllMarksSet)
    {
        
	// Create work variables.
	double dblWork;
	double dblRadiusBig;
	double dblRadiusSmall;
	double dblXBig;
	double dblYBig;
	double dblZBig;
	double dblXSmall;
	double dblYSmall;
	double dblZSmall;
	int iSize = m_Stack->GetSizeX(); // FIXME Get image extents. Currently assumes square image geometry!
            
	// Calc radii.
	dblWork = (m_Marks[0].GetX() - m_Marks[1].GetX());
	dblWork *= dblWork;
	dblRadiusBig = (m_Marks[0].GetY() - m_Marks[1].GetY());
	dblRadiusBig *= dblRadiusBig;
	dblRadiusBig = dblRadiusBig + dblWork;
	dblRadiusBig = sqrt(dblRadiusBig) / 2;
	
	dblWork = (m_Marks[2].GetX() - m_Marks[3].GetX());
	dblWork *= dblWork;
	dblRadiusSmall = (m_Marks[2].GetY() - m_Marks[3].GetY());
	dblRadiusSmall *= dblRadiusSmall;
	dblRadiusSmall = dblRadiusSmall + dblWork;
	dblRadiusSmall = sqrt(dblRadiusSmall) / 2;
            
	// Determine center coordinates.
	dblXBig = ((m_Marks[0].GetX() - m_Marks[1].GetX()) / 2) + m_Marks[1].GetX();
	dblYBig = ((m_Marks[0].GetY() - m_Marks[1].GetY()) / 2) + m_Marks[1].GetY();
	dblZBig = static_cast<double>(m_Marks[0].GetSlice() - 1); // position is zero based slices are 1 based
	dblXSmall = ((m_Marks[2].GetX() - m_Marks[3].GetX()) / 2) + m_Marks[3].GetX();
	dblYSmall = ((m_Marks[2].GetY() - m_Marks[3].GetY()) / 2) + m_Marks[3].GetY();
	dblZSmall = static_cast<double>(m_Marks[2].GetSlice() - 1); // position is zero based slices are 1 based
            
	// Scale all coordinates from normalized to pixel coordinates.
	dblRadiusBig = dblRadiusBig * static_cast<double>(iSize);
	dblXBig = dblXBig * static_cast<double>(iSize);
	dblYBig = dblYBig * static_cast<double>(iSize);
	dblRadiusSmall = dblRadiusSmall * static_cast<double>(iSize);
	dblXSmall = dblXSmall * static_cast<double>(iSize);
	dblYSmall = dblYSmall * static_cast<double>(iSize);            
            
	// Flip small and big if neccessary.
	if(dblRadiusBig < dblRadiusSmall)
	{
	    dblWork = dblRadiusBig; dblRadiusBig = dblRadiusSmall; dblRadiusSmall = dblWork;
	    dblWork = dblXBig; dblXBig = dblXSmall; dblXSmall = dblWork;
	    dblWork = dblYBig; dblYBig = dblYSmall; dblYSmall = dblWork;
	    dblWork = dblZBig; dblZBig = dblZSmall; dblZSmall = dblWork;
	}
            
	// Pass coordinates and radii to option class and 'press play on tape'.
	(*pPreProcessOptions).radiusSmall = dblRadiusSmall;
	(*pPreProcessOptions).radiusBig = dblRadiusBig;
	(*pPreProcessOptions).pointSmall[1] = static_cast<int>(dblXSmall);
	(*pPreProcessOptions).pointSmall[2] = static_cast<int>(dblYSmall);
	(*pPreProcessOptions).pointSmall[3] = static_cast<int>(dblZSmall);
	(*pPreProcessOptions).pointBig[1] = static_cast<int>(dblXBig);
	(*pPreProcessOptions).pointBig[2] = static_cast<int>(dblYBig);
	(*pPreProcessOptions).pointBig[3] = static_cast<int>(dblZBig);
                        
	int iRes = m_Reconstruction->SetPreProcessOptions(*pPreProcessOptions); // DON'T delete options here!
	NRA_ASSERT(iRes == 0, "[Reconstruction::SetPreProcessOptions] failed in [CPreprocItem::Execute]!");
	iRes = m_Reconstruction->PreProcess();
	NRA_ASSERT(iRes == 0, "[Reconstruction::PreProcess] failed in [CPreprocItem::Execute]!");
            
	// Tell the next CReconItem that there is data to proceed on.
	NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CPreprocItem::Execute]!");
	m_FollowUp->Update(true);
            
    }
    else
    {
	if((*pPreProcessOptions).skip == YES)
	{
	    int iRes = m_Reconstruction->SetPreProcessOptions(*pPreProcessOptions); // DON'T delete options here!
	    NRA_ASSERT(iRes == 0, "[Reconstruction::SetPreProcessOptions] failed in [CPreprocItem::Execute]!");
	    iRes = m_Reconstruction->PreProcess();
	    NRA_ASSERT(iRes == 0, "[Reconstruction::PreProcess] failed in [CPreprocItem::Execute]!");
            
	    // Tell the next CReconItem that there is data to proceed on.
	    NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CPreprocItem::Execute]!");
	    m_FollowUp->Update(true);
	}
    }
}

void CPreprocItem::Update(bool boolValid)
{
    m_boolUpdate = true;
    m_boolHasData = boolValid;
    NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CPreprocItem::Update]!");
    m_FollowUp->Update(false);
    
}

CPreprocItem::CPreprocMark* CPreprocItem::MarkFromTag(int iTag)
{
    for(int i = 0; i < 4; i++)
        if(m_Marks[i].GetTag() == iTag)
            return &m_Marks[i];
    return 0;
}

CPreprocItem::CPreprocMark* CPreprocItem::GetFirstFreeMark()
{
    for(int i = 0; i < 4; i++)
        if(!m_Marks[i].IsValid())
            return &m_Marks[i];
    return 0;
}

void CPreprocItem::PushCurrentMarks()
{
    for(int i = 0; i < 4; i++)
    {
        if(m_Marks[i].IsValid() && (m_Marks[i].GetSlice() == m_iCurrentSlice))
        {
            nraSharedPointer<CUIMessage> UIMessage(new CUIAddMark(m_Marks[i].GetTag(), 8, false, m_Marks[i].GetX(), m_Marks[i].GetY()));
            MessageQueue.push(UIMessage);
        }
    }
}

void CPreprocItem::ClearAllMarks()
{
    for(int i = 0; i < 4; i++)
        m_Marks[i].SetValid(false);
}

///////////////////////////////////////////////////////////
// MarkCellBody
///////////////////////////////////////////////////////////
CMarkCellBodyItem::CMarkCellBodyItem(nraWeakPointer<Reconstruction>& Recon) : CReconItem(Recon)
{
    SetCubeType(PREPROCESSED);
    m_boolHasData = false;
    m_boolUpdate = false;
    m_iCurrentSlice = 0;
}

void CMarkCellBodyItem::AddMark(int iMarkType, double dblX, double dblY)
{
    if(m_boolMarkIsSet)
    {
        if((m_iZ + 1) != m_iCurrentSlice) // position is zero based slices are 1 based
        {
            m_iZ = m_iCurrentSlice - 1; // dto.
            m_dblX = dblX;
            m_dblY = dblY;
            nraSharedPointer<CUIMessage> UIMessage(new CUIAddMark(1, 4, false, dblX, dblY));
            MessageQueue.push(UIMessage);
        }
    }
    else
    {
        m_iZ = m_iCurrentSlice - 1; // dto.
        m_dblX = dblX;
        m_dblY = dblY;
        m_boolMarkIsSet = true;
        nraSharedPointer<CUIMessage> UIMessage(new CUIAddMark(1, 4, false, dblX, dblY));
        MessageQueue.push(UIMessage);
    }
}

void CMarkCellBodyItem::DestroyMark(int iMarkTag)
{
    m_boolMarkIsSet = false;
    nraSharedPointer<CUIMessage> UIMessage(new CUIDestroyMark(iMarkTag));
    MessageQueue.push(UIMessage);        
}

void CMarkCellBodyItem::MoveMark(int iMarkTag, double dblX, double dblY)
{
    m_dblX = dblX;
    m_dblY = dblY;
    nraSharedPointer<CUIMessage> UIMessage(new CUIMoveMark(iMarkTag, dblX, dblY));
    MessageQueue.push(UIMessage);        
}



void CMarkCellBodyItem::Visible()
{
    if(m_boolUpdate)
    {
        m_boolMarkIsSet = false;
        if(m_boolHasData) // There is new data in Reconstruction.
        {
            m_iCurrentSlice = 1;
            UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);
        }
        else // Data in Reconstruction is invalid
        {
            nraSharedPointer<CUIMessage> UIMessage(new CUIDisable());
            MessageQueue.push(UIMessage);            
        }
        m_boolUpdate = false;
    }
    else
    {
    	if(m_boolHasData)
    	{
    		UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);    		
    		if(m_iCurrentSlice > m_Stack->GetSizeZ())
    		{
    			m_iCurrentSlice = 1;
    		}
    		UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlice(m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        	UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlider(1, m_Stack->GetSizeZ(), m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        	if(m_boolMarkIsSet && m_iCurrentSlice == (m_iZ + 1)) // position is zero based slices are 1 based
        	{
            	UIMessage = nraSharedPointer<CUIMessage>(new CUIAddMark(1, 4, false, m_dblX, m_dblY));
            	MessageQueue.push(UIMessage);    
        	}
        }
    }

}

void CMarkCellBodyItem::SelectSlice(int iSliceNmbr)
{
    m_iCurrentSlice = iSliceNmbr;
    nraSharedPointer<CUIMessage> UIMessage(new CUISetSlice(iSliceNmbr));
    MessageQueue.push(UIMessage);
    if(m_boolMarkIsSet && m_iCurrentSlice == (m_iZ + 1)) // position is zero based slices are 1 based
    {
        UIMessage = nraSharedPointer<CUIMessage>(new CUIAddMark(1, 4, false, m_dblX, m_dblY));
        MessageQueue.push(UIMessage);    
    }
    
}

void CMarkCellBodyItem::Execute(nraSharedPointer<Options>& RecOptions)
{
    // Check for the correct options.
    NRA_REQUIRE(RecOptions.IsValid(), "Empty RecOptions pointer received in [CMarkCellBodyItem::Execute]!");
    TreeGenerationOptions* pTreeGenOptions = dynamic_cast<TreeGenerationOptions*>(RecOptions.GetPointer());
    NRA_REQUIRE(pTreeGenOptions, "Wrong option type passed in [CMarkCellBodyItem::Execute]!");

    // Check if we have a valid mark or that the skip option is set.
    if(m_boolMarkIsSet && pTreeGenOptions->skip == NO)
    {
	int iSize = m_Stack->GetSizeX(); // FIXME Get image extents. Currently assumes square image geometry!
	(*pTreeGenOptions).body[1] = static_cast<int>(static_cast<double>(iSize)*m_dblX);
	(*pTreeGenOptions).body[2] = static_cast<int>(static_cast<double>(iSize)*m_dblY);
	(*pTreeGenOptions).body[3] = m_iZ;
	
	int ret = m_Reconstruction->SetTreeGenerationOptions(*pTreeGenOptions);
	NRA_ASSERT(ret == 0, "[Reconstruction::SetTreeGenerationOptions] failed in [CMarkCellBodyItem::Execute]!");
	ret = m_Reconstruction->TreeGenerationDetectTerminations();
	NRA_ASSERT(ret == 0, "[Reconstruction::TreeGenerationDetectTerminations] failed in [CMarkCellBodyItem::Execute]!");

          
	// Tell the next CReconItem that there is data to proceed on.
	NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CMarkCellBodyItem::Execute]!");
	m_FollowUp->Update(true);
    }
}

void CMarkCellBodyItem::Update(bool boolValid)
{
    m_boolUpdate = true;
    m_boolHasData = boolValid;
    NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CMarkCellBodyItem::Update]!");
    m_FollowUp->Update(false);
}

///////////////////////////////////////////////////////////
// Detect terminations
///////////////////////////////////////////////////////////
CDetectTermItem::CDetectTermItem(nraWeakPointer<Reconstruction>& Recon) : CReconItem(Recon)
{
    SetCubeType(COMPPLOT);
    m_boolHasData = false;
    m_boolUpdate = false;
    m_iCurrentSlice = 0;
}

CDetectTermItem::~CDetectTermItem()
{
    if(m_MarkList.size() > 0)
    {
        ClearList();
    }
}

void CDetectTermItem::AddMark(int iMarkType, double dblX, double dblY)
{
    int iZ = m_Stack->GetSizeZ(); 
    
    int iX = static_cast<int>(static_cast<double>(iZ)*dblX);
    int iY = static_cast<int>(static_cast<double>(iZ)*dblY);
    iZ = m_iCurrentSlice;
    Vector<int> Position(3);
    Position[1] = iX;
    Position[2] = iY;
    Position[3] = (iZ - 1); // Slices are 1 based positions are zero based
    nraSharedPointer<Mark> NewMark(new Mark(Position, inaktive, 0));
    m_Reconstruction->AddMark(*NewMark);
    
    if(NewMark->GetIdOfTermination() > -1)
    {
        nraSharedPointer<CUIMessage> UIMessage(new CUIAddMark(NewMark->GetIdOfTermination(), 5, false, dblX, dblY));
        MessageQueue.push(UIMessage);
        m_MarkList.push_back(NewMark);
    }
}

void CDetectTermItem::DestroyMark(int iMarkTag)
{
    list< nraSharedPointer<Mark> >::iterator it;
    it = m_MarkList.begin();
    while(it != m_MarkList.end())
    {
        if((*it)->GetIdOfTermination() == iMarkTag)
        {
            m_MarkList.erase(it);
            break;
        }
        it++;
    }
    m_Reconstruction->DeleteMark(iMarkTag);
    nraSharedPointer<CUIMessage> UIMessage(new CUIDestroyMark(iMarkTag));
    MessageQueue.push(UIMessage);
}


void CDetectTermItem::Visible()
{
    if(m_boolUpdate)
    {
        if(m_boolHasData) // There is new data in Reconstruction.
        {
            m_iCurrentSlice = 1;
            UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);
            if(m_MarkList.size() > 0)
            {
                ClearList();
            }
	    
	    // Copy all marks in internal buffer list.
            list<Mark*>* pList = m_Reconstruction->GetMarkList();
	    list<Mark*>::iterator it;
	    it = pList->begin();
	    while(it != pList->end())
	    {
		m_MarkList.push_back(nraSharedPointer<Mark>(*it));
		it++;
	    }
	    delete pList;
	    
            PushMarks();
        }
        else // Data in Reconstruction is invalid
        {
            nraSharedPointer<CUIMessage> UIMessage(new CUIDisable());
            MessageQueue.push(UIMessage);            
        }
    }
    else
    {
    	if(m_boolHasData)
    	{
    		UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);    		
    		if(m_iCurrentSlice > m_Stack->GetSizeZ())
    		{
    			m_iCurrentSlice = 1;
    		}
    		UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlice(m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        	UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlider(1, m_Stack->GetSizeZ(), m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
    	}
    }

}

void CDetectTermItem::SetDisplayType(int iDisplayType)
{
    cout << "From: " << GetCubeType() << endl;
    cout << "To: " << iDisplayType << endl;
    switch(iDisplayType)
    {
        case 0:
            SetCubeType( ORIGINCUBE );
            break;
        case 1:
            SetCubeType( TRANSFORMED );
            break;
        case 2:
            SetCubeType( FILTERED );
            break;
        case 3:
            SetCubeType( SEGMENTED );
            break;
        case 4:
            SetCubeType( FILTEREDSEG );
            break;
        case 5:
            SetCubeType( FINAL );
            break;
        case 6:
            SetCubeType( PREPROCESSED );
            break;
        case 7:
            SetCubeType( COMPPLOT );
            break;
        case 8:
            SetCubeType( RECPLOT );
            break;
        case 9:
            SetCubeType( PREPROCMAXINT5 );
            break;
        case 10:
            SetCubeType( PREPROCMAXINT10 );
            break;
        case 11:
            SetCubeType( ORIGINMAXINT5 );
            break;
        case 12:
            SetCubeType( ORIGINMAXINT10 );
            break;
         case 13:
            SetCubeType( COMPMAXINT5 );
            break;
        case 14:
            SetCubeType( COMPMAXINT10 );
            break;       
    }
 
    UpdateStack();
    nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
    MessageQueue.push(UIMessage);
    UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlice(m_iCurrentSlice));
    MessageQueue.push(UIMessage);
    PushMarks();  

    
}

void CDetectTermItem::SelectSlice(int iSliceNmbr)
{
    m_iCurrentSlice = iSliceNmbr;
    nraSharedPointer<CUIMessage> UIMessage(new CUISetSlice(iSliceNmbr));
    MessageQueue.push(UIMessage);
    PushMarks();    
}

void CDetectTermItem::Execute(nraSharedPointer<Options>& RecOptions)
{
    int iRes = m_Reconstruction->TreeGenerationCreateNervecell();
    NRA_ASSERT(iRes == 0, "[Reconstruction::TreeGenerationCreateNervecell] failed in [CDetectTermItem::Execute]!");
    
    NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CDetectTermItem::Execute]!");
    m_FollowUp->Update(true);
    
}

void CDetectTermItem::Update(bool boolValid)
{
    m_boolUpdate = true;
    m_boolHasData = boolValid;
    NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CDetectTermItem::Update]!");
    m_FollowUp->Update(false);
}

void CDetectTermItem::PushMarks()
{
    int iMarkTag;
    double dblX;
    double dblY;
    double dblSize = static_cast<double>(m_Stack->GetSizeX());
    Vector<int> Position(3);
    
    list< nraSharedPointer<Mark> >::iterator it;
    it = m_MarkList.begin();
    
    while(it != m_MarkList.end())
    {
        Position = (*it)->GetPosition();
        iMarkTag = (*it)->GetIdOfTermination();
        if((Position[3] + 1) == m_iCurrentSlice) // Slices are 1 based coordinates are zero based.
        {
            dblX = static_cast<double>(Position[1]) / dblSize;
            dblY = static_cast<double>(Position[2]) / dblSize;
            nraSharedPointer<CUIMessage> UIMessage(new CUIAddMark(iMarkTag, 3, false, dblX, dblY));
            MessageQueue.push(UIMessage);
        }
        it++;
    }

}

void CDetectTermItem::ClearList()
{
    m_MarkList.clear();
}

///////////////////////////////////////////////////////////
// Final
///////////////////////////////////////////////////////////
CFinalItem::CFinalItem(nraWeakPointer<Reconstruction>& Recon) : CReconItem(Recon)
{
    SetCubeType(RECPLOT);
    m_boolHasData = false;
    m_boolUpdate = false;
    m_iCurrentSlice = 0;
}

void CFinalItem::SelectSlice(int iSliceNmbr)
{
    m_iCurrentSlice = iSliceNmbr;
    nraSharedPointer<CUIMessage> UIMessage(new CUISetSlice(iSliceNmbr));
    MessageQueue.push(UIMessage);
}

void CFinalItem::Visible()
{
    if(m_boolUpdate)
    {
        if(m_boolHasData) // There is new data in Reconstruction.
        {
            m_iCurrentSlice = 1;
            UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);
        }
        else // Data in Reconstruction is invalid
        {
            nraSharedPointer<CUIMessage> UIMessage(new CUIDisable());
            MessageQueue.push(UIMessage);            
        }
        m_boolUpdate = false;
    }
    else
    {
    	if(m_boolHasData)
    	{
    		UpdateStack();
            nraSharedPointer<CUIMessage> UIMessage(new CUISetStack(GetStack()));
            MessageQueue.push(UIMessage);    		
    		if(m_iCurrentSlice > m_Stack->GetSizeZ())
    		{
    			m_iCurrentSlice = 1;
    		}
    		UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlice(m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        	UIMessage = nraSharedPointer<CUIMessage>(new CUISetSlider(1, m_Stack->GetSizeZ(), m_iCurrentSlice));
        	MessageQueue.push(UIMessage);
        }
    }

}

void CFinalItem::Update(bool boolValid)
{
    m_boolUpdate = true;
    m_boolHasData = boolValid;
    NRA_ASSERT(m_FollowUp.IsValid(), "No followup in [CFinalItem::Update]!");
    m_FollowUp->Update(false);
}

