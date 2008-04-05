/****************************************************************************
** Form interface generated from reading ui file 'nraDlgPreferences.ui'
**
** Created: Fri Jan 16 13:26:13 2004
**      by: The User Interface Compiler ($Id: nraDlgPreferences.h,v 1.1 2004/01/23 14:55:45 alexander Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QGroupBox;
class QLabel;
class QSlider;
class QPushButton;

class PreferenceDialog : public QDialog
{
    Q_OBJECT

public:
    PreferenceDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~PreferenceDialog();

    QGroupBox* groupBrightContr;
    QLabel* labelContrast;
    QLabel* labelBrightness;
    QLabel* labelDisplContrast;
    QLabel* labelDisplBrightness;
    QSlider* sliderContrast;
    QSlider* sliderBrightness;
    QPushButton* buttonOk;

protected:
    QHBoxLayout* Layout1;

protected slots:
    virtual void languageChange();
    void OnBrightnessValueChanged(int iValue);
    void OnContrastValueChanged(int iValue);

};

#endif // PREFERENCEDIALOG_H
