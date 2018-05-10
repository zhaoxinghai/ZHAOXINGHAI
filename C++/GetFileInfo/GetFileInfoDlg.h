
// GetFileInfoDlg.h : 头文件
//

#pragma once

#include <vector>

// CGetFileInfoDlg 对话框
class CGetFileInfoDlg : public CDialogEx
{
// 构造
public:
	CGetFileInfoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GETFILEINFO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
