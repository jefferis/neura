/****************************************************************************
** Form interface generated from reading ui file 'nraDlgTIFFAlert.ui'
**
** Created: Tue Dec 16 11:55:10 2003
**      by: The User Interface Compiler ($Id: nraDlgTIFFAlert.h,v 1.1 2004/01/23 14:55:45 alexander Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef NRADLGTIFFALERT_H
#define NRADLGTIFFALERT_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QPushButton;
class QLineEdit;

class nraDlgTIFFAlert : public QDialog
{
    Q_OBJECT

public:
    nraDlgTIFFAlert( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~nraDlgTIFFAlert();

    QLabel* labelInfo;
    QPushButton* buttonOk;
    QLineEdit* lineGridX;
    QLabel* labelUnitY;
    QLineEdit* lineGridY;
    QLabel* labelX;
    QLineEdit* lineGridZ;
    QLabel* labelUnitX;
    QLabel* labelUnitZ;
    QLabel* labelY;
    QLabel* labelZ;

protected:
    QHBoxLayout* Layout1;

protected slots:
    virtual void languageChange();

};

#endif // NRADLGTIFFALERT_H
