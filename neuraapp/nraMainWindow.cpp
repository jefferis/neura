/****************************************************************************
** Form implementation generated from reading ui file 'datacubemain.ui'
**
** Created: Tue May 6 15:19:17 2003
**      by: The User Interface Compiler ($Id: nraMainWindow.cpp,v 1.4 2004/07/14 11:17:13 philip Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "nraMainWindow.h"
#include "nraCubeView.h"

#include <iostream>



/* 
 *  Constructs a DatacubeMain as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 */
nraMainWindow::nraMainWindow( QWidget* parent, const char* name, WFlags fl )
    : QMainWindow( parent, name, fl )
{
    (void)statusBar();
        
    hboxStatus = new QHBox(0L, "hboxStatus");

    progressBar = new QProgressBar(hboxStatus, "progressBar");
    progressBar->setFixedWidth(400);
    progressBar->setTotalSteps(100);

    labelStatus = new QLabel(hboxStatus, "labelStatus");
    labelStatus->setFixedWidth(600);


    statusBar()->addWidget(hboxStatus);
    
    if ( !name )
	setName( "DatacubeMain" );
    setCentralWidget( new QWidget( this, "qt_central_widget" ) );

    labelSlider = new QLabel( centralWidget(), "labelSlider" );
    labelSlider->setGeometry( QRect( 490, 570, 41, 21 ) );

    sliderSlice = new QSlider( centralWidget(), "sliderSlice" );
    sliderSlice->setGeometry( QRect( 20, 570, 450, 20 ) );
    sliderSlice->setOrientation( QSlider::Horizontal );
    sliderSlice->setEnabled(false);
    
    pushSave = new QPushButton( centralWidget(), "pushSave" );
    pushSave->setGeometry( QRect( 540, 570, 100, 30 ) );
    
    pushPref = new QPushButton( centralWidget(), "pushPref" );
    pushPref->setGeometry( QRect( 920, 570, 100, 30 ) );

    tabViews = new QTabWidget( centralWidget(), "tabViews" );
    tabViews->setGeometry( QRect( 10, 0, 1010, 560 ) );

    // Loading
    tabLoading = new QWidget( tabViews, "tabLoading" );

    labelLoadChannel = new QLabel( tabLoading, "labelLoadChannel" );
    labelLoadChannel->setGeometry( QRect( 570, 80, 53, 20 ) );

    groupLoadBehaviour = new QGroupBox( tabLoading, "groupLoadBehaviour" );
    groupLoadBehaviour->setGeometry( QRect( 540, 120, 190, 160 ) );

    labelLoadBehStart = new QLabel( groupLoadBehaviour, "labelLoadBehStart" );
    labelLoadBehStart->setGeometry( QRect( 30, 60, 80, 20 ) );
    labelLoadBehStart->setEnabled(false);
    
    labelLoadBehEnd = new QLabel( groupLoadBehaviour, "labelLoadBehEnd" );
    labelLoadBehEnd->setGeometry( QRect( 30, 110, 73, 20 ) );
    labelLoadBehEnd->setEnabled(false);

    lineLoadBehStart = new QLineEdit( groupLoadBehaviour, "lineLoadBehStart" );
    lineLoadBehStart->setGeometry( QRect( 130, 60, 40, 20 ) );
    lineLoadBehStart->setEnabled(false);

    lineLoadBehEnd = new QLineEdit( groupLoadBehaviour, "lineLoadBehEnd" );
    lineLoadBehEnd->setGeometry( QRect( 130, 110, 40, 20 ) );
    lineLoadBehEnd->setEnabled(false);

    checkLoadMultipleFiles = new QCheckBox( groupLoadBehaviour, "checkLoadMultipleFiles" );
    checkLoadMultipleFiles->setGeometry( QRect( 20, 20, 90, 20 ) );

	canvLoading = new QCanvas(512, 512);
    viewLoading = new nraCubeView(*canvLoading, tabLoading, "viewLoading" );
    viewLoading->setGeometry( QRect( 10, 10, 516, 516 ) );

    groupLoadVolume = new QGroupBox( tabLoading, "groupLoadVolume" );
    groupLoadVolume->setGeometry( QRect( 740, 70, 250, 210 ) );

    labelLoadVolStart = new QLabel( groupLoadVolume, "labelLoadVolStart" );
    labelLoadVolStart->setGeometry( QRect( 30, 60, 31, 16 ) );
    labelLoadVolStart->setEnabled(false);

    labelLoadVolStartX = new QLabel( groupLoadVolume, "labelLoadVolStartX" );
    labelLoadVolStartX->setGeometry( QRect( 30, 90, 16, 21 ) );
    labelLoadVolStartX->setEnabled(false);
    
    labelLoadVolStartY = new QLabel( groupLoadVolume, "labelLoadVolStartY" );
    labelLoadVolStartY->setGeometry( QRect( 100, 90, 16, 21 ) );
    labelLoadVolStartY->setEnabled(false);

    labelLoadVolStartZ = new QLabel( groupLoadVolume, "labelLoadVolStartZ" );
    labelLoadVolStartZ->setGeometry( QRect( 170, 160, 16, 21 ) );
    labelLoadVolStartZ->setEnabled(false);

    lineLoadVolStartX = new QLineEdit( groupLoadVolume, "lineLoadVolStartX" );
    lineLoadVolStartX->setGeometry( QRect( 50, 90, 40, 21 ) );
    lineLoadVolStartX->setEnabled(false);

    lineLoadVolStartY = new QLineEdit( groupLoadVolume, "lineLoadVolStartY" );
    lineLoadVolStartY->setGeometry( QRect( 120, 90, 40, 21 ) );
    lineLoadVolStartY->setEnabled(false);

    lineLoadVolStartZ = new QLineEdit( groupLoadVolume, "lineLoadVolStartZ" );
    lineLoadVolStartZ->setGeometry( QRect( 190, 90, 40, 21 ) );
    lineLoadVolStartZ->setEnabled(false);

    labelLoadVolSize = new QLabel( groupLoadVolume, "labelLoadVolSize" );
    labelLoadVolSize->setGeometry( QRect( 30, 130, 30, 20 ) );
    labelLoadVolSize->setEnabled(false);

    labelLoadVolSizeX = new QLabel( groupLoadVolume, "labelLoadVolSizeX" );
    labelLoadVolSizeX->setGeometry( QRect( 30, 160, 16, 21 ) );
    labelLoadVolSizeX->setEnabled(false);

    labelLoadVolSizeY = new QLabel( groupLoadVolume, "labelLoadVolSizeY" );
    labelLoadVolSizeY->setGeometry( QRect( 100, 160, 16, 21 ) );
    labelLoadVolSizeY->setEnabled(false);
    
    labelLoadVolSizeZ = new QLabel( groupLoadVolume, "labelLoadVolSizeZ" );
    labelLoadVolSizeZ->setGeometry( QRect( 170, 90, 16, 21 ) );
    labelLoadVolSizeZ->setEnabled(false);

    lineLoadVolSizeX = new QLineEdit( groupLoadVolume, "lineLoadVolSizeX" );
    lineLoadVolSizeX->setGeometry( QRect( 50, 160, 40, 21 ) );
    lineLoadVolSizeX->setEnabled(false);

    lineLoadVolSizeY = new QLineEdit( groupLoadVolume, "lineLoadVolSizeY" );
    lineLoadVolSizeY->setGeometry( QRect( 120, 160, 40, 21 ) );
    lineLoadVolSizeY->setEnabled(false);

    lineLoadVolSizeZ = new QLineEdit( groupLoadVolume, "lineLoadVolSizeZ" );
    lineLoadVolSizeZ->setGeometry( QRect( 190, 160, 40, 21 ) );
    lineLoadVolSizeZ->setEnabled(false);


    checkLoadExtractVol = new QCheckBox( groupLoadVolume, "checkLoadExtractVol" );
    checkLoadExtractVol->setGeometry( QRect( 20, 20, 100, 20 ) );

    pushLoadLoad = new QPushButton( tabLoading, "pushLoadLoad" );
    pushLoadLoad->setGeometry( QRect( 540, 490, 100, 30 ) );

    lineLoadChannel = new QLineEdit( tabLoading, "lineLoadChannel" );
    lineLoadChannel->setGeometry( QRect( 680, 80, 40, 20 ) );

    radioLoadTest = new QRadioButton( tabLoading, "radioLoadTest" );
    radioLoadTest->setGeometry( QRect( 950, 490, 60, 30 ) );

    labelLoadHelpMsg = new QLabel( tabLoading, "labelLoadHelpMsg" );
    labelLoadHelpMsg->setGeometry( QRect( 540, 10, 450, 40 ) );
    labelLoadHelpMsg->setBackgroundColor( QColor( 255, 255, 210 ) );

    labelLoadMarkType = new QLabel( tabLoading, "labelLoadMarkType" );
    labelLoadMarkType->setGeometry( QRect( 570, 330, 120, 21 ) );
    labelLoadMarkType->setEnabled(true);

    boxLoadMarkType = new QComboBox( FALSE, tabLoading, "boxLoadMarkType" );
    boxLoadMarkType->setGeometry( QRect( 590, 360, 120, 20 ) );


    tabViews->insertTab( tabLoading, "" );

    // Trafo
    tabTrafo = new QWidget( tabViews, "tabTrafo" );

    pushTrafoTrafo = new QPushButton( tabTrafo, "pushTrafoTrafo" );
    pushTrafoTrafo->setGeometry( QRect( 540, 490, 100, 30 ) );
    
    labelTrafoThreshLow = new QLabel( tabTrafo, "labelTrafoThreshLow" );
    labelTrafoThreshLow->setGeometry( QRect( 580, 160, 84, 16 ) );

    lineTrafoThreshLow = new QLineEdit( tabTrafo, "lineTrafoThreshLow" );
    lineTrafoThreshLow->setGeometry( QRect( 600, 190, 90, 20 ) );

    lineTrafoThreshHigh = new QLineEdit( tabTrafo, "lineTrafoThreshHigh" );
    lineTrafoThreshHigh->setGeometry( QRect( 600, 290, 90, 20 ) );

    labelTrafoThreshHigh = new QLabel( tabTrafo, "labelTrafoThreshHigh" );
    labelTrafoThreshHigh->setGeometry( QRect( 580, 260, 90, 16 ) );

	canvTrafo = new QCanvas(512, 512);
    viewTrafo = new nraCubeView( *canvTrafo, tabTrafo, "viewTrafo" );
    viewTrafo->setGeometry( QRect( 10, 10, 516, 516 ) );

    radioSkipTrafo = new QRadioButton( tabTrafo, "radioSkipTrafo" );
    radioSkipTrafo->setGeometry( QRect( 891, 490, 120, 31 ) );
    
    labelTrafoHelpMsg = new QLabel( tabTrafo, "labelTrafoHelpMsg" );
    labelTrafoHelpMsg->setGeometry( QRect( 540, 10, 450, 40 ) );
    labelTrafoHelpMsg->setBackgroundColor( QColor( 255, 255, 210 ) );
    
    tabViews->insertTab( tabTrafo, "" );

    // First filter
    
    tabFFilter = new QWidget( tabViews, "tabFFilter" );

    pushFFilterFFilter = new QPushButton( tabFFilter, "pushFFilterFFilter" );
    pushFFilterFFilter->setGeometry( QRect( 540, 490, 100, 30 ) );
    
    groupFFilterDiscr = new QGroupBox( tabFFilter, "groupFFilterDiscr" );
    groupFFilterDiscr->setGeometry( QRect( 750, 130, 240, 280 ) );

    labelFFilterNonlinType = new QLabel( groupFFilterDiscr, "labelFFilterNonlinType" );
    labelFFilterNonlinType->setGeometry( QRect( 20, 50, 90, 20 ) );

    boxFFilterIntPoint = new QComboBox( FALSE, groupFFilterDiscr, "boxFFilterIntPoint" );
    boxFFilterIntPoint->setGeometry( QRect( 120, 110, 100, 21 ) );

    labelFFilterAnisoCoeff2 = new QLabel( groupFFilterDiscr, "labelFFilterAnisoCoeff2" );
    labelFFilterAnisoCoeff2->setGeometry( QRect( 90, 180, 16, 21 ) );

    labelFFilterLambda = new QLabel( groupFFilterDiscr, "labelFFilterLambda" );
    labelFFilterLambda->setGeometry( QRect( 20, 220, 51, 20 ) );

    labelFFilterIntSize = new QLabel( groupFFilterDiscr, "labelFFilterIntSize" );
    labelFFilterIntSize->setGeometry( QRect( 20, 250, 90, 20 ) );

    labelFFilterIntPoint = new QLabel( groupFFilterDiscr, "labelFFilterIntPoint" );
    labelFFilterIntPoint->setGeometry( QRect( 20, 110, 94, 20 ) );

    lineFFilterAnisoCoeff3 = new QLineEdit( groupFFilterDiscr, "lineFFilterAnisoCoeff3" );
    lineFFilterAnisoCoeff3->setGeometry( QRect( 180, 180, 40, 21 ) );

    boxFFilterNonlinType = new QComboBox( FALSE, groupFFilterDiscr, "boxFFilterNonlinType" );
    boxFFilterNonlinType->setGeometry( QRect( 120, 50, 100, 21 ) );

    labelFFilterAnisoCoeff1 = new QLabel( groupFFilterDiscr, "labelFFilterAnisoCoeff1" );
    labelFFilterAnisoCoeff1->setGeometry( QRect( 20, 180, 16, 21 ) );

    lineFFilterAnisoCoeff2 = new QLineEdit( groupFFilterDiscr, "lineFFilterAnisoCoeff2" );
    lineFFilterAnisoCoeff2->setGeometry( QRect( 110, 180, 40, 21 ) );

    boxFFilterKindOfMom = new QComboBox( FALSE, groupFFilterDiscr, "boxFFilterKindOfMom" );
    boxFFilterKindOfMom->setGeometry( QRect( 120, 80, 100, 21 ) );

    labelFFilterKindOfMom = new QLabel( groupFFilterDiscr, "labelFFilterKindOfMom" );
    labelFFilterKindOfMom->setGeometry( QRect( 20, 80, 97, 20 ) );

    lineFFilterAnisoCoeff1 = new QLineEdit( groupFFilterDiscr, "lineFFilterAnisoCoeff1" );
    lineFFilterAnisoCoeff1->setGeometry( QRect( 40, 180, 40, 21 ) );

    labelFFilterAnisoCoeff3 = new QLabel( groupFFilterDiscr, "labelFFilterAnisoCoeff3" );
    labelFFilterAnisoCoeff3->setGeometry( QRect( 160, 180, 16, 21 ) );

    labelFFilterDiffusion = new QLabel( groupFFilterDiscr, "labelFFilterDiffusion" );
    labelFFilterDiffusion->setGeometry( QRect( 20, 20, 54, 20 ) );

    lineFFilterLambda = new QLineEdit( groupFFilterDiscr, "lineFFilterLambda" );
    lineFFilterLambda->setGeometry( QRect( 120, 220, 101, 21 ) );

    boxFFilterDiffusion = new QComboBox( FALSE, groupFFilterDiscr, "boxFFilterDiffusion" );
    boxFFilterDiffusion->setGeometry( QRect( 120, 20, 100, 21 ) );

    lineFFilterIntSize = new QLineEdit( groupFFilterDiscr, "lineFFilterIntSize" );
    lineFFilterIntSize->setGeometry( QRect( 120, 250, 40, 21 ) );

    labelFFilterAnisoCoeff = new QLabel( groupFFilterDiscr, "labelFFilterAnisoCoeff" );
    labelFFilterAnisoCoeff->setGeometry( QRect( 20, 150, 101, 20 ) );

    groupFFilterSolver = new QGroupBox( tabFFilter, "groupFFilterSolver" );
    groupFFilterSolver->setGeometry( QRect( 540, 90, 200, 320 ) );

    lineFFilterLevels = new QLineEdit( groupFFilterSolver, "lineFFilterLevels" );
    lineFFilterLevels->setGeometry( QRect( 120, 210, 60, 21 ) );

    labelFFilterTSteps = new QLabel( groupFFilterSolver, "labelFFilterTSteps" );
    labelFFilterTSteps->setGeometry( QRect( 20, 160, 66, 20 ) );

    labelFFilterLevels = new QLabel( groupFFilterSolver, "labelFFilterLevels" );
    labelFFilterLevels->setGeometry( QRect( 20, 210, 66, 20 ) );

    lineFFilterTau = new QLineEdit( groupFFilterSolver, "lineFFilterTau" );
    lineFFilterTau->setGeometry( QRect( 80, 40, 101, 21 ) );

    labelFFilterTau = new QLabel( groupFFilterSolver, "labelFFilterTau" );
    labelFFilterTau->setGeometry( QRect( 20, 40, 30, 20 ) );

    labelFFilterEpsilon = new QLabel( groupFFilterSolver, "labelFFilterEpsilon" );
    labelFFilterEpsilon->setGeometry( QRect( 20, 90, 47, 20 ) );

    lineFFilterEpsilon = new QLineEdit( groupFFilterSolver, "lineFFilterEpsilon" );
    lineFFilterEpsilon->setGeometry( QRect( 80, 90, 101, 21 ) );

    lineFFilterTSteps = new QLineEdit( groupFFilterSolver, "lineFFilterTSteps" );
    lineFFilterTSteps->setGeometry( QRect( 120, 160, 60, 21 ) );

    labelFFilterSolverType = new QLabel( groupFFilterSolver, "labelFFilterSolverType" );
    labelFFilterSolverType->setGeometry( QRect( 20, 250, 71, 20 ) );

    boxFFilterSolverType = new QComboBox( FALSE, groupFFilterSolver, "boxFFilterSolverType" );
    boxFFilterSolverType->setGeometry( QRect( 70, 280, 111, 21 ) );

    lineFFilterFilterLevel = new QLineEdit( tabFFilter, "lineFFilterFilterLevel" );
    lineFFilterFilterLevel->setGeometry( QRect( 940, 90, 40, 21 ) );

    labelFFilterFilterLevel = new QLabel( tabFFilter, "labelFFilterFilterLevel" );
    labelFFilterFilterLevel->setGeometry( QRect( 840, 90, 63, 20 ) );

	canvFFilter = new QCanvas(512, 512);
    viewFFilter = new nraCubeView( *canvFFilter, tabFFilter, "viewFFilter" );
    viewFFilter->setGeometry( QRect( 10, 10, 516, 516 ) );

    radioSkipFFilter = new QRadioButton( tabFFilter, "radioSkipFFilter" );
    radioSkipFFilter->setGeometry( QRect( 891, 490, 120, 31 ) );
    
    labelFFilterHelpMsg = new QLabel( tabFFilter, "labelFFilterHelpMsg" );
    labelFFilterHelpMsg->setGeometry( QRect( 540, 10, 450, 40 ) );
    labelFFilterHelpMsg->setBackgroundColor( QColor( 255, 255, 210 ) );

    tabViews->insertTab( tabFFilter, "" );

    // First segmentation
    tabFSegm = new QWidget( tabViews, "tabFSegm" );

    pushFSegmFSegm = new QPushButton( tabFSegm, "pushFSegmFSegm" );
    pushFSegmFSegm->setGeometry( QRect( 540, 490, 100, 30 ) );
    
    lineFSegmAbsHighThresh = new QLineEdit( tabFSegm, "lineFSegmAbsHighThresh" );
    lineFSegmAbsHighThresh->setGeometry( QRect( 790, 180, 101, 21 ) );

    labelFSegmAbsHighThresh = new QLabel( tabFSegm, "labelFSegmAbsHighThresh" );
    labelFSegmAbsHighThresh->setGeometry( QRect( 760, 150, 135, 16 ) );

    boxFSegmSegmType = new QComboBox( FALSE, tabFSegm, "boxFSegmSegmType" );
    boxFSegmSegmType->setGeometry( QRect( 670, 90, 220, 20 ) );

    labelFSegmSegmType = new QLabel( tabFSegm, "labelFSegmSegmType" );
    labelFSegmSegmType->setGeometry( QRect( 550, 90, 110, 20 ) );

    labelFSegmEpsilon = new QLabel( tabFSegm, "labelFSegmEpsilon" );
    labelFSegmEpsilon->setGeometry( QRect( 590, 150, 50, 16 ) );

    lineFSegmEpsilon = new QLineEdit( tabFSegm, "lineFSegmEpsilon" );
    lineFSegmEpsilon->setGeometry( QRect( 610, 180, 101, 21 ) );

    labelFSegmAbsLowThresh = new QLabel( tabFSegm, "labelFSegmAbsLowThresh" );
    labelFSegmAbsLowThresh->setGeometry( QRect( 580, 230, 123, 16 ) );

    lineFSegmAbsLowThresh = new QLineEdit( tabFSegm, "lineFSegmAbsLowThresh" );
    lineFSegmAbsLowThresh->setGeometry( QRect( 610, 260, 101, 21 ) );

	canvFSegm = new QCanvas(512, 512);
    viewFSegm = new nraCubeView( *canvFSegm, tabFSegm, "viewFSegm" );
    viewFSegm->setGeometry( QRect( 10, 10, 516, 516 ) );

    radioSkipFSegm = new QRadioButton( tabFSegm, "radioSkipFSegm" );
    radioSkipFSegm->setGeometry( QRect( 891, 490, 120, 31 ) );
    
    labelFSegmHelpMsg = new QLabel( tabFSegm, "labelFSegmHelpMsg" );
    labelFSegmHelpMsg->setGeometry( QRect( 540, 10, 450, 40 ) );
    labelFSegmHelpMsg->setBackgroundColor( QColor( 255, 255, 210 ) );

    tabViews->insertTab( tabFSegm, "" );

    // Second filter
    tabSFilter = new QWidget( tabViews, "tabSFilter" );

    pushSFilterSFilter = new QPushButton( tabSFilter, "pushSFilterSFilter" );
    pushSFilterSFilter->setGeometry( QRect( 540, 490, 100, 30 ) );
    
    groupSFilterSolver = new QGroupBox( tabSFilter, "groupSFilterSolver" );
    groupSFilterSolver->setGeometry( QRect( 540, 100, 200, 320 ) );

    lineSFilterLevels = new QLineEdit( groupSFilterSolver, "lineSFilterLevels" );
    lineSFilterLevels->setGeometry( QRect( 120, 210, 60, 21 ) );

    labelSFilterTSteps = new QLabel( groupSFilterSolver, "labelSFilterTSteps" );
    labelSFilterTSteps->setGeometry( QRect( 20, 160, 66, 20 ) );

    labelSFilterLevels = new QLabel( groupSFilterSolver, "labelSFilterLevels" );
    labelSFilterLevels->setGeometry( QRect( 20, 210, 66, 20 ) );

    lineSFilterTau = new QLineEdit( groupSFilterSolver, "lineSFilterTau" );
    lineSFilterTau->setGeometry( QRect( 80, 40, 101, 21 ) );

    labelSFilterTau = new QLabel( groupSFilterSolver, "labelSFilterTau" );
    labelSFilterTau->setGeometry( QRect( 20, 40, 30, 20 ) );

    labelSFilterEpsilon = new QLabel( groupSFilterSolver, "labelSFilterEpsilon" );
    labelSFilterEpsilon->setGeometry( QRect( 20, 90, 47, 20 ) );

    lineSFilterEpsilon = new QLineEdit( groupSFilterSolver, "lineSFilterEpsilon" );
    lineSFilterEpsilon->setGeometry( QRect( 80, 90, 101, 21 ) );

    lineSFilterTSteps = new QLineEdit( groupSFilterSolver, "lineSFilterTSteps" );
    lineSFilterTSteps->setGeometry( QRect( 120, 160, 60, 21 ) );

    labelSFilterSolverType = new QLabel( groupSFilterSolver, "labelSFilterSolverType" );
    labelSFilterSolverType->setGeometry( QRect( 20, 250, 71, 20 ) );

    boxSFilterSolverType = new QComboBox( FALSE, groupSFilterSolver, "boxSFilterSolverType" );
    boxSFilterSolverType->setGeometry( QRect( 70, 280, 111, 21 ) );

    groupSFilterDiscr = new QGroupBox( tabSFilter, "groupSFilterDiscr" );
    groupSFilterDiscr->setGeometry( QRect( 750, 140, 240, 280 ) );

    labelSFilterNonlinType = new QLabel( groupSFilterDiscr, "labelSFilterNonlinType" );
    labelSFilterNonlinType->setGeometry( QRect( 20, 50, 90, 20 ) );

    boxSFilterIntPoint = new QComboBox( FALSE, groupSFilterDiscr, "boxSFilterIntPoint" );
    boxSFilterIntPoint->setGeometry( QRect( 120, 110, 100, 21 ) );

    labelSFilterAnisoCoeff2 = new QLabel( groupSFilterDiscr, "labelSFilterAnisoCoeff2" );
    labelSFilterAnisoCoeff2->setGeometry( QRect( 90, 180, 16, 21 ) );

    labelSFilterLambda = new QLabel( groupSFilterDiscr, "labelSFilterLambda" );
    labelSFilterLambda->setGeometry( QRect( 20, 220, 51, 20 ) );

    labelSFilterIntSize = new QLabel( groupSFilterDiscr, "labelSFilterIntSize" );
    labelSFilterIntSize->setGeometry( QRect( 20, 250, 90, 20 ) );

    labelSFilterIntPoint = new QLabel( groupSFilterDiscr, "labelSFilterIntPoint" );
    labelSFilterIntPoint->setGeometry( QRect( 20, 110, 94, 20 ) );

    lineSFilterAnisoCoeff3 = new QLineEdit( groupSFilterDiscr, "lineSFilterAnisoCoeff3" );
    lineSFilterAnisoCoeff3->setGeometry( QRect( 180, 180, 40, 21 ) );

    boxSFilterNonlinType = new QComboBox( FALSE, groupSFilterDiscr, "boxSFilterNonlinType" );
    boxSFilterNonlinType->setGeometry( QRect( 120, 50, 100, 21 ) );

    labelSFilterAnisoCoeff1 = new QLabel( groupSFilterDiscr, "labelSFilterAnisoCoeff1" );
    labelSFilterAnisoCoeff1->setGeometry( QRect( 20, 180, 16, 21 ) );

    lineSFilterAnisoCoeff2 = new QLineEdit( groupSFilterDiscr, "lineSFilterAnisoCoeff2" );
    lineSFilterAnisoCoeff2->setGeometry( QRect( 110, 180, 40, 21 ) );

    boxSFilterKindOfMom = new QComboBox( FALSE, groupSFilterDiscr, "boxSFilterKindOfMom" );
    boxSFilterKindOfMom->setGeometry( QRect( 120, 80, 100, 21 ) );

    labelSFilterKindOfMom = new QLabel( groupSFilterDiscr, "labelSFilterKindOfMom" );
    labelSFilterKindOfMom->setGeometry( QRect( 20, 80, 97, 20 ) );

    lineSFilterAnisoCoeff1 = new QLineEdit( groupSFilterDiscr, "lineSFilterAnisoCoeff1" );
    lineSFilterAnisoCoeff1->setGeometry( QRect( 40, 180, 40, 21 ) );

    labelSFilterAnisoCoeff3 = new QLabel( groupSFilterDiscr, "labelSFilterAnisoCoeff3" );
    labelSFilterAnisoCoeff3->setGeometry( QRect( 160, 180, 16, 21 ) );

    labelSFilterDiffusion = new QLabel( groupSFilterDiscr, "labelSFilterDiffusion" );
    labelSFilterDiffusion->setGeometry( QRect( 20, 20, 54, 20 ) );

    lineSFilterLambda = new QLineEdit( groupSFilterDiscr, "lineSFilterLambda" );
    lineSFilterLambda->setGeometry( QRect( 120, 220, 101, 21 ) );

    boxSFilterDiffusion = new QComboBox( FALSE, groupSFilterDiscr, "boxSFilterDiffusion" );
    boxSFilterDiffusion->setGeometry( QRect( 120, 20, 100, 21 ) );

    lineSFilterIntSize = new QLineEdit( groupSFilterDiscr, "lineSFilterIntSize" );
    lineSFilterIntSize->setGeometry( QRect( 120, 250, 40, 21 ) );

    labelSFilterAnisoCoeff = new QLabel( groupSFilterDiscr, "labelSFilterAnisoCoeff" );
    labelSFilterAnisoCoeff->setGeometry( QRect( 20, 150, 101, 20 ) );

    lineSFilterFilterLevel = new QLineEdit( tabSFilter, "lineSFilterFilterLevel" );
    lineSFilterFilterLevel->setGeometry( QRect( 940, 100, 40, 21 ) );

    labelSFilterFilterLevel = new QLabel( tabSFilter, "labelSFilterFilterLevel" );
    labelSFilterFilterLevel->setGeometry( QRect( 840, 100, 63, 20 ) );

	canvSFilter = new QCanvas(512, 512);
    viewSFilter = new nraCubeView( *canvSFilter, tabSFilter, "viewSFilter" );
    viewSFilter->setGeometry( QRect( 10, 10, 516, 516 ) );

    radioSkipSFilter = new QRadioButton( tabSFilter, "radioSkipSFilter" );
    radioSkipSFilter->setGeometry( QRect( 891, 490, 120, 31 ) );
    
    labelSFilterHelpMsg = new QLabel( tabSFilter, "labelSFilterHelpMsg" );
    labelSFilterHelpMsg->setGeometry( QRect( 540, 10, 450, 40 ) );
    labelSFilterHelpMsg->setBackgroundColor( QColor( 255, 255, 210 ) );
    
    tabViews->insertTab( tabSFilter, "" );


    // Second segmentation
    tabSSegm = new QWidget( tabViews, "tabSSegm" );

    pushSSegmSSegm = new QPushButton( tabSSegm, "pushSSegmSSegm" );
    pushSSegmSSegm->setGeometry( QRect( 540, 490, 100, 30 ) );
    
    labelSSegmSegmType = new QLabel( tabSSegm, "labelSSegmSegmType" );
    labelSSegmSegmType->setGeometry( QRect( 550, 90, 110, 20 ) );

    boxSSegmSegmType = new QComboBox( FALSE, tabSSegm, "boxSSegmSegmType" );
    boxSSegmSegmType->setGeometry( QRect( 670, 90, 220, 20 ) );

    labelSSegmEpsilon = new QLabel( tabSSegm, "labelSSegmEpsilon" );
    labelSSegmEpsilon->setGeometry( QRect( 590, 150, 50, 16 ) );

    lineSSegmAbsHighThresh = new QLineEdit( tabSSegm, "lineSSegmAbsHighThresh" );
    lineSSegmAbsHighThresh->setGeometry( QRect( 790, 180, 101, 21 ) );

    labelSSegmAbsHighThresh = new QLabel( tabSSegm, "labelSSegmAbsHighThresh" );
    labelSSegmAbsHighThresh->setGeometry( QRect( 760, 150, 135, 16 ) );

    labelSSegmAbsLowThresh = new QLabel( tabSSegm, "labelSSegmAbsLowThresh" );
    labelSSegmAbsLowThresh->setGeometry( QRect( 580, 230, 123, 16 ) );

    lineSSegmAbsLowThresh = new QLineEdit( tabSSegm, "lineSSegmAbsLowThresh" );
    lineSSegmAbsLowThresh->setGeometry( QRect( 610, 260, 101, 21 ) );

    lineSSegmEpsilon = new QLineEdit( tabSSegm, "lineSSegmEpsilon" );
    lineSSegmEpsilon->setGeometry( QRect( 610, 180, 101, 21 ) );

	canvSSegm = new QCanvas(512, 512);
    viewSSegm = new nraCubeView( *canvSSegm, tabSSegm, "viewSSegm" );
    viewSSegm->setGeometry( QRect( 10, 10, 516, 516 ) );

    radioSkipSSegm = new QRadioButton( tabSSegm, "radioSkipSSegm" );
    radioSkipSSegm->setGeometry( QRect( 891, 490, 120, 31 ) );
    
    labelSSegmHelpMsg = new QLabel( tabSSegm, "labelSSegmHelpMsg" );
    labelSSegmHelpMsg->setGeometry( QRect( 540, 10, 450, 40 ) );
    labelSSegmHelpMsg->setBackgroundColor( QColor( 255, 255, 210 ) );
    
    tabViews->insertTab( tabSSegm, "" );

    // Postprocessing
    /*
    tabPostproc = new QWidget( tabViews, "tabPostproc" );
    
    pushPostprocPostproc = new QPushButton( tabPostproc, "pushPostprocPostproc" );
    pushPostprocPostproc->setGeometry( QRect( 540, 490, 100, 30 ) );
    
    viewPostproc = new QDatacubeViewer( tabPostproc, "viewPostproc" );
    viewPostproc->setGeometry( QRect( 10, 10, 512, 512 ) );
    viewPostproc->setPaletteBackgroundColor( QColor( 0, 0, 0 ) );
    
    radioSkipPostproc = new QRadioButton( tabPostproc, "radioSkipPostproc" );
    radioSkipPostproc->setGeometry( QRect( 891, 0, 120, 31 ) );
    tabViews->insertTab( tabPostproc, "");
    */
    
    // Preprocessing
    tabPreproc = new QWidget( tabViews, "tabPreproc" );

    pushPreprocPreproc = new QPushButton( tabPreproc, "pushPreprocPreproc" );
    pushPreprocPreproc->setGeometry( QRect( 540, 490, 100, 30 ) );

    checkPreprocDelBound = new QCheckBox( tabPreproc, "checkPreprocDelBound" );
    checkPreprocDelBound->setGeometry( QRect( 550, 70, 110, 20 ) );

    checkPreprocErasePipet = new QCheckBox( tabPreproc, "checkPreprocErasePipet" );
    checkPreprocErasePipet->setGeometry( QRect( 550, 110, 90, 16 ) );

    radioSkipPreProc = new QRadioButton( tabPreproc, "radioSkipPreProc" );
    radioSkipPreProc->setGeometry( QRect( 891, 490, 120, 31 ) );

	canvPreproc = new QCanvas(512, 512);
    viewPreproc = new nraCubeView( *canvPreproc, tabPreproc, "viewPreproc" );
    viewPreproc->setGeometry( QRect( 10, 10, 516, 516 ) );
    
    labelPreprocHelpMsg = new QLabel( tabPreproc, "labelPreprocHelpMsg" );
    labelPreprocHelpMsg->setGeometry( QRect( 540, 10, 450, 40 ) );
    labelPreprocHelpMsg->setBackgroundColor( QColor( 255, 255, 210 ) );
    
    tabViews->insertTab( tabPreproc, "" );


    // Tree generation
    tabCellBody = new QWidget( tabViews, "tabCellBody" );

    pushCellBodyCellBody = new QPushButton( tabCellBody, "pushCellBodyCellBody" );
    pushCellBodyCellBody->setGeometry( QRect( 540, 490, 100, 30 ) );
    
    /*
    labelCellBodyMaxLevel = new QLabel( tabCellBody, "labelCellBodyMaxLevel" );
    labelCellBodyMaxLevel->setGeometry( QRect( 550, 310, 58, 16 ) );
    */
    
    labelCellBodyThreshold = new QLabel( tabCellBody, "labelCellBodyThreshold" );
    labelCellBodyThreshold->setGeometry( QRect( 670, 330, 84, 16 ) );

    toolButtonCellBodyDX = new QToolButton( tabCellBody, "toolButtonCellBodyDX" );
    toolButtonCellBodyDX->setGeometry( QRect( 920, 270, 40, 20 ) );

    labelCellBodyDXFilename = new QLabel( tabCellBody, "labelCellBodyDXFilename" );
    labelCellBodyDXFilename->setGeometry( QRect( 550, 240, 60, 16 ) );

    labelCellBodyDXFilenameDisplay = new QLabel( tabCellBody, "labelCellBodyDXFilenameDisplay" );
    labelCellBodyDXFilenameDisplay->setGeometry( QRect( 550, 270, 350, 20 ) );
    labelCellBodyDXFilenameDisplay->setPaletteBackgroundColor( QColor(255, 255, 255) );

    toolButtonCellBodyHOC = new QToolButton( tabCellBody, "toolButtonCellBodyHOC" );
    toolButtonCellBodyHOC->setGeometry( QRect( 920, 170, 40, 20 ) );

    labelCellBodyHOCFilename = new QLabel( tabCellBody, "labelCellBodyHOCFilename" );
    labelCellBodyHOCFilename->setGeometry( QRect( 550, 140, 60, 16 ) );

    labelCellBodyHOCFilenameDisplay = new QLabel( tabCellBody, "labelCellBodyDXFilenameDisplay" );
    labelCellBodyHOCFilenameDisplay->setGeometry( QRect( 550, 170, 350, 20 ) );
    labelCellBodyHOCFilenameDisplay->setPaletteBackgroundColor( QColor(255, 255, 255) );

    labelCellBodyThresholdLevel = new QLabel( tabCellBody, "labelCellBodyThresholdLevel" );
    labelCellBodyThresholdLevel->setGeometry( QRect( 820, 330, 91, 16 ) );

 
    lineCellBodyThreshold = new QLineEdit( tabCellBody, "lineCellBodyThreshold" );
    lineCellBodyThreshold->setGeometry( QRect( 690, 360, 90, 20 ) );

    lineCellBodyThresholdLevel = new QLineEdit( tabCellBody, "lineCellBodyThresholdLevel" );
    lineCellBodyThresholdLevel->setGeometry( QRect( 840, 360, 90, 20 ) );

	canvCellBody = new QCanvas(512, 512);
    viewCellBody = new nraCubeView( *canvCellBody, tabCellBody, "viewCellBody" );
    viewCellBody->setGeometry( QRect( 10, 10, 516, 516 ) );

    checkCellBodyWriteHOC = new QCheckBox( tabCellBody, "checkCellBodyWriteHOC" );
    checkCellBodyWriteHOC->setGeometry( QRect( 550, 110, 110, 16 ) );

    checkCellBodyWriteDX = new QCheckBox( tabCellBody, "checkCellBodyWriteDX" );
    checkCellBodyWriteDX->setGeometry( QRect( 550, 210, 70, 16 ) );
    
    /*
    lineCellBodyMaxLevel = new QLineEdit( tabCellBody, "lineCellBodyMaxLevel" );
    lineCellBodyMaxLevel->setGeometry( QRect( 570, 340, 40, 21 ) );
    */
    
    /*
    checkCellBodyCreateRecon = new QCheckBox( tabCellBody, "checkCellBodyCreateRecon" );
    checkCellBodyCreateRecon->setGeometry( QRect( 550, 70, 140, 16 ) );
    */
    
    radioSkipCellBody = new QRadioButton( tabCellBody, "radioSkipCellBody" );
    radioSkipCellBody->setGeometry( QRect( 891, 490, 120, 31 ) );
    
    labelCellBodyHelpMsg = new QLabel( tabCellBody, "labelCellBodyHelpMsg" );
    labelCellBodyHelpMsg->setGeometry( QRect( 540, 10, 450, 40 ) );
    labelCellBodyHelpMsg->setBackgroundColor( QColor( 255, 255, 210 ) );
    
    tabViews->insertTab( tabCellBody, "" );

    // Detect terminations
    tabDetectTerm = new QWidget( tabViews, "tabDetectTerm" );
    
    pushDetectTermDetectTerm = new QPushButton( tabDetectTerm, "pushDetectTermDetectTerm" );
    pushDetectTermDetectTerm->setGeometry( QRect( 540, 490, 200, 30 ) );
    
    labelDetectTermDisplayType = new QLabel( tabDetectTerm, "labelDetectTermDisplayType" );
    labelDetectTermDisplayType->setGeometry( QRect( 550, 70, 110, 40 ) );
    
    boxDetectTermDisplayType = new QComboBox( FALSE, tabDetectTerm, "boxSFilterDiffusion" );
    boxDetectTermDisplayType->setGeometry( QRect( 570, 110, 200, 21 ) );
    
    canvDetectTerm = new QCanvas(512, 512);
    viewDetectTerm = new nraCubeView( *canvDetectTerm, tabDetectTerm, "viewDetecTerm" );
    viewDetectTerm->setGeometry( QRect( 10, 10, 516, 516 ) );
    
    radioSkipDetectTerm = new QRadioButton( tabDetectTerm, "radioSkipDetectTerm" );
    radioSkipDetectTerm->setGeometry( QRect( 891, 490, 120, 31 ) );

    labelDetectTermHelpMsg = new QLabel( tabDetectTerm, "labelDetectTermHelpMsg" );
    labelDetectTermHelpMsg->setGeometry( QRect( 540, 10, 450, 40 ) );
    labelDetectTermHelpMsg->setBackgroundColor( QColor( 255, 255, 210 ) );

    tabViews->insertTab( tabDetectTerm, "");

    // Final
    tabFinal = new QWidget( tabViews, "tabFinal" );
    
    pushFinalFinal = new QPushButton( tabFinal, "pushFinalFinal" );
    pushFinalFinal->setGeometry( QRect( 540, 490, 100, 30 ) );
    
    canvFinal = new QCanvas(512, 512);
    viewFinal = new nraCubeView( *canvFinal, tabFinal, "viewFinal" );
    viewFinal->setGeometry( QRect( 10, 10, 516, 516 ) );
    
    radioSkipFinal = new QRadioButton( tabFinal, "radioSkipFinal" );
    radioSkipFinal->setGeometry( QRect( 891, 490, 120, 31 ) );

    labelFinalHelpMsg = new QLabel( tabFinal, "labelFinalHelpMsg" );
    labelFinalHelpMsg->setGeometry( QRect( 540, 10, 450, 40 ) );
    labelFinalHelpMsg->setBackgroundColor( QColor( 255, 255, 210 ) );
    
    tabViews->insertTab( tabFinal, "");


    // toolbars

    languageChange();
    setFixedSize(1030, 640);
    //resize( QSize(1030, 620).expandedTo(minimumSizeHint()) );

    // Connect first view. m_pCurrentView is a
    // pointer that stores the current view for
    // later disconnection in the event of a page flip.
    m_pCurrentView = viewLoading;
    ConnectView(m_pCurrentView);
    

    // Internal connects
    connect(radioLoadTest,		SIGNAL	(toggled(bool)),
            this,			SLOT	(OnLoadTestToggled(bool)));
    connect(checkLoadMultipleFiles,	SIGNAL	(toggled(bool)),
            this,			SLOT	(OnLoadMulFilesToggled(bool)));
    connect(checkLoadExtractVol,	SIGNAL	(toggled(bool)),
            this,			SLOT	(OnLoadExtrVolToggled(bool)));
    connect(sliderSlice,		SIGNAL	(valueChanged(int)),
            this,			SLOT	(OnSliderValueChanged(int)));
                    
    // 'Loading' page
    connect(pushLoadLoad,		SIGNAL	(clicked()),
            this,			SLOT	(OnLoadButtonClicked()));

    connect(tabViews,			SIGNAL	(currentChanged(QWidget*)),
            this,			SLOT	(OnTabFlip(QWidget*)));

    
    // 'Preprocess' page
    connect(pushPreprocPreproc,		SIGNAL(clicked()),
            this,			SLOT(OnPreprocButtonClicked()));
    
    // 'Transformation' page
    connect(pushTrafoTrafo,		SIGNAL(clicked()),
            this,			SLOT(OnTrafoButtonClicked()));
            
    // 'First filter' page
    connect(pushFFilterFFilter,		SIGNAL(clicked()),
            this,			SLOT(OnFFilterButtonClicked()));
            
    // 'First segmentation' page
    connect(pushFSegmFSegm,		SIGNAL(clicked()),
            this,			SLOT(OnFSegmButtonClicked()));

    // 'Second filter' page
    connect(pushSFilterSFilter,		SIGNAL(clicked()),
            this,			SLOT(OnSFilterButtonClicked()));
            
    // 'Second segmentation' page
    connect(pushSSegmSSegm,		SIGNAL(clicked()),
            this,			SLOT(OnSSegmButtonClicked()));
            
    // 'Tree generation' page
    connect(pushCellBodyCellBody,	SIGNAL(clicked()),
            this,			SLOT(OnTreeGenButtonClicked()));
    connect(toolButtonCellBodyHOC,	SIGNAL(clicked()),
            this,			SLOT(OnCellBodySaveHOCClicked()));
    connect(toolButtonCellBodyDX,	SIGNAL(clicked()),
            this,			SLOT(OnCellBodySaveDXClicked()));
            
   // 'Detect terminations' page
    connect(pushDetectTermDetectTerm,	SIGNAL(clicked()),
            this,			SLOT(OnDetectTermButtonClicked()));
    connect(boxDetectTermDisplayType,	SIGNAL(activated(int)),
            this,			SLOT(OnSetDisplayType(int)));
    

            
    // Save button 
    connect(pushSave,			SIGNAL(clicked()),
            this,			SLOT(OnSaveButtonClicked()));
            
    // Preference button 
    connect(pushPref,			SIGNAL(clicked()),
            this,			SLOT(OnPrefButtonClicked()));


}

/*
 *  Destroys the object and frees any allocated resources
 */
nraMainWindow::~nraMainWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void nraMainWindow::languageChange()
{
    QString DefString("");
    setCaption( tr( "NeuRA" ) );
    labelSlider->setText( tr( "-" ) );
    pushSave->setText( tr("Save cube") );
    pushPref->setText( tr("Preferences") );
    
    // UI elements of "Loading" tab.
    LoadingOptions DefLoad;
    
    labelLoadHelpMsg->setText( QString(DefLoad.HelpMsg) );
    radioLoadTest->setText( tr( "Test" ) );
    if(DefLoad.filetype == TEST)
    {
        radioLoadTest->setDown(true);
    }
    else
    {
        radioLoadTest->setDown(false);
    }
    labelLoadChannel->setText( tr( "Channel:" ) );
    
    lineLoadChannel->setText( DefString.setNum(DefLoad.channel) );
    checkLoadMultipleFiles->setText( tr( "Multiple files" ) );
    if(DefLoad.MultipleFiles == YES)
    {
        checkLoadMultipleFiles->setDown(true);
    }
    else
    {
        checkLoadMultipleFiles->setDown(false);
    }
    groupLoadBehaviour->setTitle( tr( "Load behaviour" ) );
    labelLoadBehStart->setText( tr( "Start number:" ) );
    labelLoadBehEnd->setText( tr( "End number:" ) );
    lineLoadBehStart->setText( DefString.setNum(DefLoad.StartNr) );
    lineLoadBehEnd->setText( DefString.setNum(DefLoad.EndNr) );
    checkLoadExtractVol->setText( tr( "Extract volume" ) );
    if(DefLoad.extractVolume == YES)
    {
        checkLoadExtractVol->setDown(true);
    }
    else
    {
        checkLoadExtractVol->setDown(false);
    }
    groupLoadVolume->setTitle( tr( "Volume" ) );
    labelLoadVolStart->setText( tr( "Start:" ) );
    labelLoadVolStartX->setText( tr( "X:" ) );
    labelLoadVolStartY->setText( tr( "Y:" ) );
    labelLoadVolStartZ->setText( tr( "Z:" ) );
    lineLoadVolStartX->setText( DefString.setNum(DefLoad.start[1]) );
    lineLoadVolStartY->setText( DefString.setNum(DefLoad.start[2]) );
    lineLoadVolStartZ->setText( DefString.setNum(DefLoad.start[3]) );
    labelLoadVolSize->setText( tr( "Size:" ) );
    labelLoadVolSizeX->setText( tr( "X:" ) );
    labelLoadVolSizeY->setText( tr( "Y:" ) );
    labelLoadVolSizeZ->setText( tr( "Z:" ) );
    lineLoadVolSizeX->setText( DefString.setNum(DefLoad.size[1]) );
    lineLoadVolSizeY->setText( DefString.setNum(DefLoad.size[2]) );
    lineLoadVolSizeZ->setText( DefString.setNum(DefLoad.size[3]) );
    labelLoadMarkType->setText( tr( "Type of Mark:" ) );
    boxLoadMarkType->clear();
    boxLoadMarkType->insertItem( tr( "Position" ) );
    boxLoadMarkType->insertItem( tr( "Direction" ) );
    boxLoadMarkType->insertItem( tr( "Axon" ) );
    pushLoadLoad->setText( tr( "Load ..." ) );
    
    tabViews->changeTab( tabLoading, tr( "Loading" ) );
    
    // UI elements of 'Preprocess' tab.
    PreProcessOptions DefPreproc;
    labelPreprocHelpMsg->setText( QString(DefPreproc.HelpMsg) );
    checkPreprocDelBound->setText( tr( "Delete boundry" ) );
    if(DefPreproc.deleteBoundary == YES)
    {
        checkPreprocDelBound->setDown(true);
    }
    else
    {
        checkPreprocDelBound->setDown(false);
    }
    checkPreprocErasePipet->setText( tr( "Erase pipet" ) );
    if(DefPreproc.erasePipet == YES)
    {
        checkPreprocErasePipet->setDown(true);
    }
    else
    {
        checkPreprocErasePipet->setDown(false);
    }   
    radioSkipPreProc->setText( tr( "Skip calculation" ) );
    if(DefPreproc.skip == YES)
    {
        radioSkipPreProc->setDown(true);    
    }
    else
    { 
        radioSkipPreProc->setDown(false);
    }
    pushPreprocPreproc->setText( tr( "Preprocess") );
    
    tabViews->changeTab( tabPreproc, tr( "Preprocess" ) );
    
    // UI elements of 'Transformation' tab.
    TransformOptions DefTrafo;
    labelTrafoHelpMsg->setText( QString(DefTrafo.HelpMsg) );
    radioSkipTrafo->setText( tr( "Skip calculation" ) );
    if(DefTrafo.skip == YES)
    {
        radioSkipTrafo->setDown(true);    
    }
    else
    { 
        radioSkipTrafo->setDown(false);
    }
    labelTrafoThreshLow->setText( tr( "Threshold low:" ) );
    lineTrafoThreshLow->setText( DefString.setNum(DefTrafo.ThresholdLow) );
    labelTrafoThreshHigh->setText( tr( "Threshold high:" ) );
    lineTrafoThreshHigh->setText( DefString.setNum(DefTrafo.ThresholdHigh) );
    pushTrafoTrafo->setText( tr("Transform") );
    
    tabViews->changeTab( tabTrafo, tr( "Transform" ) );
    
    // UI elements of 'First filter' tab.
    RecFilterOptions DefFilter;
    labelFFilterHelpMsg->setText( QString(DefFilter.HelpMsg) );
    pushFFilterFFilter->setText( tr( "Filter" ) );
    radioSkipFFilter->setText( tr( "Skip calculation" ) );
    if(DefFilter.skip == YES)
    {
        radioSkipFFilter->setDown(true);    
    }
    else
    { 
        radioSkipFFilter->setDown(false);
    }
    groupFFilterSolver->setTitle( tr( "Solver" ) );
    labelFFilterTau->setText( tr( "Tau:" ) );
    lineFFilterTau->setText( DefString.setNum(DefFilter.tau) );
    labelFFilterEpsilon->setText( tr( "Epsilon:" ) );
    lineFFilterEpsilon->setText( DefString.setNum(DefFilter.epsilon) );
    labelFFilterTSteps->setText( tr( "Time steps:" ) );
    lineFFilterTSteps->setText( DefString.setNum(DefFilter.time_steps) );
    labelFFilterLevels->setText( tr( "Levels:" ) );
    lineFFilterLevels->setText( DefString.setNum(DefFilter.levels) );
    labelFFilterSolverType->setText( tr( "Solver type:" ) );
    boxFFilterSolverType->clear();
    boxFFilterSolverType->insertItem( tr( "CG Solver" ) );
    boxFFilterSolverType->insertItem( tr( "Multigrid" ) );
    labelFFilterFilterLevel->setText( tr( "Filter level:" ) );
    lineFFilterFilterLevel->setText( DefString.setNum(DefFilter.filterLevel) );
    groupFFilterDiscr->setTitle( tr( "Discretization" ) );
    labelFFilterDiffusion->setText( tr( "Diffusion:" ) );
    boxFFilterDiffusion->clear();
    boxFFilterDiffusion->insertItem( tr( "Nonlinear" ) );
    boxFFilterDiffusion->insertItem( tr( "Linear" ) );
    labelFFilterNonlinType->setText( tr( "Nonlinear type:" ) );
    boxFFilterNonlinType->clear();
    boxFFilterNonlinType->insertItem( tr( "Perona Malik" ) );
    boxFFilterNonlinType->insertItem( tr( "Weikert" ) );
    boxFFilterNonlinType->insertItem( tr( "Black Sapiro" ) );
    labelFFilterKindOfMom->setText( tr( "Kind of moments:" ) );
    boxFFilterKindOfMom->clear();
    boxFFilterKindOfMom->insertItem( tr( "Nodewise" ) );
    boxFFilterKindOfMom->insertItem( tr( "Elementwise" ) );
    labelFFilterIntPoint->setText( tr( "Integration point:" ) );
    boxFFilterIntPoint->clear();
    boxFFilterIntPoint->insertItem( tr( "Usual" ) );
    boxFFilterIntPoint->insertItem( tr( "Boundary" ) );
    labelFFilterAnisoCoeff->setText( tr( "Aniso coefficients:" ) );
    labelFFilterAnisoCoeff1->setText( tr( "#1:" ) );
    lineFFilterAnisoCoeff1->setText( DefString.setNum(DefFilter.anicoeff1) );
    labelFFilterAnisoCoeff2->setText( tr( "#2:" ) );
    lineFFilterAnisoCoeff2->setText( DefString.setNum(DefFilter.anicoeff2) );
    labelFFilterAnisoCoeff3->setText( tr( "#3:" ) );
    lineFFilterAnisoCoeff3->setText( DefString.setNum(DefFilter.anicoeff3) );
    labelFFilterLambda->setText( tr( "Lambda:" ) );
    lineFFilterLambda->setText( DefString.setNum(DefFilter.lambda) );
    labelFFilterIntSize->setText( tr( "Integration size:" ) );
    lineFFilterIntSize->setText( DefString.setNum(DefFilter.integration_size_x) );
        
    tabViews->changeTab( tabFFilter, tr( "First filter" ) );
    
    // UI elements of 'First segmentation' tab.
    SegmentationOptions DefSegm;
    labelFSegmHelpMsg->setText( QString(DefSegm.HelpMsg) );
    pushFSegmFSegm->setText( tr( "Segmentation" ) );
    radioSkipFSegm->setText( tr( "Skip calculation" ) );
    if(DefSegm.skip == YES)
    {
        radioSkipFSegm->setDown(true);    
    }
    else
    { 
        radioSkipFSegm->setDown(false);
    }
    labelFSegmSegmType->setText( tr( "Segmentation type:" ) );
    boxFSegmSegmType->clear();
    boxFSegmSegmType->insertItem( tr( "Local mean value and neighbours" ) );
    boxFSegmSegmType->insertItem( tr( "Global thresholding" ) );
    boxFSegmSegmType->insertItem( tr( "Otsu" ) );
    boxFSegmSegmType->insertItem( tr( "Fast marching" ) );
    labelFSegmEpsilon->setText( tr( "Epsilon:" ) );
    lineFSegmEpsilon->setText( DefString.setNum(DefSegm.epsilon) );
    labelFSegmAbsHighThresh->setText( tr( "Absolute high threshold:" ) );
    lineFSegmAbsHighThresh->setText( DefString.setNum(DefSegm.absoluteHighThreshold) );
    labelFSegmAbsLowThresh->setText( tr( "Absolute low threshold:" ) );
    lineFSegmAbsLowThresh->setText( DefString.setNum(DefSegm.absoluteLowThreshold) );

    tabViews->changeTab( tabFSegm, tr( "First segmentation" ) );
    
    // UI elements of 'Second filter' tab.
    labelSFilterHelpMsg->setText( QString(DefFilter.HelpMsg) );
    pushSFilterSFilter->setText( tr( "Filter" ) );
    radioSkipSFilter->setText( tr( "Skip calculation" ) );
    if(DefFilter.skip == YES)
    {
        radioSkipSFilter->setDown(true);    
    }
    else
    { 
        radioSkipSFilter->setDown(false);
    }
    groupSFilterSolver->setTitle( tr( "Solver" ) );
    labelSFilterTau->setText( tr( "Tau:" ) );
    lineSFilterTau->setText( DefString.setNum(DefFilter.tau) );
    labelSFilterEpsilon->setText( tr( "Epsilon:" ) );
    lineSFilterEpsilon->setText( DefString.setNum(DefFilter.epsilon) );
    labelSFilterTSteps->setText( tr( "Time steps:" ) );
    lineSFilterTSteps->setText( DefString.setNum(DefFilter.time_steps) );
    labelSFilterLevels->setText( tr( "Levels:" ) );
    lineSFilterLevels->setText( DefString.setNum(DefFilter.levels) );
    labelSFilterSolverType->setText( tr( "Solver type:" ) );
    boxSFilterSolverType->clear();
    boxSFilterSolverType->insertItem( tr( "CG Solver" ) );
    boxSFilterSolverType->insertItem( tr( "Multigrid" ) );
    labelSFilterFilterLevel->setText( tr( "Filter level:" ) );
    lineSFilterFilterLevel->setText( DefString.setNum(DefFilter.filterLevel) );
    groupSFilterDiscr->setTitle( tr( "Discretization" ) );
    labelSFilterDiffusion->setText( tr( "Diffusion:" ) );
    boxSFilterDiffusion->clear();
    boxSFilterDiffusion->insertItem( tr( "Nonlinear" ) );
    boxSFilterDiffusion->insertItem( tr( "Linear" ) );
    labelSFilterNonlinType->setText( tr( "Nonlinear type:" ) );
    boxSFilterNonlinType->clear();
    boxSFilterNonlinType->insertItem( tr( "Perona Malik" ) );
    boxSFilterNonlinType->insertItem( tr( "Weikert" ) );
    boxSFilterNonlinType->insertItem( tr( "Black Sapiro" ) );
    labelSFilterKindOfMom->setText( tr( "Kind of moments:" ) );
    boxSFilterKindOfMom->clear();
    boxSFilterKindOfMom->insertItem( tr( "Nodewise" ) );
    boxSFilterKindOfMom->insertItem( tr( "Elementwise" ) );
    labelSFilterIntPoint->setText( tr( "Integration point:" ) );
    boxSFilterIntPoint->clear();
    boxSFilterIntPoint->insertItem( tr( "Usual" ) );
    boxSFilterIntPoint->insertItem( tr( "Boundary" ) );
    labelSFilterAnisoCoeff->setText( tr( "Aniso coefficients:" ) );
    labelSFilterAnisoCoeff1->setText( tr( "#1:" ) );
    lineSFilterAnisoCoeff1->setText( DefString.setNum(DefFilter.anicoeff1) );
    labelSFilterAnisoCoeff2->setText( tr( "#2:" ) );
    lineSFilterAnisoCoeff2->setText( DefString.setNum(DefFilter.anicoeff2) );
    labelSFilterAnisoCoeff3->setText( tr( "#3:" ) );
    lineSFilterAnisoCoeff3->setText( DefString.setNum(DefFilter.anicoeff3) );
    labelSFilterLambda->setText( tr( "Lambda:" ) );
    lineSFilterLambda->setText( DefString.setNum(DefFilter.lambda) );
    labelSFilterIntSize->setText( tr( "Integration size:" ) );
    lineSFilterIntSize->setText( DefString.setNum(DefFilter.integration_size_x) );
    
    tabViews->changeTab( tabSFilter, tr( "Second filter" ) );
    
    // UI elements of 'Second segmentation' tab.
    labelSSegmHelpMsg->setText( QString(DefSegm.HelpMsg) );
    pushSSegmSSegm->setText( tr( "Segmentation" ) );
    radioSkipSSegm->setText( tr( "Skip calculation" ) );
    if(DefSegm.skip == YES)
    {
        radioSkipSSegm->setDown(true);    
    }
    else
    { 
        radioSkipSSegm->setDown(false);
    }
    labelSSegmSegmType->setText( tr( "Segmentation type:" ) );
    boxSSegmSegmType->clear();
    boxSSegmSegmType->insertItem( tr( "Local mean value and neighbours" ) );
    boxSSegmSegmType->insertItem( tr( "Global thresholding" ) );
    boxSSegmSegmType->insertItem( tr( "Otsu" ) );
    boxSSegmSegmType->insertItem( tr( "Fast marching" ) );
    labelSSegmEpsilon->setText( tr( "Epsilon:" ) );
    lineSSegmEpsilon->setText( DefString.setNum(DefSegm.epsilon) );
    labelSSegmAbsHighThresh->setText( tr( "Absolute high threshold:" ) );
    lineSSegmAbsHighThresh->setText( DefString.setNum(DefSegm.absoluteHighThreshold) );
    labelSSegmAbsLowThresh->setText( tr( "Absolute low threshold:" ) );
    lineSSegmAbsLowThresh->setText( DefString.setNum(DefSegm.absoluteLowThreshold) );

    tabViews->changeTab( tabSSegm, tr( "Second segmentation" ) );
    
    // UI elements of 'Postprocess' tab.
    /*
    pushPostprocPostproc->setText( tr( "Postprocess" ) );
    
    radioSkipPostproc->setText( tr( "Skip calculation" ) );

    tabViews->changeTab( tabPostproc, tr( "Postprocess" ) );
    */
    
    // UI elements of 'Cell body' tab.
    TreeGenerationOptions DefCellBody;
    labelCellBodyHelpMsg->setText( QString(DefCellBody.HelpMsg) );
    pushCellBodyCellBody->setText( tr( "Process" ) );
        
    radioSkipCellBody->setText( tr( "Skip calculation" ) );
    //checkCellBodyCreateRecon->setText( tr( "Create reconstruction" ) );
    
    checkCellBodyWriteHOC->setText( tr( "Write HOC File" ) );
    labelCellBodyHOCFilename->setText( tr( "Filename:" ) );
    labelCellBodyHOCFilenameDisplay->setText( tr( "-" ) );
    toolButtonCellBodyHOC->setText( tr( "..." ) );
    
    checkCellBodyWriteDX->setText( tr( "Write DX" ) );
    labelCellBodyDXFilename->setText( tr( "Filename:" ) );
    labelCellBodyDXFilenameDisplay->setText( tr( "-" ) );
    toolButtonCellBodyDX->setText( tr( "..." ) );
    
    //labelCellBodyMaxLevel->setText( tr( "Max level:" ) );
    //lineCellBodyMaxLevel->setText( tr( "1024" ) );
   
    labelCellBodyThreshold->setText( tr( "Min dist. f. body:" ) );
    lineCellBodyThreshold->setText( tr( "20.0" ) );
    //labelCellBodyThreshold->setText( tr( "Threshold:" ) );
    //lineCellBodyThreshold->setText( tr( "0.0" ) );
    
    labelCellBodyThresholdLevel->setText( tr( "Min Len. dendrite qm:" ) );
    lineCellBodyThresholdLevel->setText( tr( "5.0" ) );
    //labelCellBodyThresholdLevel->setText( tr( "Threshold level:" ) );
    //lineCellBodyThresholdLevel->setText( tr( "0.1" ) );
    
    tabViews->changeTab( tabCellBody, tr( "Mark cell body" ) );
    
    // UI elements of 'Detect terminations' tab.
    labelDetectTermHelpMsg->setText( tr("The marks show where the algorithm has found dendrite terminations.You can \nadd one by pressing the left mouse button and the Shift key.(Erase one by using \nthe Control key) Bright: neuron, Grey: not detected structure") );
    pushDetectTermDetectTerm->setText( tr( "Create reconstruction" ) );
    
    boxDetectTermDisplayType->clear();
    boxDetectTermDisplayType->insertItem( tr( "Origin" ) );
    boxDetectTermDisplayType->insertItem( tr( "Tranformed" ) );
    boxDetectTermDisplayType->insertItem( tr( "Filtered" ) );
    boxDetectTermDisplayType->insertItem( tr( "Segmented" ) );
    boxDetectTermDisplayType->insertItem( tr( "Filtered+Segmented" ) );
    boxDetectTermDisplayType->insertItem( tr( "Final" ) );
    boxDetectTermDisplayType->insertItem( tr( "Preprocessed" ) );
    boxDetectTermDisplayType->insertItem( tr( "Compplot" ) );
    boxDetectTermDisplayType->insertItem( tr( "Recplot" ) );
    boxDetectTermDisplayType->insertItem( tr( "PREPROCMAXINT5" ) );
    boxDetectTermDisplayType->insertItem( tr( "PREPROCMAXINT10" ) );
    boxDetectTermDisplayType->insertItem( tr( "ORIGINMAXINT5" ) );
    boxDetectTermDisplayType->insertItem( tr( "ORIGINMAXINT10" ) );
    boxDetectTermDisplayType->insertItem( tr( "COMPMAXINT5" ) );
    boxDetectTermDisplayType->insertItem( tr( "COMPMAXINT10" ) );
    
    
    labelDetectTermDisplayType->setText( tr( "Display:" ) );
    
    radioSkipDetectTerm->setText( tr( "Skip calculation" ) );

    tabViews->changeTab( tabDetectTerm, tr( "Detect terminations" ) );
    
    // UI elements of 'Final' tab.
    labelFinalHelpMsg->setText( tr("This plot shows you the final reconstruction.") );
    pushFinalFinal->setText( tr( "Final" ) );
    
    radioSkipFinal->setText( tr( "Skip calculation" ) );

    tabViews->changeTab( tabFinal, tr( "Final" ) );
    
    
}


/*
        
    setCentralWidget(m_pViewer);

*/

void nraMainWindow::OnAddMark_View(int iMarkType, double dblX, double dblY)
{
    // Check for loading widget and set marktype according to combobox
    if(tabViews->isTabEnabled(tabLoading))
    {
        emit AddMark(boxLoadMarkType->currentItem(), dblX, dblY);
    }
    else
    {
        emit AddMark(iMarkType, dblX, dblY);
    }
}
void nraMainWindow::OnDestroyMark_View(int iMarkTag)
{
    emit DestroyMark(iMarkTag);
}
void nraMainWindow::OnMoveMark_View(int iMarkTag, double dblX, double dblY)
{
    emit MoveMark(iMarkTag, dblX, dblY);
}
void nraMainWindow::OnSetLabel_View(int iMarkTag, nraSharedArray<char> Label)
{
	emit SetLabel(iMarkTag, Label);
}
void nraMainWindow::OnLoadButtonClicked()
{
    LoadingOptions* RecOpt = new LoadingOptions;
    nraSharedPointer<Options> Opt(RecOpt);
    if(radioLoadTest->isOn())
    {
        RecOpt->filetype = TEST;
        emit Execute(Opt);
    }
    else
    {
        QString Result = QFileDialog::getOpenFileName(
                            QDir::homeDirPath(),
                            "Datacubes (*.cfd *.CFD *.xml *.XML *.nih *.NIH *.tiff *.TIFF *.tif *.TIF)",
                            this,
                            "Open datacube",
                            "Choose a file");
        if(checkLoadMultipleFiles->isChecked())
        {
            RecOpt->MultipleFiles = YES;
            RecOpt->StartNr = lineLoadBehStart->text().toInt();
            RecOpt->EndNr = lineLoadBehEnd->text().toInt();
        }
        if(checkLoadExtractVol->isChecked())
        {
            RecOpt->extractVolume = YES;
            RecOpt->start[0] = lineLoadVolStartX->text().toInt();
            RecOpt->start[1] = lineLoadVolStartY->text().toInt();
            RecOpt->start[2] = lineLoadVolStartZ->text().toInt();
        
            RecOpt->size[0] = lineLoadVolSizeX->text().toInt();
            RecOpt->size[1] = lineLoadVolSizeY->text().toInt();
            RecOpt->size[2] = lineLoadVolSizeZ->text().toInt();
        }
        RecOpt->channel = lineLoadChannel->text().toInt();

        if( Result.right(3) == "cfd" || Result.right(3) == "CFD" )
        {
            RecOpt->filetype = CFD;
            qstrcpy(RecOpt->FileName, Result);
            emit Execute(Opt);
            
        }
        else if( Result.right(3) == "xml" || Result.right(3) == "XML" )
        {
            RecOpt->filetype = XML;
            qstrcpy(RecOpt->FileName, Result);
            emit Execute(Opt);
            
        }
        else if( Result.right(3) == "nih" || Result.right(3) == "NIH" )
        {
            RecOpt->filetype = NIH;
            qstrcpy(RecOpt->FileName, Result);
            emit Execute(Opt);
        }
        else if( Result.right(3) == "tiff" || Result.right(3) == "TIFF" || Result.right(3) == "tif" || Result.right(3) == "TIF")
        {
            RecOpt->filetype = TIFF;
            qstrcpy(RecOpt->FileName, Result);
            emit Execute(Opt);
        }
        
        
        
        
    }
}

void nraMainWindow::OnPreprocButtonClicked()
{
    PreProcessOptions* RecOpt = new PreProcessOptions;
    nraSharedPointer<Options> Opt(RecOpt);

    if(radioSkipPreProc->isOn())
        RecOpt->skip = YES;
    else
        RecOpt->skip = NO;
    if(checkPreprocDelBound->isChecked())
        RecOpt->deleteBoundary = YES;
    else
        RecOpt->deleteBoundary = NO;
    if(checkPreprocErasePipet->isChecked())
        RecOpt->erasePipet = YES;
    else
        RecOpt->erasePipet = NO;
    
    emit Execute(Opt);
}

void nraMainWindow::OnTrafoButtonClicked()
{
    TransformOptions* RecOpt = new TransformOptions;
    nraSharedPointer<Options> Opt(RecOpt);

    if(radioSkipTrafo->isOn())
        RecOpt->skip = YES;
    else
        RecOpt->skip = NO;
    RecOpt->ThresholdLow = lineTrafoThreshLow->text().toDouble();
    RecOpt->ThresholdHigh = lineTrafoThreshHigh->text().toDouble();
    
    emit Execute(Opt);
        
}

void nraMainWindow::OnFFilterButtonClicked()
{
    RecFilterOptions* RecOpt = new RecFilterOptions;
    nraSharedPointer<Options> Opt(RecOpt);

    if(radioSkipFFilter->isOn())
        RecOpt->skip = YES;
    else
        RecOpt->skip = NO;
    RecOpt->tau = lineFFilterTau->text().toDouble();
    RecOpt->time_steps = lineFFilterTSteps->text().toInt();
    RecOpt->epsilon = lineFFilterEpsilon->text().toDouble();
    RecOpt->levels = lineFFilterLevels->text().toInt();
    RecOpt->filterLevel = lineFFilterFilterLevel->text().toInt();
    RecOpt->anicoeff1 = lineFFilterAnisoCoeff1->text().toDouble();
    RecOpt->anicoeff2 = lineFFilterAnisoCoeff2->text().toDouble();
    RecOpt->anicoeff3 = lineFFilterAnisoCoeff3->text().toDouble();
    RecOpt->lambda = lineFFilterLambda->text().toDouble();
    RecOpt->integration_size_x = lineFFilterIntSize->text().toInt();
    RecOpt->integration_size_y = lineFFilterIntSize->text().toInt();
    RecOpt->integration_size_z = lineFFilterIntSize->text().toInt();
    /* Deactivated until new filteroptions are ready.
    switch(boxFFilterSolverType->currentItem())
    {
        case 0:
            Options.small_boundary_pixels = 0;
            break;
        case 1:
            Options.small_boundary_pixels = 1;
            break;
    }
    switch(boxFFilterDiffusion->currentItem())
    {
        case 0:
            Options.nonlinear = 0;  
            break;
        case 1:
            Options.nonlinear = 1;
            break;
    }
    */
    switch(boxFFilterNonlinType->currentItem())
    {
        case 0:
            RecOpt->dependence_type = PERONA_MALIK;
            break;
        case 1:
	    RecOpt->dependence_type = WEIKERT;
            break;
        case 2:
            RecOpt->dependence_type = BLACK_SAPIRO;
            break;
    }
    switch(boxFFilterKindOfMom->currentItem())
    {
        case 0:
            //Options.gt = NODEWISE;            
            break;
        case 1:
            //Options.gt = ELEMENTWISE;
            break;
    }
    switch(boxFFilterIntPoint->currentItem())
    {
        case 0:
            RecOpt->ip_flag = IP_USUAL;
            break;
        case 1:
            RecOpt->ip_flag = IP_BND;
            break;
    }
    emit Execute(Opt);

}    

void nraMainWindow::OnFSegmButtonClicked()
{
    SegmentationOptions* RecOpt = new SegmentationOptions;
    nraSharedPointer<Options> Opt(RecOpt);
    
    if(radioSkipFSegm->isOn())
        RecOpt->skip = YES;
    else
        RecOpt->skip = NO;
    RecOpt->epsilon = lineFSegmEpsilon->text().toDouble();
    RecOpt->absoluteHighThreshold = lineFSegmAbsHighThresh->text().toDouble();
    RecOpt->absoluteLowThreshold = lineFSegmAbsLowThresh->text().toDouble();

    switch(boxFSegmSegmType->currentItem())
    {
        case 0:
        	RecOpt->segmentationType = LocalMeanValueAndNeigbours;
            break;
        case 1:
            RecOpt->segmentationType = GlobalThresholding;            
            break;
        case 2:
            RecOpt->segmentationType = Otsu;
            break;        
        case 3:
            RecOpt->segmentationType = FastMarching;
            break;
    }
    emit Execute(Opt);
 }

void nraMainWindow::OnSFilterButtonClicked()
{
    RecFilterOptions* RecOpt = new RecFilterOptions;
    nraSharedPointer<Options> Opt(RecOpt);

    if(radioSkipSFilter->isOn())
        RecOpt->skip = YES;
    else
        RecOpt->skip = NO;
    RecOpt->tau = lineSFilterTau->text().toDouble();
    RecOpt->time_steps = lineSFilterTSteps->text().toInt();
    RecOpt->epsilon = lineSFilterEpsilon->text().toDouble();
    RecOpt->levels = lineSFilterLevels->text().toInt();
    RecOpt->filterLevel = lineSFilterFilterLevel->text().toInt();
    RecOpt->anicoeff1 = lineSFilterAnisoCoeff1->text().toDouble();
    RecOpt->anicoeff2 = lineSFilterAnisoCoeff2->text().toDouble();
    RecOpt->anicoeff3 = lineSFilterAnisoCoeff3->text().toDouble();
    RecOpt->lambda = lineSFilterLambda->text().toDouble();
    RecOpt->integration_size_x = lineSFilterIntSize->text().toInt();
    RecOpt->integration_size_y = lineSFilterIntSize->text().toInt();
    RecOpt->integration_size_z = lineSFilterIntSize->text().toInt();
    /* Deactivated until new filteroptions are ready.
    switch(boxSFilterSolverType->currentItem())
    {
        case 0:
            Options.small_boundary_pixels = 0;
            break;
        case 1:
            Options.small_boundary_pixels = 1;
            break;
    }
    switch(boxSFilterDiffusion->currentItem())
    {
        case 0:
            Options.nonlinear = 0;  
            break;
        case 1:
            Options.nonlinear = 1;
            break;
    }
    */
    switch(boxSFilterNonlinType->currentItem())
    {
        case 0:
            RecOpt->dependence_type = PERONA_MALIK;
            break;
        case 1:
            RecOpt->dependence_type = WEIKERT;
            break;
        case 2:
            RecOpt->dependence_type = BLACK_SAPIRO;
            break;
    }
    switch(boxSFilterKindOfMom->currentItem())
    {
        case 0:
            //Options.gt = NODEWISE;            
            break;
        case 1:
            //Options.gt = ELEMENTWISE;
            break;
    }
    switch(boxSFilterIntPoint->currentItem())
    {
        case 0:
            RecOpt->ip_flag = IP_USUAL;
            break;
        case 1:
            RecOpt->ip_flag = IP_BND;
            break;
    }
    emit Execute(Opt);

}

void nraMainWindow::OnSSegmButtonClicked()
{
    SegmentationOptions* RecOpt = new SegmentationOptions;
    nraSharedPointer<Options> Opt(RecOpt);
    
    if(radioSkipSSegm->isOn())
        RecOpt->skip = YES;
    else
        RecOpt->skip = NO;
    RecOpt->epsilon = lineSSegmEpsilon->text().toDouble();
    RecOpt->absoluteHighThreshold = lineSSegmAbsHighThresh->text().toDouble();
    RecOpt->absoluteLowThreshold = lineSSegmAbsLowThresh->text().toDouble();

    switch(boxSSegmSegmType->currentItem())
    {
        case 0:
        	RecOpt->segmentationType = LocalMeanValueAndNeigbours;
            break;
        case 1:
            RecOpt->segmentationType = GlobalThresholding;
            break;
        case 2:
            RecOpt->segmentationType = Otsu;
            break;
        case 3:
            RecOpt->segmentationType = FastMarching;
            break;
    }
    emit Execute(Opt);

}

void nraMainWindow::OnCellBodySaveHOCClicked()
{
    
    QFileDialog* FileDialog = new QFileDialog( this, "file dialog", TRUE );
    FileDialog->setMode( QFileDialog::AnyFile );
    FileDialog->addFilter("HOC (*.hoc *.HOC)");
    
    FileDialog->setDir(QDir::homeDirPath());
    FileDialog->setCaption("HOC Filename");
    
    QString Result;
    if ( FileDialog->exec() == QDialog::Accepted )
        Result = FileDialog->selectedFile();
    
    if( Result.right(3) == "hoc" || Result.right(3) == "HOC" )
    {
        labelCellBodyHOCFilenameDisplay->setText(Result);
    }
    else
    {
        labelCellBodyHOCFilenameDisplay->setText("-");
    }

}

void nraMainWindow::OnCellBodySaveDXClicked()       
{
    QFileDialog* FileDialog = new QFileDialog( this, "file dialog", TRUE );
    FileDialog->setMode( QFileDialog::AnyFile );
    FileDialog->addFilter("DX (*.dx *.DX)");
    
    FileDialog->setDir(QDir::homeDirPath());
    FileDialog->setCaption("DX Filename");
    
    QString Result;
    if ( FileDialog->exec() == QDialog::Accepted )
        Result = FileDialog->selectedFile();
    
    if( Result.right(2) == "dx" || Result.right(2) == "DX" )
    {
        labelCellBodyDXFilenameDisplay->setText(Result);
    }
    else
    {
        labelCellBodyDXFilenameDisplay->setText("-");
    }

}

void nraMainWindow::OnTreeGenButtonClicked()
{
    TreeGenerationOptions* RecOpt = new TreeGenerationOptions;
    nraSharedPointer<Options> Opt(RecOpt);
    
    if(radioSkipCellBody->isOn())
        RecOpt->skip = YES;
    else
        RecOpt->skip = NO;
    
    /*
    if(checkCellBodyCreateRecon->isChecked())
    {
        Options.createReconstr = YES;
    }
    else
    {
        Options.createReconstr = NO;
    }
    */
    RecOpt->createReconstr = YES;
    
    
    if(checkCellBodyWriteHOC->isChecked() && labelCellBodyHOCFilenameDisplay->text() != "-")
    {
        RecOpt->writeHocFile = YES;
        qstrcpy(RecOpt->hocFileName, labelCellBodyHOCFilenameDisplay->text());
    }
    else
    {
        RecOpt->writeHocFile = NO;
    }

    if(checkCellBodyWriteDX->isChecked() && labelCellBodyDXFilenameDisplay->text() != "-")
    {
        RecOpt->writeDX = YES;
        qstrcpy(RecOpt->dxFileName, labelCellBodyDXFilenameDisplay->text());
    }
    else
    {
        RecOpt->writeDX = NO;
    }
    //Options.maxlevel = lineCellBodyMaxLevel->text().toInt();
    //Options.thresholdlevel = lineCellBodyThresholdLevel->text().toInt();
    //Options.threshold = lineCellBodyThreshold->text().toDouble();
    RecOpt->minLengthOfDendriteInqm=lineCellBodyThresholdLevel->text().toDouble();
    RecOpt->minDistanceFromBodyCompartment=lineCellBodyThreshold->text().toDouble();

    
    emit Execute(Opt);
}

void nraMainWindow::OnDetectTermButtonClicked()
{
    std::cout << "Term" << std::endl;
    nraSharedPointer<Options> Opt(new Options);
    emit Execute(Opt);
}

void nraMainWindow::OnSaveButtonClicked()
{
	
    //CFD, XML, NIH, RAW, VREND, TEST, TIFF
    QFileDialog* FileDialog = new QFileDialog( this, "file dialog", TRUE );
    FileDialog->setMode( QFileDialog::AnyFile );
    FileDialog->addFilter("RAW (*.raw *.RAW)");
    FileDialog->addFilter("VREND (*.vrd *.VRD)");
    FileDialog->addFilter("XML (*.xml *.XML)");
    FileDialog->addFilter("NIH (*.nih *.NIH)");
    FileDialog->addFilter("TIFF (*.tiff *.TIFF)");
    FileDialog->addFilter("TIFF (*.tif *.TIF)");
    FileDialog->setDir(QDir::homeDirPath());
    FileDialog->setCaption("Save datacube");
    
    QString Result;
    if ( FileDialog->exec() == QDialog::Accepted )
        Result = FileDialog->selectedFile();

    char* cFileName = new char[Result.length()];
    qstrcpy(cFileName, Result);
    
    if( Result.right(3) == "raw" || Result.right(3) == "RAW" )
    {
        emit Save(cFileName, RAW);
    }
    else if( Result.right(3) == "vrd" || Result.right(3) == "VRD" )
    {
        emit Save(cFileName, VREND);
    }    
    else if( Result.right(3) == "xml" || Result.right(3) == "XML" )
    {
        emit Save(cFileName, XML);
    }
    else if( Result.right(3) == "nih" || Result.right(3) == "NIH" )
    {
	std::cout << "Emit saving nih" << std::endl;
	emit Save(cFileName, NIH);
    }
    else if( Result.right(4) == "tiff" || Result.right(4) == "TIFF" || Result.right(3) == "tif" || Result.right(3) == "TIF")
    {
   	std::cout << "Emit Signal for saving tif!" << std::endl;
        emit Save(cFileName, TIFF);
    }
    
    delete [] cFileName;
}

void nraMainWindow::OnPrefButtonClicked()
{
	emit SetPrefs();
}

void nraMainWindow::OnAddMark(int iMarkTag, int iMarkType, bool boolLabel, double dblX, double dblY)
{
    emit AddMark_View(iMarkTag, iMarkType, boolLabel, dblX, dblY);
}
void nraMainWindow::OnDestroyMark(int iMarkTag)
{
    emit DestroyMark_View(iMarkTag);
}
void nraMainWindow::OnMoveMark(int iMarkTag, double dblX, double dblY)
{
    emit MoveMark_View(iMarkTag, dblX, dblY);
}
void nraMainWindow::OnSetLabel(int iMarkTag, nraSharedArray<char> Label)
{
	emit SetLabel_View(iMarkTag, Label);
}

void nraMainWindow::OnSetSlider(int iSliderMin, int iSliderMax, int iSliderVal)
{
    QString SliderLabel;
    SliderLabel.setNum(iSliderVal);
    sliderSlice->blockSignals(true);
    sliderSlice->setMinValue(iSliderMin);
    sliderSlice->setMaxValue(iSliderMax);
    sliderSlice->setValue(iSliderVal);
    labelSlider->setText(SliderLabel);    
    sliderSlice->blockSignals(false);
}

void nraMainWindow::OnSetSlice(int iSliceNmbr)
{
    emit SetSlice_View(iSliceNmbr);
}

void nraMainWindow::OnSetStack(nraSharedPointer<ViewCube> Stack)
{
    sliderSlice->setEnabled(true);
    OnSetSlider(1, Stack->GetSizeZ(), 1);
    emit SetStack_View(Stack);
}

void nraMainWindow::OnDisable()
{
    sliderSlice->blockSignals(true);
    sliderSlice->setValue(sliderSlice->minValue());
    sliderSlice->setEnabled(false);
    sliderSlice->blockSignals(false);
    labelSlider->setText(tr("-"));
    emit Disable_View();
}

void nraMainWindow::OnSliderValueChanged(int iValue)
{
    QString SliderLabel;
    SliderLabel.setNum(iValue);
    labelSlider->setText(SliderLabel);
    emit SetSlice(iValue);
}

void nraMainWindow::OnSetDisplayType(int iDisplayType)
{
    emit SetDisplayType(iDisplayType);
}

void nraMainWindow::OnProgress(int iPercentage)
{
    progressBar->setProgress(iPercentage);
    progressBar->update();
}

// Option slots
void nraMainWindow::OnLoadTestToggled(bool boolValue)
{
    groupLoadBehaviour->setEnabled(!boolValue);
    groupLoadVolume->setEnabled(!boolValue);
    lineLoadChannel->setEnabled(!boolValue);
    labelLoadChannel->setEnabled(!boolValue);
}
void nraMainWindow::OnLoadMulFilesToggled(bool boolValue)
{
    labelLoadBehStart->setEnabled(boolValue);
    labelLoadBehEnd->setEnabled(boolValue);
    lineLoadBehStart->setEnabled(boolValue);
    lineLoadBehEnd->setEnabled(boolValue);
}
void nraMainWindow::OnLoadExtrVolToggled(bool boolValue)
{
    labelLoadVolStartX->setEnabled(boolValue);
    labelLoadVolStartY->setEnabled(boolValue);
    labelLoadVolStartZ->setEnabled(boolValue);
    labelLoadVolSizeY->setEnabled(boolValue);
    labelLoadVolSizeX->setEnabled(boolValue);
    labelLoadVolSizeZ->setEnabled(boolValue);
    labelLoadVolSize->setEnabled(boolValue);
    lineLoadVolSizeX->setEnabled(boolValue);
    lineLoadVolSizeY->setEnabled(boolValue);
    lineLoadVolSizeZ->setEnabled(boolValue);
    labelLoadVolStart->setEnabled(boolValue);
    lineLoadVolStartX->setEnabled(boolValue);
    lineLoadVolStartY->setEnabled(boolValue);
    lineLoadVolStartZ->setEnabled(boolValue);

}

void nraMainWindow::ConnectView(nraCubeView* pViewer)
{
    // Connect viewer widget.
    connect(pViewer,	SIGNAL	(AddMark(int, double, double)),
            this,		SLOT	(OnAddMark_View(int, double, double)));
    connect(pViewer,	SIGNAL	(DestroyMark(int)),
            this,		SLOT	(OnDestroyMark_View(int)));
    connect(pViewer,	SIGNAL	(MoveMark(int, double, double)),
            this,		SLOT	(OnMoveMark_View(int, double, double)));
    connect(pViewer,	SIGNAL	(SetLabel(int, nraSharedArray<char>)),
            this,		SLOT	(OnSetLabel_View(int, nraSharedArray<char>)));

    connect(this,		SIGNAL	(AddMark_View(int, int, bool, double, double)),
            pViewer,	SLOT	(OnAddMark(int, int, bool, double, double)));
    connect(this,		SIGNAL	(DestroyMark_View(int)),
            pViewer,	SLOT	(OnDestroyMark(int)));
    connect(this,		SIGNAL	(MoveMark_View(int, double, double)),
            pViewer,	SLOT	(OnMoveMark(int, double, double)));
    connect(this,		SIGNAL	(SetLabel_View(int, nraSharedArray<char>)),
            pViewer,	SLOT	(OnSetLabel(int, nraSharedArray<char>)));
    connect(this,		SIGNAL	(SetSlice_View(int)),
            pViewer,	SLOT	(OnSetSlice(int)));
    connect(this,		SIGNAL	(SetStack_View(nraSharedPointer<ViewCube>)),
            pViewer,	SLOT	(OnSetStack(nraSharedPointer<ViewCube>)));            
/*
    connect(this,		SIGNAL	(Disable_View()),
            pViewer,	SLOT	(OnDisable()));
*/

}
void nraMainWindow::DisconnectView(nraCubeView* pViewer)
{
    disconnect( pViewer, 0, this, 0 );
    disconnect( this, 0, pViewer, 0 );
}

void nraMainWindow::OnTabFlip(QWidget* pTabWidget)
{
    DisconnectView(m_pCurrentView);
    switch(tabViews->currentPageIndex())
    {
        case 0:
            m_pCurrentView = viewLoading;
            ConnectView(viewLoading);
            emit TabFlip(1);
            break;
        case 1:
            m_pCurrentView = viewTrafo;
            ConnectView(viewTrafo);
            emit TabFlip(2);
            break;
        case 2:
            m_pCurrentView = viewFFilter;
            ConnectView(viewFFilter);
            emit TabFlip(3);
            break;
        case 3:
            m_pCurrentView = viewFSegm;
            ConnectView(viewFSegm);
            emit TabFlip(4);
            break;
        case 4:
            m_pCurrentView = viewSFilter;
            ConnectView(viewSFilter);
            emit TabFlip(5);
            break;
        case 5:
            m_pCurrentView = viewSSegm;
            ConnectView(viewSSegm);
            emit TabFlip(6);
            break;
        /*
        case 7:
            m_pCurrentView = viewPostproc;
            ConnectView(viewPostproc);
            emit TabFlip(8);
            break;
        */
        case 6:
            m_pCurrentView = viewPreproc;
            ConnectView(viewPreproc);
            emit TabFlip(7);
            break;
        case 7:
            m_pCurrentView = viewCellBody;
            ConnectView(viewCellBody);
            emit TabFlip(8);
            break;
        case 8:
            m_pCurrentView = viewDetectTerm;
            ConnectView(viewDetectTerm);
            emit TabFlip(9);
            break;
        case 9:
            m_pCurrentView = viewFinal;
            ConnectView(viewFinal);
            emit TabFlip(10);
            break;
        
    }
}
