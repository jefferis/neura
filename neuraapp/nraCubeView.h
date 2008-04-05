/*
 *  QView.h
 *  QTViewer02
 *
 *  Created by Alexander Heusel on Fri Mar 28 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _NRACUBEVIEW_H_
#define _NRACUBEVIEW_H_

#include <q3canvas.h>

#include "../reconstructionlib/reconstruction.h"
#include "../nraSmartPointer.h"

/*
Viewer class. Displays the marks that are stored in the model layer.
Design concept:
- Viewer gets user input (i.e. move, destroy or add mark)
- Viewer emits request signal (move, add, destroy, etc.)
- Model layer recieves these signals, checks for logic and emits
  corresponding signals to ALL views (move, add, destroy etc.)
This concept supports multiple views of the same model context and
is easily extensible to different marks.
All marks are tagged with a integer number by the model layer.
*/

class nraCubeView : public QCanvasView
{
    Q_OBJECT

public:
    nraCubeView(QCanvas&, QWidget* parent=0, const char* name=0, WFlags f=0);
    virtual ~nraCubeView();
    void clear();
    void setMarkType(int iMarkType) {m_iMarkType = iMarkType;};
    int markType() const {return m_iMarkType;};
    void setMarkSize(int iMarkSize) {m_iMarkSize = iMarkSize;};
    int markSize() const {return m_iMarkSize;};


public slots:
    // Slots for manipulation of mark views.
    void OnAddMark(int iMarkTag, int iMarkType, bool boolLabel, double dblX, double dblY);
    void OnDestroyMark(int iMarkTag);
    void OnMoveMark(int iMarkTag, double dblX, double dblY);
    void OnSetLabel(int iMarkTag, nraSharedArray<char> Label);
    void OnSetSlice(int iSliceNmbr);
    void OnSetStack(nraSharedPointer<ViewCube> Stack);
    void OnDisable();
    
    
signals:
 
    // Model signals
    void AddMark(int iMarktType, double dblX, double dblY);
    void DestroyMark(int iMarkTag);
    void MoveMark(int iMarkTag, double dblX, double dblY);
	void SetLabel(int iMarkTag, nraSharedArray<char> Label);


protected:
    void contentsMousePressEvent(QMouseEvent*);
	void contentsMouseReleaseEvent(QMouseEvent*);
    void contentsMouseMoveEvent(QMouseEvent*);
    void contentsMouseDoubleClickEvent(QMouseEvent*);    

private:
	int m_iMarkType;
	int m_iMarkSize;
    QPoint m_StartPosition;
    QCanvasItem* m_pHotMark;
    QCanvasItem* m_pMoveMark;
    QCanvasItem* m_pAddedMark;
	QRgb* m_pLUT;
	    
    double CanvToNormX(int dblX);
    double CanvToNormY(int dblY);
    int NormToCanvX(double dblX);
    int NormToCanvY(double dblY);


	nraSharedPointer<ViewCube> m_Stack;

};


#endif
