/*
 *  QModel.h
 *  QTViewer02
 *
 *  Created by Alexander Heusel on Fri Mar 28 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _NRAAPPLICATIONMODEL_H_
#define _NRAAPPLICATIONMODEL_H_

#include "../nraConfig.h"
#include "../nraAssert.h"

#include "../nraSmartPointer.h"

#include <qobject.h>
#include <iostream>
#include <map.h>
#include "../reconstructionlib/reconstruction.h"

class Reconstruction;
class CReconItem;
class CUIMessage;
class PreferenceDialog;

class nraApplicationModel : public QObject
{
Q_OBJECT
public:
    nraApplicationModel();
    ~nraApplicationModel();
    
    // Callback function for progress display
    static int CallbackProgress(int percentage);

public slots:
    // Slots that will be connected to the controller.
    void OnAddMark(int iMarkType, double dblX, double dblY);
    void OnDestroyMark(int iMarkTag);
    void OnMoveMark(int iMarkTag, double dblX, double dblY);
    void OnSetLabel(int iMarkTag, nraSharedArray<char> Label);
    void OnSetSlice(int iSliceNmbr);
    void OnSetDisplayType(int iDisplayType);
    void OnSetPrefs();

    
    
    // Test
    void OnExecute(nraSharedPointer<Options>& RecOptions);
    void OnSave(char* cFileName, FILETYPE FileType);
    void OnTabFlip(int iTab);

signals:
    // Signals that will be connected to the controller.
    void AddMark(int iMarkTag, int iMarkType, bool boolLabel, double dblX, double dblY);
    void DestroyMark(int iMarkTag);
    void MoveMark(int iMarkTag, double dblX, double dblY);
    void SetLabel(int iMarkTag, nraSharedArray<char> Label);
    void SetSlice(int iSliceNmbr);
    void SetSlider(int iSliderMin, int iSliderMax, int iSliderVal);
    void SetStack(nraSharedPointer<ViewCube> Stack);
    void Disable();
    void Progress(int iPercentage);

private:
    static nraApplicationModel* m_pSingleton;
    void CallbackPipe(int iPercentage);
    void MessageDispatcher(nraWeakPointer<CReconItem> ReconItem);
    nraSharedPointer<Reconstruction> m_Reconstruction;
    std::map<int, nraSharedPointer<CReconItem> > m_ItemMap;
    nraWeakPointer<CReconItem> m_CurrReconItem;
    bool m_boolSemaphore;
    int m_iTab;
    int m_iSlice;
    nraSharedPointer<PreferenceDialog> m_PrefDialog;

};

#endif


