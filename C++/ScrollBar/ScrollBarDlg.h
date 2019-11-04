
// ScrollBarDlg.h : header file
//

#pragma once
#include "afxcmn.h"

#include "ScrollBarTree.h"
#include "ScrollBarList.h"

// CScrollBarDlg dialog
class CScrollBarDlg : public CDialog
{
// Construction
public:
	CScrollBarDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SCROLLBAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CScrollBarTree m_tree1;
	CScrollBarList m_list1;
};
