#pragma once
// Download by http://www.codefans.net
#include "ScrollBarEx.h"

// CScrollBarList

class CScrollBarList : public CListCtrl
{
	DECLARE_DYNAMIC(CScrollBarList)

public:
	CScrollBarList();
	virtual ~CScrollBarList();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	afx_msg void OnPaint();
private:
	CScrollBarEx m_HScroll;
	CScrollBarEx m_VScroll;
};


