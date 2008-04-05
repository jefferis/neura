/****************************************************************************
** Form interface generated from reading ui file 'datacubemain.ui'
**
** Created: Tue May 6 15:19:24 2003
**      by: The User Interface Compiler ($Id: nraMainWindow.h,v 1.3 2004/07/14 11:17:13 philip Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef _NRAMAINWINDOW_H_
#define _NRAMAINWINDOW_H_

#include <map.h>
#include "qvariant.h"
#include <qcolor.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qimage.h>
#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qslider.h>
#include <qstatusbar.h>
#include <q3popupmenu.h>
#include <qmenubar.h>
#include <qfiledialog.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qvariant.h>
#include <qslider.h>
#include <qtabwidget.h>
#include <qtoolbutton.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <q3popupmenu.h>
#include <qtoolbar.h>
#include <qfiledialog.h>
#include <qdir.h>
#include <q3cstring.h>
#include <qstring.h>
#include <q3canvas.h>
#include <qprogressbar.h>
#include <q3hbox.h>

#include <qvariant.h>
#include <qmainwindow.h>
#include "../reconstructionlib/reconstruction.h"
#include "../nraSmartPointer.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QSlider;
class QTabWidget;
class QToolButton;
class QWidget;
class nraCubeView;
class QProgressBar; 
class QHBox;

class nraMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    nraMainWindow( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~nraMainWindow();

private slots:

    // Slots for the datacube view
    void OnAddMark_View(int iMarkType, double dblX, double dblY);
    void OnDestroyMark_View(int iMarkTag);
    void OnMoveMark_View(int iMarkTag, double dblX, double dblY);
    void OnSetLabel_View(int iMarkTag, nraSharedArray<char> Label);
    
    // Slot for the tab
    void OnTabFlip(QWidget* pTabWidget);

    // Slots for buttons etc.
    void OnLoadButtonClicked();
    void OnPreprocButtonClicked();
    void OnTrafoButtonClicked();
    void OnFFilterButtonClicked();
    void OnFSegmButtonClicked();
    void OnSFilterButtonClicked();
    void OnSSegmButtonClicked();
    void OnTreeGenButtonClicked();
    void OnDetectTermButtonClicked();
    void OnSaveButtonClicked();
    void OnPrefButtonClicked();

    // Slots for options.
    void OnLoadTestToggled(bool boolValue);
    void OnLoadMulFilesToggled(bool boolValue);
    void OnLoadExtrVolToggled(bool boolValue);
    void OnCellBodySaveHOCClicked();
    void OnCellBodySaveDXClicked();        
    
    // Slot for the slider.
    void OnSliderValueChanged(int iValue);
    
    // Slot for the displaytype combobox.
    void OnSetDisplayType(int iDisplayType);

public slots:
    // Slots for the view.
    void OnAddMark(int iMarkTag, int iMarkType, bool boolLabel, double dblX, double dblY);
    void OnDestroyMark(int iMarkTag);
    void OnMoveMark(int iMarkTag, double dblX, double dblY);
    void OnSetLabel(int iMarkTag, nraSharedArray<char> Label);
    void OnSetSlider(int iSliderMin, int iSliderMax, int iSliderVal);
    void OnSetSlice(int iSliceNmbr);
    void OnSetStack(nraSharedPointer<ViewCube> Stack);
    void OnDisable();
    void OnProgress(int iPercentage);


signals:
    // Signals that go to the model layer.
    void AddMark(int iMarkType, double dblX, double dblY);
    void DestroyMark(int iMarkTag);
    void MoveMark(int iMarkTag, double dblX, double dblY);
	void SetLabel(int iMarkTag, nraSharedArray<char> Label);
    void SetSlice(int iSlice);
    void Execute(nraSharedPointer<Options>& RecOptions);
    void Save(char* cFileName, FILETYPE FileType);
    void TabFlip(int iTab);
    void SetDisplayType(int iDisplayType);
    void SetPrefs();
    
    // Signals that go to the view.
    void AddMark_View(int iMarkTag, int iMarkType, bool boolLabel, double dblX, double dblY);
    void DestroyMark_View(int iMarkTag);
    void MoveMark_View(int iMarkTag, double dblX, double dblY);
	void SetLabel_View(int iMarkTag, nraSharedArray<char> Label);
    void SetSlice_View(int iSliceNmbr);
    void SetStack_View(nraSharedPointer<ViewCube> Stack);
    void Disable_View();

private:

    QProgressBar* progressBar;
    QHBox* hboxStatus;
    QLabel* labelStatus;
    QLabel* labelSlider;
    QSlider* sliderSlice;
    QPushButton* pushSave;
    QPushButton* pushPref;
    QTabWidget* tabViews;
    
    // Loading
    QWidget* tabLoading;
    QLabel* labelLoadChannel;
    QGroupBox* groupLoadBehaviour;
    QLabel* labelLoadBehStart;
    QLabel* labelLoadBehEnd;
    QLineEdit* lineLoadBehStart;
    QLineEdit* lineLoadBehEnd;
    QCheckBox* checkLoadMultipleFiles;
    nraCubeView* viewLoading;
    QCanvas* canvLoading;
    QGroupBox* groupLoadVolume;
    QLabel* labelLoadVolStartX;
    QLabel* labelLoadVolStartY;
    QLabel* labelLoadVolStartZ;
    QLabel* labelLoadVolSizeY;
    QLabel* labelLoadVolSizeX;
    QLabel* labelLoadVolSizeZ;
    QLabel* labelLoadVolSize;
    QLineEdit* lineLoadVolSizeY;
    QLineEdit* lineLoadVolSizeZ;
    QLineEdit* lineLoadVolStartZ;
    QLabel* labelLoadVolStart;
    QLineEdit* lineLoadVolStartX;
    QLineEdit* lineLoadVolSizeX;
    QLineEdit* lineLoadVolStartY;
    QCheckBox* checkLoadExtractVol;
    QPushButton* pushLoadLoad;
    QLineEdit* lineLoadChannel;
    QRadioButton* radioLoadTest;
    QLabel* labelLoadHelpMsg;
    QLabel* labelLoadMarkType;
    QComboBox* boxLoadMarkType;
    
    // Preprocessing.
    QWidget* tabPreproc;
    QPushButton* pushPreprocPreproc;
    QCheckBox* checkPreprocDelBound;
    QCheckBox* checkPreprocErasePipet;
    QRadioButton* radioSkipPreProc;
    nraCubeView* viewPreproc;
    QCanvas* canvPreproc;
    QLabel* labelPreprocHelpMsg;
        
    // Tranformation.
    QWidget* tabTrafo;
    QPushButton* pushTrafoTrafo;
    QLabel* labelTrafoThreshLow;
    QLineEdit* lineTrafoThreshLow;
    QLabel* labelTrafoThreshHigh;
    QLineEdit* lineTrafoThreshHigh;
    nraCubeView* viewTrafo;
    QCanvas* canvTrafo;
    QRadioButton* radioSkipTrafo;
    QLabel* labelTrafoHelpMsg;
    
    // First filter.
    QPushButton* pushFFilterFFilter;
    QWidget* tabFFilter;
    QGroupBox* groupFFilterDiscr;
    QLabel* labelFFilterNonlinType;
    QComboBox* boxFFilterIntPoint;
    QLabel* labelFFilterAnisoCoeff2;
    QLabel* labelFFilterLambda;
    QLabel* labelFFilterIntSize;
    QLabel* labelFFilterIntPoint;
    QLineEdit* lineFFilterAnisoCoeff3;
    QComboBox* boxFFilterNonlinType;
    QLabel* labelFFilterAnisoCoeff1;
    QLineEdit* lineFFilterAnisoCoeff2;
    QComboBox* boxFFilterKindOfMom;
    QLabel* labelFFilterKindOfMom;
    QLineEdit* lineFFilterAnisoCoeff1;
    QLabel* labelFFilterAnisoCoeff3;
    QLabel* labelFFilterDiffusion;
    QLineEdit* lineFFilterLambda;
    QComboBox* boxFFilterDiffusion;
    QLineEdit* lineFFilterIntSize;
    QLabel* labelFFilterAnisoCoeff;
    QGroupBox* groupFFilterSolver;
    QLineEdit* lineFFilterLevels;
    QLabel* labelFFilterTSteps;
    QLabel* labelFFilterLevels;
    QLineEdit* lineFFilterTau;
    QLabel* labelFFilterTau;
    QLabel* labelFFilterEpsilon;
    QLineEdit* lineFFilterEpsilon;
    QLineEdit* lineFFilterTSteps;
    QLabel* labelFFilterSolverType;
    QComboBox* boxFFilterSolverType;
    QLineEdit* lineFFilterFilterLevel;
    QLabel* labelFFilterFilterLevel;
    nraCubeView* viewFFilter;
    QCanvas* canvFFilter;
    QRadioButton* radioSkipFFilter;
    QLabel* labelFFilterHelpMsg;
    
    // First segmentation
    QPushButton* pushFSegmFSegm;
    QWidget* tabFSegm;
    QLineEdit* lineFSegmAbsHighThresh;
    QLabel* labelFSegmAbsHighThresh;
    QComboBox* boxFSegmSegmType;
    QLabel* labelFSegmSegmType;
    QLabel* labelFSegmEpsilon;
    QLineEdit* lineFSegmEpsilon;
    QLabel* labelFSegmAbsLowThresh;
    QLineEdit* lineFSegmAbsLowThresh;
    nraCubeView* viewFSegm;
    QCanvas* canvFSegm;
    QRadioButton* radioSkipFSegm;
    QLabel* labelFSegmHelpMsg;    
    
    // Second filter
    QPushButton* pushSFilterSFilter;
    QWidget* tabSFilter;
    QGroupBox* groupSFilterSolver;
    QLineEdit* lineSFilterLevels;
    QLabel* labelSFilterTSteps;
    QLabel* labelSFilterLevels;
    QLineEdit* lineSFilterTau;
    QLabel* labelSFilterTau;
    QLabel* labelSFilterEpsilon;
    QLineEdit* lineSFilterEpsilon;
    QLineEdit* lineSFilterTSteps;
    QLabel* labelSFilterSolverType;
    QComboBox* boxSFilterSolverType;
    QGroupBox* groupSFilterDiscr;
    QLabel* labelSFilterNonlinType;
    QComboBox* boxSFilterIntPoint;
    QLabel* labelSFilterAnisoCoeff2;
    QLabel* labelSFilterLambda;
    QLabel* labelSFilterIntSize;
    QLabel* labelSFilterIntPoint;
    QLineEdit* lineSFilterAnisoCoeff3;
    QComboBox* boxSFilterNonlinType;
    QLabel* labelSFilterAnisoCoeff1;
    QLineEdit* lineSFilterAnisoCoeff2;
    QComboBox* boxSFilterKindOfMom;
    QLabel* labelSFilterKindOfMom;
    QLineEdit* lineSFilterAnisoCoeff1;
    QLabel* labelSFilterAnisoCoeff3;
    QLabel* labelSFilterDiffusion;
    QLineEdit* lineSFilterLambda;
    QComboBox* boxSFilterDiffusion;
    QLineEdit* lineSFilterIntSize;
    QLabel* labelSFilterAnisoCoeff;
    QLineEdit* lineSFilterFilterLevel;
    QLabel* labelSFilterFilterLevel;
    nraCubeView* viewSFilter;
    QCanvas* canvSFilter;
    QRadioButton* radioSkipSFilter;
    QLabel* labelSFilterHelpMsg;
    
    // Second segmentation
    QPushButton* pushSSegmSSegm;
    QWidget* tabSSegm;
    QLabel* labelSSegmSegmType;
    QComboBox* boxSSegmSegmType;
    QLabel* labelSSegmEpsilon;
    QLineEdit* lineSSegmAbsHighThresh;
    QLabel* labelSSegmAbsHighThresh;
    QLabel* labelSSegmAbsLowThresh;
    QLineEdit* lineSSegmAbsLowThresh;
    QLineEdit* lineSSegmEpsilon;
    nraCubeView* viewSSegm;
    QCanvas* canvSSegm;
    QRadioButton* radioSkipSSegm;
    QLabel* labelSSegmHelpMsg;
    
    // Postprocessing
    /*
    QPushButton* pushPostprocPostproc;
    QWidget* tabPostproc; 
    QDatacubeViewer* viewPostproc;
    QRadioButton* radioSkipPostproc;
    */
    
    // Cell body
    QPushButton* pushCellBodyCellBody;
    QWidget* tabCellBody;
    //QLabel* labelCellBodyMaxLevel;
    QLabel* labelCellBodyThreshold;
    QLabel* labelCellBodyDXFilename;
    QLabel* labelCellBodyDXFilenameDisplay;
    QToolButton* toolButtonCellBodyHOC;
    QLabel* labelCellBodyHOCFilename;
    QLabel* labelCellBodyHOCFilenameDisplay;
    QLabel* labelCellBodyThresholdLevel;
    QToolButton* toolButtonCellBodyDX;
    QLineEdit* lineCellBodyThreshold;
    QLineEdit* lineCellBodyThresholdLevel;
    nraCubeView* viewCellBody;
    QCanvas* canvCellBody;
    QCheckBox* checkCellBodyWriteHOC;
    QCheckBox* checkCellBodyWriteDX;
    //QLineEdit* lineCellBodyMaxLevel;
    //QCheckBox* checkCellBodyCreateRecon;
    QRadioButton* radioSkipCellBody;
    QLabel* labelCellBodyHelpMsg;

    // Detect terminations
    QPushButton* pushDetectTermDetectTerm;
    QLabel* labelDetectTermDisplayType;
    QComboBox* boxDetectTermDisplayType;
    QWidget* tabDetectTerm;
    nraCubeView* viewDetectTerm;
    QCanvas* canvDetectTerm;
    QRadioButton* radioSkipDetectTerm;
    QLabel* labelDetectTermHelpMsg;

    // Final
    QPushButton* pushFinalFinal;
    QWidget* tabFinal;
    nraCubeView* viewFinal;
    QCanvas* canvFinal;
    QRadioButton* radioSkipFinal;
    QLabel* labelFinalHelpMsg;

    nraCubeView* m_pCurrentView;
    void ConnectView(nraCubeView* pViewer);
    void DisconnectView(nraCubeView* pViewer);

protected slots:
    virtual void languageChange();
};

#endif 
