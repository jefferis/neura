
#ifndef _NRACUBEMARK_H_
#define _NRACUBEMARK_H_

#include <q3canvas.h>
#include <qcolor.h>
#include <qstring.h>

class QPainter;

class nraCubeMarkText : public QCanvasText
{
public:
	nraCubeMarkText(const QString& text, QCanvas* canvas) : QCanvasText(text, canvas) {};
	virtual ~nraCubeMarkText() {};
	
	virtual void moveBy(double dx, double dy) {};
	void pmoveBy(double dx, double dy) {QCanvasText::moveBy(dx, dy);};
};

class nraCubeMark : public QCanvasRectangle
{

public:
	nraCubeMark(QCanvas* canvas, int iMarkType, int iMarkID, bool boolLabel);
	virtual ~nraCubeMark();
	
	int markType() const {return m_iMarkType;};
	int markID() const  {return m_iMarkID;};
	bool acceptsLabel() const {return m_boolLabel;};
	
	virtual void drawShape(QPainter& p);	
	virtual void moveBy(double dx, double dy);

	void setLabel(nraCubeMarkText* label) {m_pLabel = label;};
	nraCubeMarkText* label() {return m_pLabel;};
	
	void setActive(bool active);
	bool isActive() const {return m_boolActive;};

private:
	const int m_iMarkType;
	const int m_iMarkID;
	bool m_boolActive;
	bool m_boolLabel;
	nraCubeMarkText* m_pLabel;

};
#endif

