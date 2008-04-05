/****************************************************************************
** Form implementation generated from reading ui file 'nraDlgPreferences.ui'
**
** Created: Fri Jan 16 13:26:19 2004
**      by: The User Interface Compiler ($Id: nraDlgPreferences.cpp,v 1.2 2004/01/23 15:18:14 alexander Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "nraDlgPreferences.h"

#include <qvariant.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qstring.h>
#include <qslider.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a PreferenceDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
PreferenceDialog::PreferenceDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "PreferenceDialog" );
    setSizeGripEnabled( TRUE );

    groupBrightContr = new QGroupBox( this, "groupBrightContr" );
    groupBrightContr->setGeometry( QRect( 10, 20, 410, 200 ) );
    groupBrightContr->setCheckable( FALSE );

    labelContrast = new QLabel( groupBrightContr, "labelContrast" );
    labelContrast->setGeometry( QRect( 20, 100, 60, 21 ) );

    labelBrightness = new QLabel( groupBrightContr, "labelBrightness" );
    labelBrightness->setGeometry( QRect( 20, 30, 70, 21 ) );

    labelDisplContrast = new QLabel( groupBrightContr, "labelDisplContrast" );
    labelDisplContrast->setGeometry( QRect( 320, 130, 61, 21 ) );
    labelDisplContrast->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    labelDisplContrast->setFrameShape( QLabel::Box );
    labelDisplContrast->setFrameShadow( QLabel::Sunken );

    labelDisplBrightness = new QLabel( groupBrightContr, "labelDisplBrightness" );
    labelDisplBrightness->setGeometry( QRect( 320, 60, 61, 21 ) );
    labelDisplBrightness->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    labelDisplBrightness->setFrameShape( QLabel::Box );
    labelDisplBrightness->setFrameShadow( QLabel::Sunken );

    sliderContrast = new QSlider( groupBrightContr, "sliderContrast" );
    sliderContrast->setGeometry( QRect( 20, 130, 280, 21 ) );
    sliderContrast->setMaxValue( 100 );
    sliderContrast->setOrientation( QSlider::Horizontal );
    sliderContrast->setTickmarks( QSlider::Below );
    sliderContrast->setTickInterval( 10 );
	sliderContrast->setValue( 50 );

    sliderBrightness = new QSlider( groupBrightContr, "sliderBrightness" );
    sliderBrightness->setGeometry( QRect( 20, 60, 280, 21 ) );
    sliderBrightness->setMaxValue( 100 );
    sliderBrightness->setOrientation( QSlider::Horizontal );
    sliderBrightness->setTickmarks( QSlider::Below );
    sliderBrightness->setTickInterval( 10 );
    sliderBrightness->setValue( 50 );

    QWidget* privateLayoutWidget = new QWidget( this, "Layout1" );
    privateLayoutWidget->setGeometry( QRect( 10, 230, 410, 33 ) );
    Layout1 = new QHBoxLayout( privateLayoutWidget, 0, 6, "Layout1"); 
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer );

    buttonOk = new QPushButton( privateLayoutWidget, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );
    languageChange();
    resize( QSize(426, 271).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

	

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    
    connect(sliderBrightness,	SIGNAL	(valueChanged(int)),
            this,				SLOT	(OnBrightnessValueChanged(int)));
    connect(sliderContrast,		SIGNAL	(valueChanged(int)),
            this,				SLOT	(OnContrastValueChanged(int)));

    
}

/*
 *  Destroys the object and frees any allocated resources
 */
PreferenceDialog::~PreferenceDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void PreferenceDialog::languageChange()
{
    setCaption( tr( "Preferences" ) );
    groupBrightContr->setTitle( tr( "Brightness and Contrast" ) );
    labelContrast->setText( tr( "Contrast" ) );
    labelBrightness->setText( tr( "Brightness" ) );
    labelDisplContrast->setText( tr( "50" ) );
    labelDisplBrightness->setText( tr( "50" ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
}

void PreferenceDialog::OnBrightnessValueChanged(int iValue)
{
	QString BrightnessLabel;
    BrightnessLabel.setNum(iValue);
    labelDisplBrightness->setText(BrightnessLabel);
}

void PreferenceDialog::OnContrastValueChanged(int iValue)
{
    QString ContrastLabel;
    ContrastLabel.setNum(iValue);
    labelDisplContrast->setText(ContrastLabel);
}


