// ScrollBarTree.cpp : implementation file
// Download by http://www.codefans.net

#include "stdafx.h"
#include "ScrollBar.h"
#include "ScrollBarTree.h"


// CScrollBarTree

IMPLEMENT_DYNAMIC(CScrollBarTree, CTreeCtrl)

CScrollBarTree::CScrollBarTree()
{

}

CScrollBarTree::~CScrollBarTree()
{
}


BEGIN_MESSAGE_MAP(CScrollBarTree, CTreeCtrl)
	ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CScrollBarTree message handlers



void CScrollBarTree::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	CTreeCtrl::PreSubclassWindow();
	//m_HScroll.InitScrollBar(this, SB_HORZ);
	m_VScroll.InitScrollBar(this, SB_VERT);
}

void CScrollBarTree::OnPaint()
{
	CTreeCtrl::OnPaint();
	//m_HScroll.UpdateScrollBar();
	m_VScroll.UpdateScrollBar();
}

void CScrollBarTree::OnMouseMove(UINT nFlags, CPoint point)
{
    CTreeCtrl::OnMouseMove(nFlags, point);
}
