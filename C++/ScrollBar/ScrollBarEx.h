
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

    void LoadBmp(CString strPath); //���ȵ��õĺ����������ü���Ĭ��ͼƬ
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
    UINT		m_uHtPress;    //����Ԫ��
    UINT		m_uHtHover;    //��ͣԪ��
    BOOL		m_bTrace;      //���׷��

    BOOL        m_bMouseDownThumb;  //���״̬
    BOOL        m_bMouseDownArrowUp;
    BOOL        m_bMouseDownArrowDown;

    CRect       m_rcArrUp;    //Ԫ��λ��
    CRect       m_rcArrDown;
    CRect       m_rcPageUp;
    CRect       m_rcPageDown;
    CRect       m_rcThumb;

    int		    m_nWid;       //��
    int		    m_nHei;       //��
    int         m_nInterHei;  //�м�߶�
    int         m_nSlideHei;  //����߶�
    int         m_nEmptyHei;  //�հ׸߶�
};
