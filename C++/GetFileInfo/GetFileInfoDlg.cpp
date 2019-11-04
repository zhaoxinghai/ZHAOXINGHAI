
// GetFileInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GetFileInfo.h"
#include "GetFileInfoDlg.h"
#include "afxdialogex.h"
#include <commoncontrols.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGetFileInfoDlg �Ի���



CGetFileInfoDlg::CGetFileInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GETFILEINFO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetFileInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGetFileInfoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CGetFileInfoDlg ��Ϣ�������

BOOL CGetFileInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    MoveWindow(0, 0, 256*5, 300);

    //get folder icon
    SHFILEINFO shfi;
    ZeroMemory(&shfi, sizeof(SHFILEINFO));
    HRESULT hr = SHGetFileInfo(L"C:\\Windows",
        -1,
        &shfi,
        sizeof(shfi),
        SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);
    m_iFolderIcon = shfi.iIcon;

    //get all system icon
    HRESULT hResult = 0;
    hResult = SHGetImageList(SHIL_SMALL, IID_IImageList, (void**)&m_pImageList_16);
    hResult = SHGetImageList(SHIL_LARGE, IID_IImageList, (void**)&m_pImageList_32);
    hResult = SHGetImageList(SHIL_EXTRALARGE, IID_IImageList, (void**)&m_pImageList_48);
    hResult = SHGetImageList(SHIL_JUMBO, IID_IImageList, (void**)&m_pImageList_256);

    GetAllPath(L"C:\\Users\\H158837\\1Person\\mClound\\");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGetFileInfoDlg::GetAllPath(CString strPath)
{
    SHFILEINFO shfi;
    ZeroMemory(&shfi, sizeof(SHFILEINFO));

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = ::FindFirstFile(strPath + "*", &FindFileData);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        return;
    }

    while (FindNextFile(hFind, &FindFileData))
    {
        CString tmp = strPath;
        tmp.Append(FindFileData.cFileName);

        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
            continue;

        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            m_vPathIcon.push_back(m_iFolderIcon);
        }
        else
        {
            //�ļ�
            HRESULT hr = SHGetFileInfo(tmp,
                FILE_ATTRIBUTE_NORMAL,
                &shfi,
                sizeof(shfi),
                SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);

            m_vPathIcon.push_back(shfi.iIcon);
        }
    }
    FindClose(hFind);
}

void CGetFileInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CGetFileInfoDlg::OnPaint()
{
    CPaintDC dc(this);

    CRect rc;
    GetClientRect(&rc);
    CMemDC memDC(dc, rc);
    memDC.GetDC().FillSolidRect(0, 0, rc.Width(), rc.Height(), RGB(255, 255, 255));

    int x = 0;
    int y = 0;
    for (unsigned int i = 0; i < m_vPathIcon.size(); i++)
    {
        DrawIcon256(m_vPathIcon[i],&memDC.GetDC(), x, y);
        x += 256;
    }
}

void CGetFileInfoDlg::DrawIcon256(int index, CDC *pDC, int x, int y)
{
    //ͼ����С��ʾ
    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.AlphaFormat = AC_SRC_ALPHA;
    bf.SourceConstantAlpha = 255;

    CDC tempDC;
    tempDC.CreateCompatibleDC(pDC);
    CBitmap tempBmp;
    tempBmp.CreateCompatibleBitmap(pDC, 256, 256);
    tempDC.SelectObject(&tempBmp);
    tempDC.FillSolidRect(0, 0, 256, 256, RGB(255, 255, 255));

    CDC dcColorIcon;
    dcColorIcon.CreateCompatibleDC(pDC);
    CBitmap cbColorIcon;
    cbColorIcon.CreateCompatibleBitmap(pDC, 256, 256);
    dcColorIcon.SelectObject(&cbColorIcon);
    ImageList_Draw((HIMAGELIST)m_pImageList_256, index, dcColorIcon.GetSafeHdc(), 0, 0, ILD_TRANSPARENT);

    tempDC.AlphaBlend(0, 0, 256, 256, &dcColorIcon, 0, 0, 256, 256, bf);
    pDC->SetStretchBltMode(HALFTONE);
    pDC->StretchBlt(x, y, 256, 256, &tempDC, 0, 0, 256, 256, SRCCOPY);
    CPoint pt;
    ::SetBrushOrgEx(pDC->m_hDC, 0, 0, &pt);
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGetFileInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

