// ScrollBarList.cpp : implementation file
// Download by http://www.codefans.net

#include "stdafx.h"
#include "ScrollBar.h"
#include "ScrollBarList.h"


// CScrollBarList

IMPLEMENT_DYNAMIC(CScrollBarList, CListCtrl)

CScrollBarList::CScrollBarList()
{

}

CScrollBarList::~CScrollBarList()
{
}


BEGIN_MESSAGE_MAP(CScrollBarList, CListCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CScrollBarList message handlers



void CScrollBarList::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	CListCtrl::PreSubclassWindow();
	//m_HScroll.InitScrollBar(this, SB_HORZ);
	//m_VScroll.InitScrollBar(this, SB_VERT);
}

void CScrollBarList::OnPaint()
{
//	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CListCtrl::OnPaint() for painting messages

	CListCtrl::OnPaint();
	//m_HScroll.UpdateScrollBar();
	//m_VScroll.UpdateScrollBar();
}
