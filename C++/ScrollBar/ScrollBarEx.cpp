// ScrollBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "ScrollBarEx.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SCROLLBAR_NORMAL_MINIMUM 45  //经验值,当Scrollbar宽度/高度小于这一数值时需要特殊处理,保证与系统的处理方式一致

#define TIMERID_NOTIFY	100
#define TIMERID_DELAY	200
#define TIME_DELAY		500
#define TIME_INTER		100

#define THUMB_BORDER	5
#define THUMB_MINSIZE	(THUMB_BORDER*10)

CScrollBarEx::CScrollBarEx()
{
    m_bMouseDownThumb = FALSE;
    m_bMouseDownArrowUp = FALSE;
    m_bMouseDownArrowDown = FALSE;

    m_hBmp=NULL;
    m_pCtrl = NULL;
    memset(&m_si,0,sizeof(SCROLLINFO));

    m_nScrollStyle = SB_VERT;
    m_uHtHover = -1;
    m_uHtPress = -1;
    m_bTrace=FALSE;
    m_ptDrag.SetPoint(0,0);
    m_nDragPos = 0;
  
    m_nWid = 0;
    m_nHei = 0;
    m_nInterHei = 0;
    m_nSlideHei = 0;
    m_nEmptyHei = 0;

    m_rcArrUp.SetRectEmpty();
    m_rcArrDown.SetRectEmpty();
    m_rcThumb.SetRectEmpty();
    m_rcPageUp.SetRectEmpty();
    m_rcPageDown.SetRectEmpty();
}

CScrollBarEx::~CScrollBarEx()
{
}


BEGIN_MESSAGE_MAP(CScrollBarEx, CStatic)
    ON_WM_SIZE()
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

void CScrollBarEx::OnPaint() 
{
    CPaintDC dc(this); // device context for painting
    if(!m_hBmp) return;

    CRect rc;
    GetClientRect(&rc);

    CDC memdc;
    memdc.CreateCompatibleDC(&dc);
    HGDIOBJ hOldBmp=::SelectObject(memdc,m_hBmp);

    dc.FillSolidRect(0,0,rc.Width(),rc.Height(),RGB(255,255,255));

    //Arrow
    if(m_uHtPress==SB_LINEUP)
    {
        DrawArrow(&dc,&memdc,SB_LINEUP,2);
    }
    else if(m_uHtHover==SB_LINEUP)
    {
        DrawArrow(&dc,&memdc,SB_LINEUP,1);
    }
    else
    {
        DrawArrow(&dc,&memdc,SB_LINEUP,0);
    }
    if(m_uHtPress==SB_LINEDOWN)
    {
        DrawArrow(&dc,&memdc,SB_LINEDOWN,2);
    }
    else if(m_uHtHover==SB_LINEDOWN)
    {
        DrawArrow(&dc,&memdc,SB_LINEDOWN,1);
    }
    else
    {
        DrawArrow(&dc,&memdc,SB_LINEDOWN,0);
    }
    //滑块
    CRect rcDest=GetRect(SB_THUMBTRACK);
    if(m_bMouseDownThumb) 
    {
        DrawThumbPress(&dc,&rcDest,&memdc);
    }
    else if(m_uHtHover==SB_THUMBTRACK)
    {
        DrawThumbHover(&dc,&rcDest,&memdc); 
    }
    else
    {
        DrawThumbNormal(&dc,&rcDest,&memdc);
    }
}

BOOL CScrollBarEx::InitScrollBar(CWnd *pCtrl, UINT nStyle)
{
    if (pCtrl == NULL) 
    {
        return FALSE;
    }
    m_pCtrl = pCtrl;
	
    int nx, ny;
    if (nStyle == SB_VERT) 
    {
        nx = ::GetSystemMetrics(SM_CXVSCROLL);
        ny = ::GetSystemMetrics(SM_CYVSCROLL);
    }
    else if(nStyle == SB_HORZ)
    {
        nx = GetSystemMetrics(SM_CXHSCROLL);
        ny = GetSystemMetrics(SM_CYHSCROLL);
    }
	else
	{
		return FALSE;
	}
    m_nScrollStyle = nStyle;


	CRect rect;
    pCtrl->GetWindowRect(rect);
	pCtrl->ModifyStyle(0, WS_CLIPSIBLINGS);

	CWnd* pParent = pCtrl->GetParent();
    pParent->ScreenToClient(rect);
    
    if (nStyle == SB_VERT) 
    {
        rect.left = rect.right- nx;		
    }
    else if(nStyle == SB_HORZ)
    {
        rect.top = rect.bottom - ny;
    }
    BOOL bResult = Create(NULL,WS_CHILD|SS_NOTIFY|SS_LEFT|WS_EX_TOPMOST, rect, pParent);
    
    if (!bResult)
    {
        return FALSE;
    }
    //图片为空是加载默认图片
    if(m_hBmp==NULL)
    {
        //加载图片
        CString strImgPath;
        /*!< 获取系统信息 */
        TCHAR szPathName[MAX_PATH] = { 0 };
        LPTSTR pszDir = szPathName;
        if (::GetModuleFileName( NULL, szPathName, MAX_PATH ) != 0)
        {
            LPTSTR pszTmp = StrRChrI( pszDir, pszDir+MAX_PATH, _T('\\') );
            if ( pszTmp )
            {
                *pszTmp = 0;
            }
            strImgPath = szPathName;

        }
        CString tstrVscrollUp_n = strImgPath+_T("\\scrollbar.bmp");	//12*12

        ATL::CImage bigImage;
        bigImage.Load(tstrVscrollUp_n.GetBuffer()  );
        tstrVscrollUp_n.ReleaseBuffer();
        SetBitmap(bigImage.Detach());
    }
	UpdateScrollBar();
    return TRUE;
}
void CScrollBarEx::UpdateScrollBar()
{
	if(!IsWindow(m_hWnd) || m_pCtrl == NULL)
	{
		return;
	}
	m_pCtrl->SetWindowPos(&wndBottom, 0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);

	SCROLLINFO sf;
	memset(&sf, 0, sizeof(SCROLLINFO));
	sf.cbSize = sizeof(SCROLLINFO);
	m_pCtrl->GetScrollInfo(m_nScrollStyle, &sf, SIF_ALL);
    if (sf.nPage < 0) 
    {
		ShowWindow(SW_HIDE);
        return;
    }

	SCROLLBARINFO hscr;
	memset(&hscr, 0, sizeof(SCROLLBARINFO));
	hscr.cbSize = sizeof(SCROLLBARINFO);
	CRect rcScrollBar;

	if(m_nScrollStyle == SB_VERT)
	{
		::GetScrollBarInfo(m_pCtrl->m_hWnd, OBJID_VSCROLL, &hscr);
		rcScrollBar = CRect(hscr.rcScrollBar);
	}
	else if(m_nScrollStyle == SB_HORZ)
	{
		::GetScrollBarInfo(m_pCtrl->m_hWnd, OBJID_HSCROLL, &hscr);
		rcScrollBar = CRect(hscr.rcScrollBar);
	}
    m_pCtrl->GetScrollInfo(m_nScrollStyle,&m_si);
	GetParent()->ScreenToClient(&rcScrollBar);
	SetWindowPos(NULL, rcScrollBar.left, rcScrollBar.top, rcScrollBar.Width(), rcScrollBar.Height(), SWP_SHOWWINDOW|SWP_NOZORDER);
    Invalidate();
}
BOOL CScrollBarEx::IsVertical()
{
    return m_nScrollStyle==SB_VERT;
}

UINT CScrollBarEx::HitTest( CPoint pt )
{
    int nTestPos=pt.y;
    if(!IsVertical()) nTestPos=pt.x;
    if(nTestPos<0) return -1;
    SCROLLINFO si=m_si;
    int nInterHei=m_nHei-2*m_nWid;
    if(nInterHei<0) nInterHei=0;
    int	nSlideHei=si.nPage*nInterHei/(si.nMax-si.nMin+1);
    if(nSlideHei<THUMB_MINSIZE) nSlideHei=THUMB_MINSIZE;
    if(nInterHei<THUMB_MINSIZE) nSlideHei=0;
    int nEmptyHei=nInterHei-nSlideHei;

    int nArrowHei=(nInterHei==0)?(m_nHei/2):m_nWid;
    int nBottom=0;
    int nSegLen=nArrowHei;
    nBottom+=nSegLen;
    UINT uHit=SB_LINEUP;
    if(nTestPos<nBottom) goto end;
    
    uHit=SB_PAGEUP;
    if((si.nMax-si.nMin-si.nPage+1)==0)
        nSegLen=nEmptyHei/2;
    else
        nSegLen=nEmptyHei*si.nPos/(si.nMax-si.nMin-si.nPage+1);
    nBottom+=nSegLen;
    if(nTestPos<nBottom) goto end;
    nSegLen=nSlideHei;
    nBottom+=nSegLen;
    uHit=SB_THUMBTRACK;
    if(nTestPos<nBottom) goto end;
    nBottom=m_nHei-nArrowHei;
    uHit=SB_PAGEDOWN;
    if(nTestPos<nBottom) goto end;
    uHit=SB_LINEDOWN;
end:
    return uHit;
}

void CScrollBarEx::SetBitmap( HBITMAP hBmp )
{
    ASSERT(m_hWnd);
    m_hBmp=hBmp;
    BITMAP bm;
    GetObject(hBmp,sizeof(bm),&bm);
    m_nWid=bm.bmWidth/9;
    CRect rc;
    GetWindowRect(&rc);
    GetParent()->ScreenToClient(&rc);
    if(IsVertical())
    {
        rc.right=rc.left+m_nWid;
    }else
    {
        rc.bottom=rc.top+m_nWid;
    }
    MoveWindow(&rc);
}

void CScrollBarEx::OnSize( UINT nType, int cx, int cy )
{
    CWnd::OnSize(nType, cx, cy);
    m_nHei=IsVertical()?cy:cx;
}

RECT CScrollBarEx::GetImageRect( UINT uSBCode,int nState/*=0*/ )
{
    int nIndex=0;
    switch(uSBCode)
    {
    case SB_LINEUP:nIndex=0;break;
    case SB_PAGEUP:nIndex=3;break;
    case SB_THUMBTRACK:nIndex=2;break;
    case SB_PAGEDOWN:nIndex=3;break;
    case SB_LINEDOWN:nIndex=1;break;
    }
    if(!IsVertical())nIndex+=4;
    RECT rcRet={m_nWid*nIndex,m_nWid*nState,m_nWid*(nIndex+1),m_nWid*(nState+1)};
    return rcRet;
}

RECT CScrollBarEx::GetRect( UINT uSBCode )
{
    SCROLLINFO si=m_si;
    si.nPos=si.nPos;
    
    //中间高度
    m_nInterHei=m_nHei-2*m_nWid;
    if(m_nInterHei<0) m_nInterHei=0;

    //滑块高度
    m_nSlideHei=si.nPage*m_nInterHei/(si.nMax-si.nMin+1);
    if(m_nSlideHei<THUMB_MINSIZE) m_nSlideHei=THUMB_MINSIZE;
    if(m_nInterHei<THUMB_MINSIZE) m_nSlideHei=0;

    //空白高度
    if((si.nMax-si.nMin-si.nPage+1)==0) m_nInterHei=0;
    m_nEmptyHei=m_nInterHei-m_nSlideHei;

    //Arrow高度==宽度
    int nArrowHei=m_nWid;
    if(m_nInterHei==0) nArrowHei=m_nHei/2;
    RECT rcRet={0,0,m_nWid,nArrowHei};

    if(uSBCode==SB_LINEUP) //上按扭 
    {   
        goto end;
    }
    rcRet.top=rcRet.bottom;
    if((si.nMax-si.nMin-si.nPage+1)==0)
        rcRet.bottom+=m_nEmptyHei/2;
    else
        rcRet.bottom+=m_nEmptyHei*si.nPos/(si.nMax-si.nMin-si.nPage+1);

    if(uSBCode==SB_PAGEUP) //上翻页
    {
        goto end;
    }
    rcRet.top=rcRet.bottom;
    rcRet.bottom+=m_nSlideHei;
    if(uSBCode==SB_THUMBTRACK) 
    {
        goto end;
    }
    rcRet.top=rcRet.bottom;
    rcRet.bottom=m_nHei-nArrowHei;
    if(uSBCode==SB_PAGEDOWN)   //下翻页
    {
        goto end;
    }
    rcRet.top=rcRet.bottom;
    rcRet.bottom=m_nHei;
    if(uSBCode==SB_LINEDOWN) 
    {
        goto end;
    }
end:
    if(!IsVertical())
    {
        int t=rcRet.left;
        rcRet.left=rcRet.top;
        rcRet.top=t;
        t=rcRet.right;
        rcRet.right=rcRet.bottom;
        rcRet.bottom=t;
    }
    //保存矩形
    switch(uSBCode)
    {
    case SB_LINEUP:
        m_rcArrUp = rcRet;
        break;
    case SB_LINEDOWN:
        m_rcArrDown = rcRet;
        break;
    case SB_PAGEUP:
        m_rcPageUp = rcRet;
        break;
    case SB_PAGEDOWN:
        m_rcPageDown = rcRet;
        break;
    case SB_THUMBTRACK:
        m_rcThumb = rcRet;
        break;
    default:
        {
            break;
        }
    }
    return rcRet;
}
void CScrollBarEx::TileBlt( CDC *pDestDC,RECT *pDestRect,CDC *pSourDC,RECT *pSourRect )
{
    int nSourHei=pSourRect->bottom-pSourRect->top;
    int nSourWid=pSourRect->right-pSourRect->left;

    int y=pDestRect->top;
    while(y<pDestRect->bottom)
    {
        int nHei=nSourHei;
        if(y+nHei>pDestRect->bottom) nHei=pDestRect->bottom-y;

        int x=pDestRect->left;
        while(x<pDestRect->right)
        {
            int nWid=nSourWid;
            if(x+nWid>pDestRect->right) nWid=pDestRect->right-x;
            pDestDC->BitBlt(x,y,nWid,nHei,pSourDC,pSourRect->left,pSourRect->top,SRCCOPY);
            x+=nWid;
        }
        y+=nHei;
    }
}
void CScrollBarEx::DrawThumb(CDC *pDestDC, RECT *pDestRect, CDC *pSourDC, RECT *pSourRect)
{
    if(IsRectEmpty(pDestRect)) return;
    RECT rcDest=*pDestRect,rcSour=*pSourRect;
    if(IsVertical())
    {
        ASSERT(pDestRect->bottom-pDestRect->top>=THUMB_MINSIZE);
        pDestDC->BitBlt(pDestRect->left,pDestRect->top,m_nWid,THUMB_BORDER,pSourDC,pSourRect->left,pSourRect->top,SRCCOPY);
        pDestDC->BitBlt(pDestRect->left,pDestRect->bottom-THUMB_BORDER,m_nWid,THUMB_BORDER,pSourDC,pSourRect->left,pSourRect->bottom-THUMB_BORDER,SRCCOPY);
        rcDest.top+=THUMB_BORDER,rcDest.bottom-=THUMB_BORDER;
        rcSour.top+=THUMB_BORDER,rcSour.bottom-=THUMB_BORDER;
        TileBlt(pDestDC,&rcDest,pSourDC,&rcSour);
    }
    else
    {
        ASSERT(pDestRect->right-pDestRect->left>=THUMB_MINSIZE);
        pDestDC->BitBlt(pDestRect->left,pDestRect->top,THUMB_BORDER,m_nWid,pSourDC,pSourRect->left,pSourRect->top,SRCCOPY);
        pDestDC->BitBlt(pDestRect->right-THUMB_BORDER,pDestRect->top,THUMB_BORDER,m_nWid,pSourDC,pSourRect->right-THUMB_BORDER,pSourRect->top,SRCCOPY);
        rcDest.left+=THUMB_BORDER,rcDest.right-=THUMB_BORDER;
        rcSour.left+=THUMB_BORDER,rcSour.right-=THUMB_BORDER;
        TileBlt(pDestDC,&rcDest,pSourDC,&rcSour);
    }
}
void CScrollBarEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
    SetCapture();
    m_ptDrag=point;
    UINT uHit=HitTest(point);
    if(uHit==SB_THUMBTRACK)
    {
        m_bMouseDownThumb = TRUE;
        m_si.nPos=m_si.nPos;
        m_nDragPos=m_si.nPos;
    }
    else if(uHit==SB_LINEUP)
    {
        m_uHtPress = uHit;
        m_bMouseDownArrowUp = TRUE;
        ScrollUp();
        SetTimer(2,250,NULL);
    }
    else if(uHit==SB_LINEDOWN)
    {
        m_uHtPress = uHit;
        m_bMouseDownArrowDown = TRUE;
        ScrollDown();
        SetTimer(2,250,NULL);
    }
    Invalidate();
}
void CScrollBarEx::OnMouseMove(UINT nFlags, CPoint point) 
{
    if(!m_bTrace && nFlags!=-1)
    {
        m_bTrace=TRUE;
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.hwndTrack = m_hWnd;
        tme.dwFlags = TME_LEAVE;
        tme.dwHoverTime = 1;
        m_bTrace = _TrackMouseEvent(&tme);
    }
    if(m_bMouseDownThumb)
    {  
        int nDragLen=IsVertical()?point.y-m_ptDrag.y:point.x-m_ptDrag.x;
        int nSlide=(m_nEmptyHei==0)?0:(nDragLen*(int)(m_si.nMax-m_si.nMin-m_si.nPage+1)/m_nEmptyHei);

        int nNewTrackPos=m_nDragPos+nSlide;
        if(nNewTrackPos<m_si.nMin)
        {
            nNewTrackPos=m_si.nMin;
        }
        else if(nNewTrackPos>(int)(m_si.nMax-m_si.nMin-m_si.nPage+1))
        {
            nNewTrackPos=m_si.nMax-m_si.nMin-m_si.nPage+1;
        }
        if(nNewTrackPos!=m_si.nPos)
        {
            if(nNewTrackPos>m_si.nPos)
            {
                int pos = nNewTrackPos-m_si.nPos;
                while(pos>0)
                {
                    ScrollDown();
                    pos--;
                }
            }
            else
            {
                int pos = m_si.nPos - nNewTrackPos;
                while(pos>0)
                {
                    ScrollUp();
                    pos--;
                }
            }
            m_si.nPos = nNewTrackPos;
        }
    }
    else
    {
        UINT uHit=HitTest(point);
        if(uHit!=m_uHtHover)
        {
            m_uHtHover = uHit;
            Invalidate();
        }
    }
    CStatic::OnMouseMove(nFlags,point);
}

void CScrollBarEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
    KillTimer(1);
    ReleaseCapture();
 
    m_uHtPress = -1;
    m_bMouseDownThumb = FALSE;
    m_bMouseDownArrowUp = FALSE;
    m_bMouseDownArrowDown = FALSE;
    Invalidate();
}
void CScrollBarEx::DrawArrow(CDC *pDC,CDC *pDCSrc,UINT uArrow, int nState)
{
    ASSERT(uArrow==SB_LINEUP||uArrow==SB_LINEDOWN);
    
    RECT rcDest=GetRect(uArrow);
    RECT rcSour=GetImageRect(uArrow,nState);
    pDC->BitBlt(rcDest.left,rcDest.top,m_nWid,m_nWid,pDCSrc,rcSour.left,rcSour.top,SRCCOPY);
}
LRESULT CScrollBarEx::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
    m_bTrace = FALSE;
    OnMouseMove(-1,CPoint(-1,-1));
    return 0;
}
void CScrollBarEx::PageDown()
{
    m_pCtrl->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL, 
        MAKELONG(IsVertical()?SB_PAGEDOWN:SB_PAGERIGHT,0),NULL);
}

void CScrollBarEx::PageUp()
{
    m_pCtrl->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL, 
        MAKELONG(IsVertical()?SB_PAGEUP:SB_PAGELEFT,0),NULL);
}

void CScrollBarEx::ScrollUp()
{
    m_pCtrl->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL, 
        MAKELONG(IsVertical()?SB_LINEUP:SB_LINELEFT,0),NULL);
}

void CScrollBarEx::ScrollDown()
{
    m_pCtrl->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL, 
        MAKELONG(IsVertical()?SB_LINEDOWN:SB_LINERIGHT,0),NULL);
}
void CScrollBarEx::OnTimer(UINT nIDEvent) 
{
    if(nIDEvent == 1)
    {
        if(m_bMouseDownArrowDown)
        {
            ScrollDown();
        }
        else if(m_bMouseDownArrowUp)
        {
            ScrollUp();
        }
    }
    else if(nIDEvent == 2)
    {
        if(m_bMouseDownArrowDown)
        {
            KillTimer(2);
            SetTimer(1, 50, NULL);
        }
        else if(m_bMouseDownArrowUp)
        {
            KillTimer(2);
            SetTimer(1, 50, NULL);
        }
    }
}

void CScrollBarEx::DrawThumbNormal( CDC *pDestDC, RECT *pDestRect, CDC *pSourDC )
{
    RECT rcSour={0,0,m_nWid,m_nWid};
    OffsetRect(&rcSour,m_nWid*2,0);

    DrawThumb(pDestDC,pDestRect,pSourDC,&rcSour);
}

void CScrollBarEx::DrawThumbHover( CDC *pDestDC, RECT *pDestRect, CDC *pSourDC )
{
    RECT rcSour={0,0,m_nWid,m_nWid};
    OffsetRect(&rcSour,m_nWid*2,m_nWid);

    DrawThumb(pDestDC,pDestRect,pSourDC,&rcSour);
}

void CScrollBarEx::DrawThumbPress( CDC *pDestDC, RECT *pDestRect, CDC *pSourDC )
{
    RECT rcSour={0,0,m_nWid,m_nWid};
    OffsetRect(&rcSour,m_nWid*2,m_nWid*2);

    DrawThumb(pDestDC,pDestRect,pSourDC,&rcSour);
}

void CScrollBarEx::LoadBmp( CString strPath )
{
    ATL::CImage bigImage;
    bigImage.Load(strPath.GetBuffer()  );
    strPath.ReleaseBuffer();
    SetBitmap(bigImage.Detach());
}