#pragma once

#include "ScrollBarEx.h"

// CScrollBarTree

class CScrollBarTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CScrollBarTree)

public:
	CScrollBarTree();
	virtual ~CScrollBarTree();

protected:
	DECLARE_MESSAGE_MAP()
private:
	//CScrollBarEx m_HScroll;
	CScrollBarEx m_VScroll;
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


