/*
 *  QModel.cpp
 *  QTViewer02
 *
 *  Created by Alexander Heusel on Fri Mar 28 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */

#include "nraApplicationModel.h"
#include "CReconstructionItem.h"
#include "nraDlgPreferences.h"
#include <qstring.h>
#include <qlabel.h>

nraApplicationModel::nraApplicationModel()
{
    m_iTab = 1; // Intial tab settings.
    m_iSlice = 0; // Initial slice settings.
    m_pSingleton = this;
    m_PrefDialog = nraSharedPointer<PreferenceDialog>(new PreferenceDialog);
    m_Reconstruction = nraSharedPointer<Reconstruction>(new Reconstruction(nraApplicationModel::CallbackProgress));
    nraWeakPointer<Reconstruction> ReconBuff = m_Reconstruction;
    
    // Create the reconstruction items and connect them.
    m_ItemMap[1] = nraSharedPointer<CReconItem>(new CLoadingItem(ReconBuff));
    m_ItemMap[2] = nraSharedPointer<CReconItem>(new CTransformItem(ReconBuff));
    m_ItemMap[3] = nraSharedPointer<CReconItem>(new CFFilterItem(ReconBuff));
    m_ItemMap[4] = nraSharedPointer<CReconItem>(new CFSegmItem(ReconBuff));
    m_ItemMap[5] = nraSharedPointer<CReconItem>(new CSFilterItem(ReconBuff));
    m_ItemMap[6] = nraSharedPointer<CReconItem>(new CSSegmItem(ReconBuff));
    m_ItemMap[7] = nraSharedPointer<CReconItem>(new CPreprocItem(ReconBuff));
    m_ItemMap[8] = nraSharedPointer<CReconItem>(new CMarkCellBodyItem(ReconBuff));
    m_ItemMap[9] = nraSharedPointer<CReconItem>(new CDetectTermItem(ReconBuff));
    m_ItemMap[10] = nraSharedPointer<CReconItem>(new CFinalItem(ReconBuff));
    m_ItemMap[11] = nraSharedPointer<CReconItem>(new CDummyItem(ReconBuff));
    
    for(int i = 1; i < 11; i++)
        m_ItemMap[i]->SetFollowUp(m_ItemMap[i+1]);
    
    m_CurrReconItem = m_ItemMap[1];
    m_boolSemaphore = true;
    
}
nraApplicationModel::~nraApplicationModel()
{

}

// Slots that will be connected to the controller.
void nraApplicationModel::OnAddMark(int iMarkType, double dblX, double dblY)
{   
    m_CurrReconItem->AddMark(iMarkType, dblX, dblY);
    if(m_boolSemaphore)
        MessageDispatcher(m_CurrReconItem);
}

void nraApplicationModel::OnDestroyMark(int iMarkTag)
{
    m_CurrReconItem->DestroyMark(iMarkTag);
    if(m_boolSemaphore)
        MessageDispatcher(m_CurrReconItem);
}

void nraApplicationModel::OnMoveMark(int iMarkTag, double dblX, double dblY)
{
    m_CurrReconItem->MoveMark(iMarkTag, dblX, dblY);
    if(m_boolSemaphore)
        MessageDispatcher(m_CurrReconItem);
}

void nraApplicationModel::OnSetLabel(int iMarkTag, nraSharedArray<char> Label)
{
    m_CurrReconItem->SetLabel(iMarkTag, Label);
    if(m_boolSemaphore)
        MessageDispatcher(m_CurrReconItem);
}

void nraApplicationModel::OnSetSlice(int iSliceNmbr)
{
    m_CurrReconItem->SelectSlice(iSliceNmbr);
    if(m_boolSemaphore)
        MessageDispatcher(m_CurrReconItem);
}

void nraApplicationModel::OnSave(char* cFileName, FILETYPE FileType)
{

    m_CurrReconItem->SaveCube(cFileName, FileType);

}

void nraApplicationModel::OnSetPrefs()
{
	int iBrightness = 0;
	int iContrast = 0;
	m_PrefDialog->exec();
	iBrightness = m_PrefDialog->labelDisplBrightness->text().toInt();
	iContrast = m_PrefDialog->labelDisplContrast->text().toInt();
	
	m_CurrReconItem->SetPrefs(iBrightness, iContrast);
	
	
}

void nraApplicationModel::OnExecute(nraSharedPointer<Options>& RecOptions)
{
    m_CurrReconItem->Execute(RecOptions);
    if(m_boolSemaphore)
        MessageDispatcher(m_CurrReconItem);
}

void nraApplicationModel::OnSetDisplayType(int iDisplayType)
{
    m_ItemMap[9]->SetDisplayType(iDisplayType);
}

void nraApplicationModel::OnTabFlip(int iTab)
{


    if(m_ItemMap[2]->HasCube())
    {	
    
        // Find last valid tab.
        int iIdxPointer = 3;
        
        while(m_ItemMap[iIdxPointer]->HasCube()) 
        {
            iIdxPointer++;
        }
        
        switch(iIdxPointer)
        {
            case 3:
                if(iTab > 2 && iTab < 9)
                {
		    		nraSharedPointer<Options> TrafoOpt(new TransformOptions);
		    		TrafoOpt->skip = YES;
                    m_ItemMap[2]->Execute(TrafoOpt);
                }
            case 4:
                if(iTab > 3 && iTab < 9)
                {
		    		nraSharedPointer<Options> RecFilterOpt(new RecFilterOptions);
                    RecFilterOpt->skip = YES;
		    		m_ItemMap[3]->Execute(RecFilterOpt);
                }
            case 5:
                if(iTab > 4 && iTab < 9)
                {
		    		nraSharedPointer<Options> SegmOpt(new SegmentationOptions);
                    SegmOpt->skip = YES;
		    		m_ItemMap[4]->Execute(SegmOpt);
                }
            case 6:
                if(iTab > 5 && iTab < 9)
                {
		    		nraSharedPointer<Options> RecFilterOpt(new RecFilterOptions);
                    RecFilterOpt->skip = YES;
                    m_ItemMap[5]->Execute(RecFilterOpt);
                }
            case 7:
                if(iTab > 6 && iTab < 9)
                {
		    		nraSharedPointer<Options> SegmOpt(new SegmentationOptions);
                    SegmOpt->skip = YES;
                    m_ItemMap[6]->Execute(SegmOpt);
                }
            case 8:
                if(iTab == 8)
                {
		    		nraSharedPointer<Options> PreProcOpt(new PreProcessOptions);
		    		PreProcOpt->skip = YES;
                    m_ItemMap[7]->Execute(PreProcOpt);
                }
            default:
                break;
        }
    
    }
    
    m_CurrReconItem->Invisible();
    m_CurrReconItem = m_ItemMap[iTab];
    m_CurrReconItem->Visible();
    MessageDispatcher(m_CurrReconItem);
    
}

void nraApplicationModel::MessageDispatcher(nraWeakPointer<CReconItem> ReconItem)
{
    nraSharedPointer<CUIMessage> UIMessage;
    CUIAddMark* 	pAddMark;
    CUIDestroyMark* pDestroyMark;
    CUIMoveMark* 	pMoveMark;
    CUISetLabel* 	pSetLabel;
    CUISetSlice* 	pSetSlice;
    CUISetSlider*	pSetSlider;
    CUISetStack* 	pSetStack;
    CUIDisable*		pDisable;
    
    //queue< nraSharedPointer<CUIMessage> > MessageQueue;
    
    m_boolSemaphore = false;
    while(!(*ReconItem).MessageQueue.empty())
    {
        UIMessage = (*ReconItem).MessageQueue.front();
        (*ReconItem).MessageQueue.pop();
    
        pAddMark = dynamic_cast<CUIAddMark*>(UIMessage.GetPointer());
        if(pAddMark)
        {
            emit AddMark(pAddMark->GetTag(), pAddMark->GetType(), pAddMark->GetAccLabel(), pAddMark->GetX(), pAddMark->GetY());
            continue;
        }
        pDestroyMark = dynamic_cast<CUIDestroyMark*>(UIMessage.GetPointer());
        if(pDestroyMark)
        {
            emit DestroyMark(pDestroyMark->GetTag());
            continue;
        }
        pMoveMark = dynamic_cast<CUIMoveMark*>(UIMessage.GetPointer());
        if(pMoveMark)
        {
            emit MoveMark(pMoveMark->GetTag(), pMoveMark->GetX(), pMoveMark->GetY());
            continue;
        }
        pSetLabel = dynamic_cast<CUISetLabel*>(UIMessage.GetPointer());
        if(pSetLabel)
        {
            emit SetLabel(pSetLabel->GetTag(), pSetLabel->GetLabel());
            continue;
        }
        pSetSlice = dynamic_cast<CUISetSlice*>(UIMessage.GetPointer());
        if(pSetSlice)
        {
            emit SetSlice(pSetSlice->GetSlice());
            continue;
        }
        pSetSlider = dynamic_cast<CUISetSlider*>(UIMessage.GetPointer());
        if(pSetSlider)
        {
            emit SetSlider(pSetSlider->GetMin(),pSetSlider->GetMax(),pSetSlider->GetVal());
	    	continue;
        }
        pSetStack = dynamic_cast<CUISetStack*>(UIMessage.GetPointer());
        if(pSetStack)
        {
            emit SetStack(pSetStack->GetStack());
            continue;
        }
        pDisable = dynamic_cast<CUIDisable*>(UIMessage.GetPointer());
        if(pDisable)
        {
            emit Disable();
            continue;
        }

    }
    m_boolSemaphore = true;
}

void nraApplicationModel::CallbackPipe(int iPercentage)
{
    emit Progress(iPercentage);
}

int nraApplicationModel::CallbackProgress(int percentage)
{
    m_pSingleton->CallbackPipe(percentage);
    return 0;
}

nraApplicationModel* nraApplicationModel::m_pSingleton = 0L;
