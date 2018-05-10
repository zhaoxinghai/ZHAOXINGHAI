
// GetFileInfoDlg.h : ͷ�ļ�
//

#pragma once

#include <vector>

// CGetFileInfoDlg �Ի���
class CGetFileInfoDlg : public CDialogEx
{
// ����
public:
	CGetFileInfoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GETFILEINFO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    void GetAllPath(CString strPath);
    void DrawIcon256(int index, CDC *pDC, int x, int y);

protected:
    IImageList*   m_pImageList_16;
    IImageList*   m_pImageList_32;
    IImageList*   m_pImageList_48;
    IImageList*   m_pImageList_256;

    int               m_iFolderIcon;
    std::vector<int>  m_vPathIcon;
};
