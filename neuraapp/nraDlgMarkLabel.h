/****************************************************************************
** Form interface generated from reading ui file 'nraDlgMarkLabel.ui'
**
** Created: Sun Feb 15 20:18:26 2004
**      by: The User Interface Compiler ($Id: nraDlgMarkLabel.h,v 1.1 2004/05/11 13:21:06 alexander Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef NRADLGMARKLABEL_H
#define NRADLGMARKLABEL_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QPushButton;
class QLineEdit;
class QLabel;

class nraDlgMarkLabel : public QDialog
{
    Q_OBJECT

public:
    nraDlgMarkLabel( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~nraDlgMarkLabel();

    QPushButton* buttonOk;
    QLineEdit* lineLabel;
    QLabel* labelMessage;

protected:
    QHBoxLayout* Layout1;

protected slots:
    virtual void languageChange();

};

#endif // NRADLGMARKLABEL_H
