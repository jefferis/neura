/*
 *  QMain.h
 *  QTViewer02
 *
 *  Created by Alexander Heusel on Fri Mar 28 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */

#include <qapplication.h>
#include "nraMainWindow.h"
#include "nraApplicationModel.h"
#include <iostream.h>
#include "../nraSmartPointer.h"


class nraApplication : public QApplication
{
Q_OBJECT
public:
    nraApplication(int& argc, char** argv) : QApplication(argc, argv)
    {
        m_pMainWindow = new nraMainWindow();
        m_pApplicationModel = new nraApplicationModel();
        
        // Connecting mainwindow and model.
        connect(m_pApplicationModel,	SIGNAL	(AddMark(int, int, bool, double, double)),
                m_pMainWindow,			SLOT	(OnAddMark(int, int, bool, double, double)));
        connect(m_pApplicationModel,	SIGNAL	(DestroyMark(int)),
                m_pMainWindow,			SLOT	(OnDestroyMark(int)));
        connect(m_pApplicationModel,	SIGNAL	(MoveMark(int, double, double)),
                m_pMainWindow,			SLOT	(OnMoveMark(int, double, double)));
        connect(m_pApplicationModel,	SIGNAL	(SetLabel(int, nraSharedArray<char>)),
                m_pMainWindow,			SLOT	(OnSetLabel(int, nraSharedArray<char>)));
        connect(m_pApplicationModel,	SIGNAL	(SetSlider(int, int, int)),
                m_pMainWindow,			SLOT	(OnSetSlider(int, int, int)));
        connect(m_pApplicationModel,	SIGNAL	(SetSlice(int)),
                m_pMainWindow,			SLOT	(OnSetSlice(int)));
        connect(m_pApplicationModel,	SIGNAL	(SetStack(nraSharedPointer<ViewCube>)),
                m_pMainWindow,			SLOT	(OnSetStack(nraSharedPointer<ViewCube>)));
        connect(m_pApplicationModel,	SIGNAL	(Disable()),
                m_pMainWindow,			SLOT	(OnDisable()));
        connect(m_pApplicationModel,	SIGNAL	(Progress(int)),
                m_pMainWindow,			SLOT	(OnProgress(int)));
                        
        connect(m_pMainWindow, 			SIGNAL	(AddMark(int, double, double)),
                m_pApplicationModel, 	SLOT	(OnAddMark(int, double, double)));
        connect(m_pMainWindow, 			SIGNAL	(DestroyMark(int)), 
                m_pApplicationModel, 	SLOT	(OnDestroyMark(int)));
        connect(m_pMainWindow, 			SIGNAL	(MoveMark(int, double, double)), 
                m_pApplicationModel, 	SLOT	(OnMoveMark(int, double, double)));
        connect(m_pMainWindow, 			SIGNAL	(SetLabel(int, nraSharedArray<char>)), 
                m_pApplicationModel, 	SLOT	(OnSetLabel(int, nraSharedArray<char>)));
        connect(m_pMainWindow, 			SIGNAL	(Execute(nraSharedPointer<Options>&)), 
                m_pApplicationModel, 	SLOT	(OnExecute(nraSharedPointer<Options>&)));
        connect(m_pMainWindow, 			SIGNAL	(TabFlip(int)), 
                m_pApplicationModel, 	SLOT	(OnTabFlip(int)));
        connect(m_pMainWindow,			SIGNAL	(SetSlice(int)),
                m_pApplicationModel,	SLOT	(OnSetSlice(int)));
        connect(m_pMainWindow,			SIGNAL	(Save(char*, FILETYPE)),
                m_pApplicationModel,	SLOT	(OnSave(char*, FILETYPE)));
        connect(m_pMainWindow,			SIGNAL	(SetPrefs(void)),
                m_pApplicationModel,	SLOT	(OnSetPrefs(void)));
        connect(m_pMainWindow,			SIGNAL	(SetDisplayType(int)),
                m_pApplicationModel,	SLOT	(OnSetDisplayType(int)));
                
		
        // ...
        setMainWidget(m_pMainWindow);
        m_pMainWindow->show();

        
    };
    

private:
    nraMainWindow* m_pMainWindow;
    nraApplicationModel* m_pApplicationModel;
    
    
};
