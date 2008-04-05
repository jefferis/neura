#include <qapplication.h>
#include <qpainter.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qimage.h>

#include "nraCubeMark.h"
#include "nraDlgMarkLabel.h"
#include "nraCubeView.h"


#include <iostream>

nraCubeView::nraCubeView( QCanvas& c, QWidget* parent, const char* name, WFlags f) 
	: 	QCanvasView(&c,parent,name,f), 
		m_iMarkType(0),
		m_iMarkSize(25), 
		m_pHotMark(0L),
		m_pAddedMark(0L),
		m_pMoveMark(0L),
		m_pLUT(0L)
{
	
	// Create standard colortable for m_pImage.
	m_pLUT = new QRgb[256];
	for(long i = 0; i < 256; i++)
		m_pLUT[i] = qRgb(i,i,i);

}

nraCubeView::~nraCubeView()
{
	if(m_pLUT)
	{
		delete [] m_pLUT;
	}
}

void nraCubeView::contentsMousePressEvent(QMouseEvent* e)
{

	if(e->state() & (Qt::LeftButton | Qt::ShiftButton))
	{
		// Deactivate hot mark if existent
		if(m_pHotMark)
		{
			nraCubeMark* pHotMark = 0L;
			pHotMark = dynamic_cast<nraCubeMark*>(m_pHotMark);
			if(pHotMark)
			{
				pHotMark->setActive(false);
			}
			m_pHotMark = 0L;
		}
		
		// Request to add a new one.
		m_pAddedMark = 0L;
		m_StartPosition = inverseWorldMatrix().map(e->pos());
		emit AddMark(m_iMarkType, CanvToNormX(m_StartPosition.x()), CanvToNormY(m_StartPosition.y()));
		
		// Look into added mark for recently added mark.
		// In addition set new mark as movable.
		if(m_pAddedMark)
		{
			nraCubeMark* pAddedMark = 0L;
			pAddedMark = dynamic_cast<nraCubeMark*>(m_pAddedMark);
			pAddedMark->setActive(true);
			m_pHotMark = m_pAddedMark;
			m_pMoveMark = m_pHotMark;
		}
		
		
	}
    else if(e->state() & (Qt::LeftButton | Qt::ControlButton))
    {
    	// Find mark to destroy.
		nraCubeMark* pCubeMark = 0L;
    	QCanvasItemList list = canvas()->collisions(inverseWorldMatrix().map(e->pos()));
    	for (QCanvasItemList::Iterator it = list.begin(); it != list.end(); ++it)
    	{	
    		pCubeMark = dynamic_cast<nraCubeMark*>(*it);
    		if(pCubeMark)
    		{
    			break;
    		}
    	}
    	// Request to destroy it if there is one.
    	if(pCubeMark)
    	{
    		emit DestroyMark(pCubeMark->markID());
    	}
	}
	else
	{
		// Find mark to select.		
		nraCubeMark* pCubeMark = 0L;
		nraCubeMark* pHotMark = 0L;
    	pHotMark = dynamic_cast<nraCubeMark*>(m_pHotMark);
    	QCanvasItemList list = canvas()->collisions(inverseWorldMatrix().map(e->pos()));
    	for (QCanvasItemList::Iterator it = list.begin(); it != list.end(); ++it)
    	{	
    		pCubeMark = dynamic_cast<nraCubeMark*>(*it);
    		if(pCubeMark)
    		{
				break;
    		}
    	}
    	
    	// We clicked into something...
    	if(pCubeMark)
    	{
    		// ... we clicked on the hot mark. Set hot mark as move mark.
    		if(pCubeMark == pHotMark)
	    	{
	    		m_StartPosition = inverseWorldMatrix().map(e->pos());
	    		m_pMoveMark = m_pHotMark;
	    	}
	    	// ... we clicked on a different mark. Deactivate hot, activate other mark.
	    	else
	    	{
    			// If there is an active mark deactivate it and set new mark as active.
    			// In addition set new mark as movable.
    			if(pHotMark)
    			{
	    			pHotMark->setActive(false);
    			}
	    		pCubeMark->setActive(true);
	    		m_pHotMark = pCubeMark;
	    		m_StartPosition = inverseWorldMatrix().map(e->pos());
	    		m_pMoveMark = m_pHotMark;			
    		}
    		
    	}
    	// We clicked into empty space...
    	else
    	{
    		// ... if there is a hot mark deactivate it.
    		if(pHotMark)
    		{
	    		pHotMark->setActive(false);
    		}
    		m_pHotMark = 0L;
    	}
	}	    
    canvas()->update();
    
}

void nraCubeView::contentsMouseReleaseEvent(QMouseEvent* e)
{
	m_pMoveMark = 0L;
}

void nraCubeView::contentsMouseMoveEvent(QMouseEvent* e)
{
	
	if (m_pMoveMark) 
    {    		
    	nraCubeMark* pMoveMark = 0L;
    	pMoveMark = dynamic_cast<nraCubeMark*>(m_pMoveMark);
    	
     	// Calculate center of mark...
     	int iX = static_cast<int>(static_cast<double>(pMoveMark->x()) + static_cast<double>(pMoveMark->width())*0.5);
    	int iY = static_cast<int>(static_cast<double>(pMoveMark->y()) + static_cast<double>(pMoveMark->height())*0.5);
    	QPoint p = inverseWorldMatrix().map(e->pos());
    	QPoint Delta(p - m_StartPosition);
    	iX += Delta.x();
    	iY += Delta.y();
		emit MoveMark(pMoveMark->markID(), CanvToNormX(iX), CanvToNormY(iY));
		m_StartPosition = p; 
    }
}

void nraCubeView::contentsMouseDoubleClickEvent(QMouseEvent* e)
{

	// Find mark to label.
	nraCubeMark* pCubeMark = 0L;
	QCanvasItemList list = canvas()->collisions(inverseWorldMatrix().map(e->pos()));
	for (QCanvasItemList::Iterator it = list.begin(); it != list.end(); ++it)
	{	
	    pCubeMark = dynamic_cast<nraCubeMark*>(*it);
	    if(pCubeMark)
	    {
	    	break;
	    }
	}

	// Open dialog and ask for label if we clicked on something.
	// Check if cube mark accepts a label.
	if(pCubeMark)
	{
		if(pCubeMark->acceptsLabel())
		{
			nraDlgMarkLabel LabelDialog;
			LabelDialog.exec();
			
			nraSharedArray<char> Label;
			unsigned int uiLength = LabelDialog.lineLabel->text().length();
			
			if(uiLength > 0)
			{
				Label = nraSharedArray<char>(new char[uiLength], uiLength);
				qstrcpy(Label.GetPointer(), LabelDialog.lineLabel->text());
			}
			
			
			//Destroy old label.
			if(pCubeMark->label())
			{
				delete pCubeMark->label();
			}
			pCubeMark->setLabel(0L);
			
			// Place new label if there is a valid one.
			if(Label.IsValid())
			{	
				nraCubeMarkText* pLabel = new nraCubeMarkText(LabelDialog.lineLabel->text(), canvas());
				pLabel->pmoveBy(pCubeMark->x() + pCubeMark->width(), pCubeMark->y() + pCubeMark->height());
				pLabel->setColor(pCubeMark->pen().color());
				pLabel->show();
				pCubeMark->setLabel(pLabel);
			}
			
			// Tell modell layer that there is a new Layer.
			emit SetLabel(pCubeMark->markID(), Label);
			canvas()->update();
		}	
	}

}


void nraCubeView::clear()
{
    QCanvasItemList list = canvas()->allItems();
    for (QCanvasItemList::Iterator it = list.begin(); it != list.end(); ++it)
    {
        if (*it)
        {
            delete *it;
        }
    }
    m_pHotMark = 0L;
	m_pAddedMark = 0L;
	m_pMoveMark = 0L;
}

void nraCubeView::OnAddMark(int iMarkTag, int iMarkType, bool boolLabel, double dblX, double dblY)
{
	nraCubeMark* pCubeMark = new nraCubeMark(canvas(), iMarkType / 4, iMarkTag, boolLabel);
	switch(iMarkType % 4)
	{
		case 0:
			pCubeMark->setPen(QColor(0, 0, 255));
			break;
		case 1:
			pCubeMark->setPen(QColor(0, 255, 0));
			break;
		case 2:
			pCubeMark->setPen(QColor(255, 0, 0));
			break;
		case 3:
			pCubeMark->setPen(QColor(255, 255, 0));
			break;
	}
	pCubeMark->setActive(false);
	pCubeMark->setSize(m_iMarkSize, m_iMarkSize);
	dblX -= CanvToNormX(static_cast<int>(static_cast<double>(m_iMarkSize)*0.5 + 0.5));
	dblY -= CanvToNormY(static_cast<int>(static_cast<double>(m_iMarkSize)*0.5 + 0.5));
	pCubeMark->move(NormToCanvX(dblX), NormToCanvY(dblY));
	pCubeMark->show();
	m_pAddedMark = pCubeMark;
	canvas()->update();
}

void nraCubeView::OnDestroyMark(int iMarkTag)
{

	/*
	 * Create a nraCubeMark pointer to m_pHotMark
	 * if there is a hot mark and if its mark tag equals
	 * the passed mark tag. 
	 */
	nraCubeMark* pHotMark = 0L;
	if(m_pHotMark)
	{
        pHotMark = dynamic_cast<nraCubeMark*>(m_pHotMark);		
 	    if(pHotMark->markID() != iMarkTag)
 	    {
 	    	pHotMark = 0L;
 	    }
 	}

	
	// We have to delete the hot mark.
    if(pHotMark)
    {
    	// Delete label if one exists.
		if(pHotMark->label())
		{
		    delete pHotMark->label();
		    pHotMark->setLabel(0L);
		}
    	delete m_pHotMark;
    	m_pHotMark = 0L;
    }
    // We have to delete a different mark.
    else
    {
    
 		// Find mark to delete.		
		nraCubeMark* pCubeMark = 0L;
    	QCanvasItemList list = canvas()->allItems();
    	for (QCanvasItemList::Iterator it = list.begin(); it != list.end(); ++it)
    	{	
    	    pCubeMark = dynamic_cast<nraCubeMark*>(*it);
    	    if(pCubeMark)
    	    {
    	    	if(pCubeMark->markID() == iMarkTag)
    	    	{
		    		break;
    	   		}
    	   	}
    	}
    	
    	// Delete it.	
    	if(pCubeMark)
    	{
    	    if(pCubeMark->markID() == iMarkTag)
    	    {
    	    	// Delete label if one exists.
    	    	if(pCubeMark->label())
    	    	{
    	    		delete pCubeMark->label();
    	    		pCubeMark->setLabel(0L);
    	    	}
		    	delete pCubeMark;		
    	    }
    	}
    }
    canvas()->update();
}

void nraCubeView::OnMoveMark(int iMarkTag, double dblX, double dblY)
{
	/*
	 * Create a nraCubeMark pointer to m_pMoveMark
	 * if there is a move mark and if its mark tag equals
	 * the passed mark tag. 
	 */
	nraCubeMark* pMoveMark = 0L;
	if(m_pMoveMark)
	{
        pMoveMark = dynamic_cast<nraCubeMark*>(m_pMoveMark);		
 	    if(pMoveMark->markID() != iMarkTag)
 	    {
 	    	pMoveMark = 0L;
 	    }
 	}

	// We have to move the move mark.
 	if(pMoveMark)
 	{
 		int iX = NormToCanvX(dblX);
 		int iY = NormToCanvY(dblY);
	 	iX = iX - static_cast<int>(static_cast<double>(pMoveMark->x()) + static_cast<double>(pMoveMark->width())*0.5);
	 	iY = iY - static_cast<int>(static_cast<double>(pMoveMark->y()) + static_cast<double>(pMoveMark->height())*0.5);
 		pMoveMark->moveBy(iX, iY);
 	}
 	// We have to move a different mark.
 	else
 	{
 	    // Find mark to move.		
	    nraCubeMark* pCubeMark = 0L;
        QCanvasItemList list = canvas()->allItems();
        for (QCanvasItemList::Iterator it = list.begin(); it != list.end(); ++it)
        {	
        	pCubeMark = dynamic_cast<nraCubeMark*>(*it);
        	if(pCubeMark)
        	{
        		if(pCubeMark->markID() == iMarkTag)
        		{
	    			break;
        		}
        		else
        		{
        			pCubeMark = 0L;
        		}
        	}
        }
    
        // Move it.	
        if(pCubeMark)
        {
 			int iX = NormToCanvX(dblX);
 			int iY = NormToCanvY(dblY);
	 		iX = iX - static_cast<int>(static_cast<double>(pCubeMark->x()) + static_cast<double>(pCubeMark->width())*0.5);
	 		iY = iY - static_cast<int>(static_cast<double>(pCubeMark->y()) + static_cast<double>(pCubeMark->height())*0.5);
	    	pCubeMark->moveBy(NormToCanvX(dblX), NormToCanvY(dblY));		
        }
        
 	}		
    canvas()->update();
}

void nraCubeView::OnSetLabel(int iMarkTag, nraSharedArray<char> Label)
{

	// Find mark to set label.
	nraCubeMark* pCubeMark = 0L;
	QCanvasItemList list = canvas()->allItems();
	for (QCanvasItemList::Iterator it = list.begin(); it != list.end(); ++it)
	{
		pCubeMark = dynamic_cast<nraCubeMark*>(*it);
		if(pCubeMark)
		{
			if(pCubeMark->markID() == iMarkTag)
			{
				break;
			}
			else
			{
				pCubeMark = 0L;
			}
		}
	}

	// If there is a mark with this tag set new label.
	if(pCubeMark)
	{
		//Destroy old label.
		if(pCubeMark->label())
		{
			delete pCubeMark->label();
		}
		pCubeMark->setLabel(0L);
			
		// Place new label if there is a valid one.
		if(Label.IsValid())
		{	
			nraCubeMarkText* pLabel = new nraCubeMarkText(QString(Label.GetPointer()), canvas());
			pLabel->pmoveBy(pCubeMark->x() + pCubeMark->width(), pCubeMark->y() + pCubeMark->height());
			pLabel->setColor(pCubeMark->pen().color());
			pLabel->show();
			pCubeMark->setLabel(pLabel);
		}
		canvas()->update();
	}

}

void nraCubeView::OnSetSlice(int iSliceNmbr)
{

    // Clear canvas.
   	clear();

    // Calc offset for slice.
    int iPageSize =  m_Stack->GetSizeX()*m_Stack->GetSizeY();
    
    int iOffs = (iSliceNmbr - 1) * iPageSize;
    unsigned char* pSlice = (unsigned char*)(&((m_Stack->GetDataPointer()) [iOffs]));
    

    // Set slice as background pixmap for canvas...
    QPixmap Pixmap(QImage(static_cast<uchar*>(pSlice), 
                 m_Stack->GetSizeX(), 
                 m_Stack->GetSizeY(), 
                 8, 
                 m_pLUT, 
                 256, 
                 QImage::systemByteOrder()).scale(canvas()->width(), canvas()->height())); 

	canvas()->setTiles(Pixmap, 1, 1,  canvas()->width(), canvas()->height());

    // ...and redraw it.
    canvas()->update();

}

void nraCubeView::OnSetStack(nraSharedPointer<ViewCube> Stack)
{
	m_Stack = Stack;
	OnSetSlice(1);
}

void nraCubeView::OnDisable()
{
	clear();
	
	/*
	QPainter Painter;
    QRect Rect(0,0,width(),height());
    QColor Black(0,0,0);
    QColor Red(255,0,0);
    
    Painter.begin(this);
    Painter.setBackgroundMode(Qt::OpaqueMode);
    Painter.setBackgroundColor(Black);
    Painter.eraseRect(Rect);
    Painter.setPen(Red);
    Painter.moveTo(0, 0); 
    Painter.lineTo(width(), height());
    Painter.moveTo(width(), 0);
    Painter.lineTo(0, height());
    Painter.end();
	*/
	
	
}


/*
 * As we are working with normalized coordinates we need transformation functions.
 *
 * (0.0 | 0.0)
 *  |
 *  *-----------------------*
 *  |                       |
 *  |                       |
 *  |                       |
 *  |                       |
 *  |                       |  Canvas height
 *  |                       |
 *  |                       |
 *  |                       |
 *  |                       |
 *  *-----------------------*
 *       Canvas width       |
 *                         (1.0|1.0)
 */
double nraCubeView::CanvToNormX(int iX)
{
	return static_cast<double>(iX) / static_cast<double>(canvas()->width());
}

double nraCubeView::CanvToNormY(int iY)
{
	return static_cast<double>(iY) / static_cast<double>(canvas()->height());
}

int nraCubeView::NormToCanvX(double dblX)
{
	return static_cast<int>(static_cast<double>(canvas()->width())*dblX + 0.5);
}

int nraCubeView::NormToCanvY(double dblY)
{
	return static_cast<int>(static_cast<double>(canvas()->height())*dblY + 0.5);
}



