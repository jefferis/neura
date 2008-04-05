/****************************************************************************
** Form implementation generated from reading ui file 'nraDlgMarkLabel.ui'
**
** Created: Sun Feb 15 20:20:08 2004
**      by: The User Interface Compiler ($Id: nraDlgMarkLabel.cpp,v 1.1 2004/05/11 13:21:06 alexander Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "nraDlgMarkLabel.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a nraDlgMarkLabel as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
nraDlgMarkLabel::nraDlgMarkLabel( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "nraDlgMarkLabel" );
    setSizeGripEnabled( TRUE );

    QWidget* privateLayoutWidget = new QWidget( this, "Layout1" );
    privateLayoutWidget->setGeometry( QRect( 10, 90, 220, 33 ) );
    Layout1 = new QHBoxLayout( privateLayoutWidget, 0, 6, "Layout1"); 
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer );

    buttonOk = new QPushButton( privateLayoutWidget, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    lineLabel = new QLineEdit( this, "lineLabel" );
    lineLabel->setGeometry( QRect( 10, 50, 220, 21 ) );

    labelMessage = new QLabel( this, "labelMessage" );
    labelMessage->setGeometry( QRect( 10, 20, 230, 20 ) );
    languageChange();
    resize( QSize(242, 134).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
nraDlgMarkLabel::~nraDlgMarkLabel()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void nraDlgMarkLabel::languageChange()
{
    setCaption( tr( "Mark Label" ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    labelMessage->setText( tr( "Here you can add a label to the mark." ) );
}

