
#pragma once

class CScrollBarEx : public CStatic
{
public:
	CScrollBarEx();
    virtual ~CScrollBarEx();

    void ScrollDown();
    void ScrollUp();
    void PageUp();
    void PageDown();

    void LoadBmp(CString strPath); //最先调用的函数，不调用加载默认图片
	BOOL InitScrollBar(CWnd *pCtrl, UINT nStyle);
    

	void UpdateScrollBar();
    void DrawArrow(CDC *pDC,CDC *pDCSrc,UINT uArrow,int nState);
    void SetBitmap(HBITMAP hBmp);
    BOOL IsVertical();
 
public:
	afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
    void DrawThumb(CDC *pDestDC, RECT *pDestRect, CDC *pSourDC, RECT *pSourRect);
    void DrawThumbNormal(CDC *pDestDC, RECT *pDestRect, CDC *pSourDC);
    void DrawThumbHover(CDC *pDestDC, RECT *pDestRect, CDC *pSourDC);
    void DrawThumbPress(CDC *pDestDC, RECT *pDestRect, CDC *pSourDC);

    void TileBlt(CDC *pDestDC,RECT *pDestRect,CDC *pSourDC,RECT *pSourRect);
    RECT GetRect(UINT uSBCode);
    RECT GetImageRect(UINT uSBCode,int nState=0);
    UINT HitTest(CPoint pt);

protected:
    CWnd*       m_pCtrl;
    UINT        m_nScrollStyle;
    HBITMAP	    m_hBmp;
    SCROLLINFO	m_si;

    CPoint		m_ptDrag;
    int			m_nDragPos;
    UINT		m_uHtPress;    //按下元素
    UINT		m_uHtHover;    //悬停元素
    BOOL		m_bTrace;      //鼠标追踪

    BOOL        m_bMouseDownThumb;  //鼠标状态
    BOOL        m_bMouseDownArrowUp;
    BOOL        m_bMouseDownArrowDown;

    CRect       m_rcArrUp;    //元素位置
    CRect       m_rcArrDown;
    CRect       m_rcPageUp;
    CRect       m_rcPageDown;
    CRect       m_rcThumb;

    int		    m_nWid;       //宽
    int		    m_nHei;       //高
    int         m_nInterHei;  //中间高度
    int         m_nSlideHei;  //滑块高度
    int         m_nEmptyHei;  //空白高度
};
