#include "nraCubeMark.h"
#include <qpainter.h>
#include <qrect.h>
#include <qbrush.h>
#include <qpen.h>

nraCubeMark::nraCubeMark(QCanvas* canvas, int iMarkType, int iMarkID, bool boolLabel)
	: 	QCanvasRectangle(canvas), 
		m_iMarkType(iMarkType), 
		m_iMarkID(iMarkID), 
		m_boolActive(true),
		m_boolLabel(boolLabel), 
		m_pLabel(0L)
{
}

nraCubeMark::~nraCubeMark()
{
}

void nraCubeMark::drawShape(QPainter& p)
{

	
	// TL-----------C-----------RT
	//  |           |           |
	//  |    +------|------+    |
	//  |    |      |      |    |
	//  |    |      |  DistTop  |
	//  C----+------C-----------C
	//  |    |      |      |    |
	//  |    |      |  DistBott |
	//  |    +------|------+    |
	//  | DistLeft  | DistRight |
	// BL-----------C-----------RB

	QRect BoundRect = rect();
	QRect InnerRect = BoundRect;
	InnerRect.setLeft(InnerRect.left() + 1);
	InnerRect.setTop(InnerRect.top() + 1);
	InnerRect.setRight(InnerRect.right() - 1);
	InnerRect.setBottom(InnerRect.bottom() - 1);
	
	int iCenterX = BoundRect.left() + ((BoundRect.right() - BoundRect.left()) / 2);
	int iCenterY = BoundRect.top() + ((BoundRect.bottom() - BoundRect.top()) / 2);;
	
	double dblScale = 0.70711;
	
	int iDistLeft = (int)((double)(iCenterX - InnerRect.left())*dblScale + 0.5);
	int iDistRight = (int)((double)(InnerRect.right() - iCenterX)*dblScale + 0.5);
	int iDistTop = (int)((double)(iCenterY - InnerRect.top())*dblScale + 0.5);
	int iDistBottom = (int)((double)(InnerRect.bottom() - iCenterY)*dblScale + 0.5);
	
	int iDistTriang = (int)( ((double)(iDistTop) / 0.5774) * 0.5 /*0.2887*/ );
	
	p.setPen(pen());
	p.setBrush(brush());
	
	switch(m_iMarkType)
	{
	case 0:
		if(m_boolActive)
		{
			p.drawEllipse(InnerRect);
		}
		p.drawLine(iCenterX - iDistLeft, iCenterY + iDistBottom, iCenterX + iDistRight, iCenterY - iDistTop);
		p.drawLine(iCenterX + iDistRight, iCenterY + iDistBottom, iCenterX - iDistLeft, iCenterY - iDistTop);
		break;
	case 1:
		if(m_boolActive)
		{
			p.drawRect(InnerRect);
		}
		p.drawLine(InnerRect.left(), InnerRect.top(), InnerRect.right(), InnerRect.bottom());
		p.drawLine(InnerRect.right(), InnerRect.top(), InnerRect.left(), InnerRect.bottom());
		break;
	case 2:
		if(m_boolActive)
		{
			p.drawLine(iCenterX, InnerRect.top(), InnerRect.right(), iCenterY);
			p.drawLine(InnerRect.right(), iCenterY, iCenterX, InnerRect.bottom());
			p.drawLine(iCenterX, InnerRect.bottom(), InnerRect.left(), iCenterY);
			p.drawLine(InnerRect.left(), iCenterY, iCenterX, InnerRect.top());
		}
		p.drawLine(iCenterX, InnerRect.top(), iCenterX, InnerRect.bottom());
		p.drawLine(InnerRect.left(), iCenterY, InnerRect.right(), iCenterY);
		break;
	case 3:
		if(m_boolActive)
		{
			p.drawLine(iCenterX, InnerRect.top(), InnerRect.right(), iCenterY + iDistTriang);
			p.drawLine(InnerRect.right(), iCenterY + iDistTriang, InnerRect.left(), iCenterY + iDistTriang);
			p.drawLine(InnerRect.left(), iCenterY + iDistTriang, iCenterX, InnerRect.top());	
		}
		p.drawLine(iCenterX, InnerRect.top(), iCenterX, iCenterY);
		p.drawLine(InnerRect.right(), iCenterY + iDistTriang, iCenterX, iCenterY);
		p.drawLine(InnerRect.left(), iCenterY + iDistTriang, iCenterX, iCenterY);
		break;
		
	}
	
	
}
	
void nraCubeMark::moveBy(double dx, double dy)
{
	QCanvasRectangle::moveBy(dx, dy);
	if(m_pLabel)
	{
		m_pLabel->pmoveBy(dx, dy);
	}
	
}

void nraCubeMark::setActive(bool active) 
{
	m_boolActive = active;
	if(isVisible())
	{
		update();
	}
};


