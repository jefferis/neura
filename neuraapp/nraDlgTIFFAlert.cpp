/****************************************************************************
** Form implementation generated from reading ui file 'nraDlgTIFFAlert.ui'
**
** Created: Tue Dec 16 11:56:26 2003
**      by: The User Interface Compiler ($Id: nraDlgTIFFAlert.cpp,v 1.1 2004/01/23 14:55:45 alexander Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "nraDlgTIFFAlert.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a nraDlgTIFFAlert as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
nraDlgTIFFAlert::nraDlgTIFFAlert( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "nraDlgTIFFAlert" );
    setSizeGripEnabled( TRUE );
    setModal( TRUE );

    labelInfo = new QLabel( this, "labelInfo" );
    labelInfo->setGeometry( QRect( 10, 10, 440, 50 ) );
    labelInfo->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    labelInfo->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );

    QWidget* privateLayoutWidget = new QWidget( this, "Layout1" );
    privateLayoutWidget->setGeometry( QRect( 20, 240, 430, 33 ) );
    Layout1 = new QHBoxLayout( privateLayoutWidget, 0, 6, "Layout1"); 
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer );

    buttonOk = new QPushButton( privateLayoutWidget, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    lineGridX = new QLineEdit( this, "lineGridX" );
    lineGridX->setGeometry( QRect( 170, 100, 121, 20 ) );

    labelUnitY = new QLabel( this, "labelUnitY" );
    labelUnitY->setGeometry( QRect( 300, 140, 80, 21 ) );
    labelUnitY->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    lineGridY = new QLineEdit( this, "lineGridY" );
    lineGridY->setGeometry( QRect( 170, 140, 121, 20 ) );

    labelX = new QLabel( this, "labelX" );
    labelX->setGeometry( QRect( 60, 100, 80, 21 ) );
    labelX->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    lineGridZ = new QLineEdit( this, "lineGridZ" );
    lineGridZ->setGeometry( QRect( 170, 180, 121, 20 ) );

    labelUnitX = new QLabel( this, "labelUnitX" );
    labelUnitX->setGeometry( QRect( 300, 100, 80, 21 ) );
    labelUnitX->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    labelUnitZ = new QLabel( this, "labelUnitZ" );
    labelUnitZ->setGeometry( QRect( 300, 180, 80, 21 ) );
    labelUnitZ->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    labelY = new QLabel( this, "labelY" );
    labelY->setGeometry( QRect( 60, 140, 80, 21 ) );
    labelY->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    labelZ = new QLabel( this, "labelZ" );
    labelZ->setGeometry( QRect( 60, 180, 80, 21 ) );
    labelZ->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    languageChange();
    resize( QSize(458, 285).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
nraDlgTIFFAlert::~nraDlgTIFFAlert()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void nraDlgTIFFAlert::languageChange()
{
    setCaption( tr( "Set Gridspaceings" ) );
    labelInfo->setText( tr( "This TIFF - file does not contain information about its grid spaceings. Please enter the appropriate values." ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    labelUnitY->setText( tr( "micrometers" ) );
    labelX->setText( tr( "Spaceings X:" ) );
    labelUnitX->setText( tr( "micrometers" ) );
    labelUnitZ->setText( tr( "micrometers" ) );
    labelY->setText( tr( "Spaceings Y:" ) );
    labelZ->setText( tr( "Spaceings Z:" ) );
}

